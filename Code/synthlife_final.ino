#include <Wire.h>
#include <MAX30100_PulseOximeter.h>
#include <DFRobot_MLX90614.h>
#include <OakOLED.h>

#define REPORTING_PERIOD_MS 1000 
#define GSR_PIN A0  // Define the GSR sensor pin


OakOLED oled;
PulseOximeter pox;
DFRobot_MLX90614_I2C mlx;

// Define the fuzzy membership functions
void gsrMembershipValue(float gsr, float &low, float &medium, float &high, float &veryHigh) {
  if (gsr <= 2.0) {
    low = 1.0;
    medium = high = veryHigh = 0.0;
  } else if (gsr <= 5.0) {
    low = (5.0 - gsr) / (5.0 - 2.0);
    medium = (gsr - 2.0) / (5.0 - 2.0);
    high = veryHigh = 0.0;
  } else if (gsr <= 8.0) {
    low = medium = (8.0 - gsr) / (8.0 - 5.0);
    medium = (gsr - 5.0) / (8.0 - 5.0);
    high = (8.0 - gsr) / (8.0 - 5.0);
    veryHigh = 0.0;
  } else {
    low = medium = high = (gsr - 8.0) / (10.0 - 8.0);
    veryHigh = 1.0;
  }
}

void tempMembershipValue(float temp, float &low, float &medium, float &high, float &veryHigh) {
  if (temp <= 34.0) {
    low = 1.0;
    medium = high = veryHigh = 0.0;
  } else if (temp <= 37.0) {
    low = (37.0 - temp) / (37.0 - 34.0);
    medium = (temp - 34.0) / (37.0 - 34.0);
    high = veryHigh = 0.0;
  } else if (temp <= 39.0) {
    low = medium = (39.0 - temp) / (39.0 - 37.0);
    medium = (temp - 37.0) / (39.0 - 37.0);
    high = (39.0 - temp) / (39.0 - 37.0);
    veryHigh = 0.0;
  } else {
    low = medium = high = (temp - 39.0) / (42.0 - 39.0);
    veryHigh = 1.0;
  }
}

void hrMembershipValue(float hr, float &low, float &medium, float &high, float &veryHigh) {
  if (hr <= 60.0) {
    low = 1.0;
    medium = high = veryHigh = 0.0;
  } else if (hr <= 80.0) {
    low = (80.0 - hr) / (80.0 - 60.0);
    medium = (hr - 60.0) / (80.0 - 60.0);
    high = veryHigh = 0.0;
  } else if (hr <= 100.0) {
    low = medium = (100.0 - hr) / (100.0 - 80.0);
    medium = (hr - 80.0) / (100.0 - 80.0);
    high = (100.0 - hr) / (100.0 - 80.0);
    veryHigh = 0.0;
  } else {
    low = medium = high = (hr - 100.0) / (120.0 - 100.0);
    veryHigh = 1.0;
  }
}

// Fuzzy associative memory based on the research
String getStressState(float gsrLow, float gsrMedium, float gsrHigh, float gsrVeryHigh,
                      float tempLow, float tempMedium, float tempHigh, float tempVeryHigh,
                      float hrLow, float hrMedium, float hrHigh, float hrVeryHigh) {
  String gsrState = gsrLow > max(gsrMedium, max(gsrHigh, gsrVeryHigh)) ? "Low" :
                     (gsrMedium > max(gsrHigh, gsrVeryHigh)) ? "Medium" :
                     (gsrHigh > gsrVeryHigh) ? "High" : "Very High";

  String tempState = tempLow > max(tempMedium, max(tempHigh, tempVeryHigh)) ? "Low" :
                      (tempMedium > max(tempHigh, tempVeryHigh)) ? "Medium" :
                      (tempHigh > tempVeryHigh) ? "High" : "Very High";

  String hrState = hrLow > max(hrMedium, max(hrHigh, hrVeryHigh)) ? "Low" :
                    (hrMedium > max(hrHigh, hrVeryHigh)) ? "Medium" :
                    (hrHigh > hrVeryHigh) ? "High" : "Very High";

  if (gsrState == "Low") {
    if (tempState == "Low") {
      if (hrState == "Low") return "Relax";
      else return "Anxious";
    } else if (tempState == "Medium") {
      if (hrState == "Low") return "Relax";
      else if (hrState == "Medium") return "Calm";
      else return "Anxious";
    } else if (tempState == "High" || tempState == "Very High") {
      if (hrState == "Low") return "Relax";
      else if (hrState == "Medium" || hrState == "High") return "Calm";
      else return "Anxious";
    }
  } else if (gsrState == "Medium") {
    if (tempState == "Low") {
      if (hrState == "Very High") return "Stress";
      else return "Anxious";
    } else if (tempState == "Medium" || tempState == "High") {
      if (hrState == "Low" || hrState == "Medium") return "Calm";
      else return "Anxious";
    } else if (tempState == "Very High") {
      if (hrState == "Low") return "Relax";
      else if (hrState == "Very High") return "Relax";
      else return "Calm";
    }
  } else if (gsrState == "High") {
    if (tempState == "Low") {
      if (hrState == "Low") return "Anxious";
      else if (hrState == "Medium") return "Calm";
      else return "Anxious";
    } else if (tempState == "Medium") {
      if (hrState == "Low") return "Anxious";
      else if (hrState == "Medium" || hrState == "High") return "Anxious";
    } else if (tempState == "High" || tempState == "Very High") {
      if (hrState == "Low" || hrState == "Medium") return "Calm";
      else return "Anxious";
    }
  } else if (gsrState == "Very High") {
    if (tempState == "Low" || tempState == "Medium" || tempState == "High") {
      if (hrState == "High" || hrState == "Very High") return "Stress";
      else return "Anxious";
    } else if (tempState == "Very High") {
      if (hrState == "Low" || hrState == "Medium") return "Calm";
      else return "Anxious";
    }
  }
  return "Unknown"; // In case no match is found (shouldn't happen)
}

// Map the stress state to a score
int getStressScore(String stressState, float membershipValue) {
  if (stressState == "Relax") {
    return map(membershipValue * 100, 0, 100, 1, 25);  // Relax: 1-25
  } else if (stressState == "Calm") {
    return map(membershipValue * 100, 0, 100, 26, 50); // Calm: 26-50
  } else if (stressState == "Anxious") {
    return map(membershipValue * 100, 0, 100, 51, 75); // Anxious: 51-75
  } else if (stressState == "Stress") {
    return map(membershipValue * 100, 0, 100, 76, 100); // Stress: 76-100
  } else {
    return 0;  // Default value in case of error
  }
}

void setup() 
{
  Serial.begin(9600);
  mlx.begin();
  oled.begin();
  pox.begin();
  Wire.setClock(100000);     
}                      

void loop()
{
  pox.update();
  static uint32_t tsLastReport = 0;
  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
    //Serial.print("Heart BPM: ");
    //Serial.print(pox.getHeartRate());
    //Serial.print(" Oxygen Percent: ");
    //Serial.println(pox.getSpO2());
    float spo2Value = pox.getSpO2();
    float hrValue = pox.getHeartRate();
    float tempValue = mlx.getObjectTempCelsius();
    float gsrValue = analogRead(GSR_PIN);

    // Calculate membership values
    float gsrLow, gsrMedium, gsrHigh, gsrVeryHigh;
    gsrMembershipValue(gsrValue, gsrLow, gsrMedium, gsrHigh, gsrVeryHigh);

    float tempLow, tempMedium, tempHigh, tempVeryHigh;
    tempMembershipValue(tempValue, tempLow, tempMedium, tempHigh, tempVeryHigh);

    float hrLow, hrMedium, hrHigh, hrVeryHigh;
    hrMembershipValue(hrValue, hrLow, hrMedium, hrHigh, hrVeryHigh);

    // Determine the stress state
    String stressState = getStressState(gsrLow, gsrMedium, gsrHigh, gsrVeryHigh,
                                        tempLow, tempMedium, tempHigh, tempVeryHigh,
                                        hrLow, hrMedium, hrHigh, hrVeryHigh);

    // Calculate membership value for the stress state
    float membershipValue = max(max(gsrLow, gsrMedium), max(gsrHigh, gsrVeryHigh));
    membershipValue = max(membershipValue, max(max(tempLow, tempMedium), max(tempHigh, tempVeryHigh)));
    membershipValue = max(membershipValue, max(max(hrLow, hrMedium), max(hrHigh, hrVeryHigh)));

    // Map the stress state to a score within a range
    int stressScore = getStressScore(stressState, membershipValue);


    oled.clearDisplay();
    oled.setCursor(0, 0);
    oled.print("BPM: ");
    oled.print(hrValue);
    oled.setCursor(70, 0);
    oled.print("SPO2: ");
    oled.print(spo2Value);

    //float ambientTemp = mlx.getAmbientTempCelsius();
    //Serial.print(" ----- Temperature: ");
    //Serial.println(temp);
    oled.setCursor(0, 15);
    oled.print("TEMP: ");
    oled.println(tempValue);
    

    // Read and display GSR sensor value
    
    //Serial.print("GSR Value: ");
    //Serial.println(gsrValue);
    oled.setCursor(0, 35);
    oled.print("GSR: ");
    oled.println(gsrValue/20);

    oled.setCursor(0, 45);
    oled.println("Current State- ");
    
    oled.setCursor(0, 55);
    oled.println(stressState);
    

    oled.display();
    tsLastReport = millis();
    delay(5000);

  
  }
}
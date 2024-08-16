#include <Fuzzy.h> // Ensure you have a fuzzy logic library compatible with Arduino

// Create fuzzy objects
Fuzzy* fuzzy = new Fuzzy();

void setup() {
    Serial.begin(9600);

    // Define fuzzy input variables
    FuzzySet* gsrLow = new FuzzySet(0, 0, 1.7, 2.0);
    FuzzySet* gsrMedium = new FuzzySet(1.7, 2.0, 4.0, 4.3);
    FuzzySet* gsrHigh = new FuzzySet(4.0, 4.3, 6.0, 6.3);
    FuzzySet* gsrVeryHigh = new FuzzySet(6.0, 6.3, 10.0, 10.0);
    
    FuzzyInput* GSR = new FuzzyInput(1);
    GSR->addFuzzySet(gsrLow);
    GSR->addFuzzySet(gsrMedium);
    GSR->addFuzzySet(gsrHigh);
    GSR->addFuzzySet(gsrVeryHigh);
    fuzzy->addFuzzyInput(GSR);

    FuzzySet* tempLow = new FuzzySet(30.0, 30.0, 32.0, 33.0);
    FuzzySet* tempMedium = new FuzzySet(32.0, 33.0, 34.5, 35.5);
    FuzzySet* tempHigh = new FuzzySet(35.0, 35.5, 36.5, 37.0);
    FuzzySet* tempVeryHigh = new FuzzySet(36.5, 37.5, 40.0, 40.0);
    
    FuzzyInput* body_temp = new FuzzyInput(2);
    body_temp->addFuzzySet(tempLow);
    body_temp->addFuzzySet(tempMedium);
    body_temp->addFuzzySet(tempHigh);
    body_temp->addFuzzySet(tempVeryHigh);
    fuzzy->addFuzzyInput(body_temp);

    FuzzySet* hrLow = new FuzzySet(50.0, 50.0, 65.0, 70.0);
    FuzzySet* hrMedium = new FuzzySet(65.0, 70.0, 85.0, 90.0);
    FuzzySet* hrHigh = new FuzzySet(85.0, 90.0, 95.0, 100.0);
    FuzzySet* hrVeryHigh = new FuzzySet(95.0, 100.0, 150.0, 150.0);
    
    FuzzyInput* heart_rate = new FuzzyInput(3);
    heart_rate->addFuzzySet(hrLow);
    heart_rate->addFuzzySet(hrMedium);
    heart_rate->addFuzzySet(hrHigh);
    heart_rate->addFuzzySet(hrVeryHigh);
    fuzzy->addFuzzyInput(heart_rate);

    // Define fuzzy output variable
    FuzzySet* relax = new FuzzySet(0.0, 0.0, 20.0, 25.0);
    FuzzySet* calm = new FuzzySet(22.5, 25.0, 45.0, 52.5);
    FuzzySet* anxious = new FuzzySet(47.5, 50.0, 75.0, 77.5);
    FuzzySet* stress = new FuzzySet(72.5, 75.0, 100.0, 100.0);
    
    FuzzyOutput* stress_level = new FuzzyOutput(1);
    stress_level->addFuzzySet(relax);
    stress_level->addFuzzySet(calm);
    stress_level->addFuzzySet(anxious);
    stress_level->addFuzzySet(stress);
    fuzzy->addFuzzyOutput(stress_level);

    // Define fuzzy rules
    FuzzyRuleAntecedent* ifGsrLowAndTempLowAndHrLow = new FuzzyRuleAntecedent();
    ifGsrLowAndTempLowAndHrLow->joinWithAND(gsrLow, tempLow);
    ifGsrLowAndTempLowAndHrLow->joinWithAND(ifGsrLowAndTempLowAndHrLow, hrLow);

    FuzzyRuleConsequent* thenRelax = new FuzzyRuleConsequent();
    thenRelax->addOutput(relax);

    FuzzyRule* fuzzyRule1 = new FuzzyRule(1, ifGsrLowAndTempLowAndHrLow, thenRelax);
    fuzzy->addFuzzyRule(fuzzyRule1);

    // You can add more fuzzy rules as necessary
}

void loop() {
    // Set input values (replace these with actual sensor readings)
    fuzzy->setInput(1, 2.14); // GSR
    fuzzy->setInput(2, 38.21); // Body temperature
    fuzzy->setInput(3, 79.23); // Heart rate

    // Execute fuzzy logic
    fuzzy->fuzzify();

    // Get the output value for stress level
    float stressLevel = fuzzy->defuzzify(1);

    // Print the predicted stress level
    Serial.print("Predicted Stress Level: ");
    Serial.println(stressLevel);

    delay(1000); // Wait a second before the next loop
}

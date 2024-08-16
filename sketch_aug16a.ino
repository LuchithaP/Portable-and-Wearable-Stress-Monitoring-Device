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
    //Rule 1: IF GSR is low AND body_temp is low AND heart_rate is low THEN stress_level is relax
    FuzzyRuleAntecedent* ifGsrLowAndTempLowAndHrLow = new FuzzyRuleAntecedent();
    ifGsrLowAndTempLowAndHrLow->joinWithAND(gsrLow, tempLow);
    ifGsrLowAndTempLowAndHrLow->joinWithAND(ifGsrLowAndTempLowAndHrLow, hrLow);

    FuzzyRuleConsequent* thenRelax = new FuzzyRuleConsequent();
    thenRelax->addOutput(relax);

    FuzzyRule* fuzzyRule1 = new FuzzyRule(1, ifGsrLowAndTempLowAndHrLow, thenRelax);
    fuzzy->addFuzzyRule(fuzzyRule1);

    //Rule 2: IF GSR is low AND body_temp is low AND heart_rate is Medium THEN stress_level is anxious
    FuzzyRuleAntecedent* ifGsrLowAndTempLowAndHrMid = new FuzzyRuleAntecedent();
    ifGsrLowAndTempLowAndHrMid->joinWithAND(gsrLow, tempLow);
    ifGsrLowAndTempLowAndHrMid->joinWithAND(ifGsrLowAndTempLowAndHrMid, hrMedium);

    FuzzyRuleConsequent* thenAnxious = new FuzzyRuleConsequent();
    thenAnxious->addOutput(anxious);

    FuzzyRule* fuzzyRule2 = new FuzzyRule(2,  ifGsrLowAndTempLowAndHrMid, thenAnxious);
    fuzzy->addFuzzyRule(fuzzyRule2);

    //Rule 3: IF GSR is low AND body_temp is low AND heart_rate is high THEN stress_level is anxious
    FuzzyRuleAntecedent* ifGsrLowAndTempLowAndHrHigh = new FuzzyRuleAntecedent();
    ifGsrLowAndTempLowAndHrHigh->joinWithAND(gsrLow, tempLow);
    ifGsrLowAndTempLowAndHrHigh->joinWithAND(ifGsrLowAndTempLowAndHrHigh, hrHigh);

    FuzzyRuleConsequent* thenAnxious = new FuzzyRuleConsequent();
    thenAnxious->addOutput(anxious);

    FuzzyRule* fuzzyRule3 = new FuzzyRule(3,  ifGsrLowAndTempLowAndHrHigh, thenAnxious);
    fuzzy->addFuzzyRule(fuzzyRule3);

    //Rule 4: IF GSR is low AND body_temp is low AND heart_rate is very_high THEN stress_level is anxious
    FuzzyRuleAntecedent* ifGsrLowAndTempLowAndHrVeryHigh = new FuzzyRuleAntecedent();
    ifGsrLowAndTempLowAndHrVeryHigh->joinWithAND(gsrLow, tempLow);
    ifGsrLowAndTempLowAndHrVeryHigh->joinWithAND(ifGsrLowAndTempLowAndHrVeryHigh, hrVeryHigh);

    FuzzyRuleConsequent* thenAnxious = new FuzzyRuleConsequent();
    thenAnxious->addOutput(anxious);

    FuzzyRule* fuzzyRule4 = new FuzzyRule(4,  ifGsrLowAndTempLowAndHrVeryHigh, thenAnxious);
    fuzzy->addFuzzyRule(fuzzyRule4);

    //Rule 5: IF GSR is low AND body_temp is medium AND heart_rate is low THEN stress_level is relax

    FuzzyRuleAntecedent* ifGsrLowAndTempMidAndHrLow = new FuzzyRuleAntecedent();
    ifGsrLowAndTempMidAndHrLow->joinWithAND(gsrLow, tempMedium);
    ifGsrLowAndTempMidAndHrLow->joinWithAND(ifGsrLowAndTempMidAndHrLow, hrLow);

    FuzzyRuleConsequent* thenAnxious = new FuzzyRuleConsequent();
    thenRelax->addOutput(relax);

    FuzzyRule* fuzzyRule5 = new FuzzyRule(5,  ifGsrLowAndTempMidAndHrLow, thenRelax);
    fuzzy->addFuzzyRule(fuzzyRule5);

    //Rule 6: IF GSR is low AND body_temp is medium AND heart_rate is medium THEN stress_level is calm
    FuzzyRuleAntecedent* ifGsrLowAndTempMidAndHrMid = new FuzzyRuleAntecedent();
    ifGsrLowAndTempMidAndHrMid->joinWithAND(gsrLow, tempMedium);
    ifGsrLowAndTempMidAndHrMid->joinWithAND(ifGsrLowAndTempMidAndHrMid, hrMedium);

    FuzzyRuleConsequent* thenCalm = new FuzzyRuleConsequent();
    thenCalm->addOutput(calm);

    FuzzyRule* fuzzyRule6 = new FuzzyRule(6,  ifGsrLowAndTempLowAndHrVeryHigh, thenCalm);
    fuzzy->addFuzzyRule(fuzzyRule6);

    // Rule 7: IF GSR is low AND body_temp is medium AND heart_rate is lhigh THEN stress_level is anxious
    FuzzyRuleAntecedent* ifGsrLowAndTempMidAndHrHigh = new FuzzyRuleAntecedent();
    ifGsrLowAndTempMidAndHrHigh->joinWithAND(gsrLow, tempMedium);
    ifGsrLowAndTempMidAndHrHigh->joinWithAND(ifGsrLowAndTempMidAndHrHigh, hrHigh);

    FuzzyRuleConsequent* thenAnxious = new FuzzyRuleConsequent();
    thenAnxious->addOutput(anxious);

    FuzzyRule* fuzzyRule7 = new FuzzyRule(7,  ifGsrLowAndTempMidAndHrHigh, thenAnxious);
    fuzzy->addFuzzyRule(fuzzyRule7);

    //Rule 8: IF GSR is low AND body_temp is medium AND heart_rate is very_high THEN stress_level is anxious
    FuzzyRuleAntecedent* ifGsrLowAndTempMidAndHrVeryHigh = new FuzzyRuleAntecedent();
    ifGsrLowAndTempMidAndHrVeryHigh->joinWithAND(gsrLow, tempMedium);
    ifGsrLowAndTempMidAndHrVeryHigh->joinWithAND(ifGsrLowAndTempMidAndHrVeryHigh, hrVeryHigh);

    FuzzyRuleConsequent* thenAnxious = new FuzzyRuleConsequent();
    thenAnxious->addOutput(anxious);

    FuzzyRule* fuzzyRule8 = new FuzzyRule(8,  ifGsrLowAndTempLowAndHrVeryHigh, thenAnxious);
    fuzzy->addFuzzyRule(fuzzyRule8);

    // Rule 9: IF GSR is low AND body_temp is high AND heart_rate is low THEN stress_level is relax
    FuzzyRuleAntecedent* ifGsrLowAndTempHighAndHrLow = new FuzzyRuleAntecedent();
    ifGsrLowAndTempHighAndHrLow->joinWithAND(gsrLow, tempHigh);
    ifGsrLowAndTempHighAndHrLow->joinWithAND(ifGsrLowAndTempHighAndHrLow, hrLow);
    
    FuzzyRuleConsequent* thenRelax = new FuzzyRuleConsequent();
    thenRelax->addOutput(relax);
    
    FuzzyRule* fuzzyRule9 = new FuzzyRule(9, ifGsrLowAndTempHighAndHrLow, thenRelax);
    fuzzy->addFuzzyRule(fuzzyRule9);
    
    // Rule 10: IF GSR is low AND body_temp is high AND heart_rate is medium THEN stress_level is calm
    FuzzyRuleAntecedent* ifGsrLowAndTempHighAndHrMedium = new FuzzyRuleAntecedent();
    ifGsrLowAndTempHighAndHrMedium->joinWithAND(gsrLow, tempHigh);
    ifGsrLowAndTempHighAndHrMedium->joinWithAND(ifGsrLowAndTempHighAndHrMedium, hrMedium);
    
    FuzzyRuleConsequent* thenCalm = new FuzzyRuleConsequent();
    thenCalm->addOutput(calm);
    
    FuzzyRule* fuzzyRule10 = new FuzzyRule(10, ifGsrLowAndTempHighAndHrMedium, thenCalm);
    fuzzy->addFuzzyRule(fuzzyRule10);
    
    // Rule 11: IF GSR is low AND body_temp is high AND heart_rate is high THEN stress_level is calm
    FuzzyRuleAntecedent* ifGsrLowAndTempHighAndHrHigh = new FuzzyRuleAntecedent();
    ifGsrLowAndTempHighAndHrHigh->joinWithAND(gsrLow, tempHigh);
    ifGsrLowAndTempHighAndHrHigh->joinWithAND(ifGsrLowAndTempHighAndHrHigh, hrHigh);
    
    FuzzyRule* fuzzyRule11 = new FuzzyRule(11, ifGsrLowAndTempHighAndHrHigh, thenCalm);
    fuzzy->addFuzzyRule(fuzzyRule11);
    
    // Rule 12: IF GSR is low AND body_temp is high AND heart_rate is very_high THEN stress_level is anxious
    FuzzyRuleAntecedent* ifGsrLowAndTempHighAndHrVeryHigh = new FuzzyRuleAntecedent();
    ifGsrLowAndTempHighAndHrVeryHigh->joinWithAND(gsrLow, tempHigh);
    ifGsrLowAndTempHighAndHrVeryHigh->joinWithAND(ifGsrLowAndTempHighAndHrVeryHigh, hrVeryHigh);
    
    FuzzyRuleConsequent* thenAnxious = new FuzzyRuleConsequent();
    thenAnxious->addOutput(anxious);
    
    FuzzyRule* fuzzyRule12 = new FuzzyRule(12, ifGsrLowAndTempHighAndHrVeryHigh, thenAnxious);
    fuzzy->addFuzzyRule(fuzzyRule12);
    
    // Rule 13: IF GSR is low AND body_temp is very_high AND heart_rate is low THEN stress_level is relax
    FuzzyRuleAntecedent* ifGsrLowAndTempVeryHighAndHrLow = new FuzzyRuleAntecedent();
    ifGsrLowAndTempVeryHighAndHrLow->joinWithAND(gsrLow, tempVeryHigh);
    ifGsrLowAndTempVeryHighAndHrLow->joinWithAND(ifGsrLowAndTempVeryHighAndHrLow, hrLow);
    
    FuzzyRule* fuzzyRule13 = new FuzzyRule(13, ifGsrLowAndTempVeryHighAndHrLow, thenRelax);
    fuzzy->addFuzzyRule(fuzzyRule13);
    
    // Rule 14: IF GSR is low AND body_temp is very_high AND heart_rate is medium THEN stress_level is relax
    FuzzyRuleAntecedent* ifGsrLowAndTempVeryHighAndHrMedium = new FuzzyRuleAntecedent();
    ifGsrLowAndTempVeryHighAndHrMedium->joinWithAND(gsrLow, tempVeryHigh);
    ifGsrLowAndTempVeryHighAndHrMedium->joinWithAND(ifGsrLowAndTempVeryHighAndHrMedium, hrMedium);
    
    FuzzyRule* fuzzyRule14 = new FuzzyRule(14, ifGsrLowAndTempVeryHighAndHrMedium, thenRelax);
    fuzzy->addFuzzyRule(fuzzyRule14);
    
    // Rule 15: IF GSR is low AND body_temp is very_high AND heart_rate is high THEN stress_level is calm
    FuzzyRuleAntecedent* ifGsrLowAndTempVeryHighAndHrHigh = new FuzzyRuleAntecedent();
    ifGsrLowAndTempVeryHighAndHrHigh->joinWithAND(gsrLow, tempVeryHigh);
    ifGsrLowAndTempVeryHighAndHrHigh->joinWithAND(ifGsrLowAndTempVeryHighAndHrHigh, hrHigh);
    
    FuzzyRule* fuzzyRule15 = new FuzzyRule(15, ifGsrLowAndTempVeryHighAndHrHigh, thenCalm);
    fuzzy->addFuzzyRule(fuzzyRule15);
    
    // Rule 16: IF GSR is low AND body_temp is very_high AND heart_rate is very_high THEN stress_level is anxious
    FuzzyRuleAntecedent* ifGsrLowAndTempVeryHighAndHrVeryHigh = new FuzzyRuleAntecedent();
    ifGsrLowAndTempVeryHighAndHrVeryHigh->joinWithAND(gsrLow, tempVeryHigh);
    ifGsrLowAndTempVeryHighAndHrVeryHigh->joinWithAND(ifGsrLowAndTempVeryHighAndHrVeryHigh, hrVeryHigh);
    
    FuzzyRule* fuzzyRule16 = new FuzzyRule(16, ifGsrLowAndTempVeryHighAndHrVeryHigh, thenAnxious);
    fuzzy->addFuzzyRule(fuzzyRule16);

    // Rule 17: IF GSR is medium AND body_temp is low AND heart_rate is low THEN stress_level is calm
    FuzzyRuleAntecedent* ifGsrMediumAndTempLowAndHrLow = new FuzzyRuleAntecedent();
    ifGsrMediumAndTempLowAndHrLow->joinWithAND(gsrMedium, tempLow);
    ifGsrMediumAndTempLowAndHrLow->joinWithAND(ifGsrMediumAndTempLowAndHrLow, hrLow);
    
    FuzzyRule* fuzzyRule17 = new FuzzyRule(17, ifGsrMediumAndTempLowAndHrLow, thenCalm);
    fuzzy->addFuzzyRule(fuzzyRule17);
    
    // Rule 18: IF GSR is medium AND body_temp is low AND heart_rate is medium THEN stress_level is calm
    FuzzyRuleAntecedent* ifGsrMediumAndTempLowAndHrMedium = new FuzzyRuleAntecedent();
    ifGsrMediumAndTempLowAndHrMedium->joinWithAND(gsrMedium, tempLow);
    ifGsrMediumAndTempLowAndHrMedium->joinWithAND(ifGsrMediumAndTempLowAndHrMedium, hrMedium);
    
    FuzzyRule* fuzzyRule18 = new FuzzyRule(18, ifGsrMediumAndTempLowAndHrMedium, thenCalm);
    fuzzy->addFuzzyRule(fuzzyRule18);
    
    // Rule 19: IF GSR is medium AND body_temp is low AND heart_rate is high THEN stress_level is calm
    FuzzyRuleAntecedent* ifGsrMediumAndTempLowAndHrHigh = new FuzzyRuleAntecedent();
    ifGsrMediumAndTempLowAndHrHigh->joinWithAND(gsrMedium, tempLow);
    ifGsrMediumAndTempLowAndHrHigh->joinWithAND(ifGsrMediumAndTempLowAndHrHigh, hrHigh);
    
    FuzzyRule* fuzzyRule19 = new FuzzyRule(19, ifGsrMediumAndTempLowAndHrHigh, thenCalm);
    fuzzy->addFuzzyRule(fuzzyRule19);
    
    // Rule 20: IF GSR is medium AND body_temp is low AND heart_rate is very_high THEN stress_level is anxious
    FuzzyRuleAntecedent* ifGsrMediumAndTempLowAndHrVeryHigh = new FuzzyRuleAntecedent();
    ifGsrMediumAndTempLowAndHrVeryHigh->joinWithAND(gsrMedium, tempLow);
    ifGsrMediumAndTempLowAndHrVeryHigh->joinWithAND(ifGsrMediumAndTempLowAndHrVeryHigh, hrVeryHigh);
    
    FuzzyRule* fuzzyRule20 = new FuzzyRule(20, ifGsrMediumAndTempLowAndHrVeryHigh, thenAnxious);
    fuzzy->addFuzzyRule(fuzzyRule20);
    
    // Rule 21: IF GSR is medium AND body_temp is medium AND heart_rate is low THEN stress_level is calm
    FuzzyRuleAntecedent* ifGsrMediumAndTempMediumAndHrLow = new FuzzyRuleAntecedent();
    ifGsrMediumAndTempMediumAndHrLow->joinWithAND(gsrMedium, tempMedium);
    ifGsrMediumAndTempMediumAndHrLow->joinWithAND(ifGsrMediumAndTempMediumAndHrLow, hrLow);
    
    FuzzyRule* fuzzyRule21 = new FuzzyRule(21, ifGsrMediumAndTempMediumAndHrLow, thenCalm);
    fuzzy->addFuzzyRule(fuzzyRule21);
    
    // Rule 22: IF GSR is medium AND body_temp is medium AND heart_rate is medium THEN stress_level is calm
    FuzzyRuleAntecedent* ifGsrMediumAndTempMediumAndHrMedium = new FuzzyRuleAntecedent();
    ifGsrMediumAndTempMediumAndHrMedium->joinWithAND(gsrMedium, tempMedium);
    ifGsrMediumAndTempMediumAndHrMedium->joinWithAND(ifGsrMediumAndTempMediumAndHrMedium, hrMedium);
    
    FuzzyRule* fuzzyRule22 = new FuzzyRule(22, ifGsrMediumAndTempMediumAndHrMedium, thenCalm);
    fuzzy->addFuzzyRule(fuzzyRule22);
    
    // Rule 23: IF GSR is medium AND body_temp is medium AND heart_rate is high THEN stress_level is calm
    FuzzyRuleAntecedent* ifGsrMediumAndTempMediumAndHrHigh = new FuzzyRuleAntecedent();
    ifGsrMediumAndTempMediumAndHrHigh->joinWithAND(gsrMedium, tempMedium);
    ifGsrMediumAndTempMediumAndHrHigh->joinWithAND(ifGsrMediumAndTempMediumAndHrHigh, hrHigh);
    
    FuzzyRule* fuzzyRule23 = new FuzzyRule(23, ifGsrMediumAndTempMediumAndHrHigh, thenCalm);
    fuzzy->addFuzzyRule(fuzzyRule23);
    
    // Rule 24: IF GSR is medium AND body_temp is medium AND heart_rate is very_high THEN stress_level is anxious
    FuzzyRuleAntecedent* ifGsrMediumAndTempMediumAndHrVeryHigh = new FuzzyRuleAntecedent();
    ifGsrMediumAndTempMediumAndHrVeryHigh->joinWithAND(gsrMedium, tempMedium);
    ifGsrMediumAndTempMediumAndHrVeryHigh->joinWithAND(ifGsrMediumAndTempMediumAndHrVeryHigh, hrVeryHigh);
    
    FuzzyRule* fuzzyRule24 = new FuzzyRule(24, ifGsrMediumAndTempMediumAndHrVeryHigh, thenAnxious);
    fuzzy->addFuzzyRule(fuzzyRule24);
    
    // Rule 25: IF GSR is medium AND body_temp is high AND heart_rate is low THEN stress_level is calm
    FuzzyRuleAntecedent* ifGsrMediumAndTempHighAndHrLow = new FuzzyRuleAntecedent();
    ifGsrMediumAndTempHighAndHrLow->joinWithAND(gsrMedium, tempHigh);
    ifGsrMediumAndTempHighAndHrLow->joinWithAND(ifGsrMediumAndTempHighAndHrLow, hrLow);
    
    FuzzyRule* fuzzyRule25 = new FuzzyRule(25, ifGsrMediumAndTempHighAndHrLow, thenCalm);
    fuzzy->addFuzzyRule(fuzzyRule25);
    
    // Rule 26: IF GSR is medium AND body_temp is high AND heart_rate is medium THEN stress_level is anxious
    FuzzyRuleAntecedent* ifGsrMediumAndTempHighAndHrMedium = new FuzzyRuleAntecedent();
    ifGsrMediumAndTempHighAndHrMedium->joinWithAND(gsrMedium, tempHigh);
    ifGsrMediumAndTempHighAndHrMedium->joinWithAND(ifGsrMediumAndTempHighAndHrMedium, hrMedium);
    
    FuzzyRule* fuzzyRule26 = new FuzzyRule(26, ifGsrMediumAndTempHighAndHrMedium, thenAnxious);
    fuzzy->addFuzzyRule(fuzzyRule26);
    
    // Rule 27: IF GSR is medium AND body_temp is high AND heart_rate is high THEN stress_level is anxious
    FuzzyRuleAntecedent* ifGsrMediumAndTempHighAndHrHigh = new FuzzyRuleAntecedent();
    ifGsrMediumAndTempHighAndHrHigh->joinWithAND(gsrMedium, tempHigh);
    ifGsrMediumAndTempHighAndHrHigh->joinWithAND(ifGsrMediumAndTempHighAndHrHigh, hrHigh);
    
    FuzzyRule* fuzzyRule27 = new FuzzyRule(27, ifGsrMediumAndTempHighAndHrHigh, thenAnxious);
    fuzzy->addFuzzyRule(fuzzyRule27);
    
    // Rule 28: IF GSR is medium AND body_temp is high AND heart_rate is very_high THEN stress_level is stressed
    FuzzyRuleAntecedent* ifGsrMediumAndTempHighAndHrVeryHigh = new FuzzyRuleAntecedent();
    ifGsrMediumAndTempHighAndHrVeryHigh->joinWithAND(gsrMedium, tempHigh);
    ifGsrMediumAndTempHighAndHrVeryHigh->joinWithAND(ifGsrMediumAndTempHighAndHrVeryHigh, hrVeryHigh);
    
    FuzzyRuleConsequent* thenStressed = new FuzzyRuleConsequent();
    thenStressed->addOutput(stressed);
    
    FuzzyRule* fuzzyRule28 = new FuzzyRule(28, ifGsrMediumAndTempHighAndHrVeryHigh, thenStressed);
    fuzzy->addFuzzyRule(fuzzyRule28);
    
    // Rule 29: IF GSR is medium AND body_temp is very_high AND heart_rate is low THEN stress_level is calm
    FuzzyRuleAntecedent* ifGsrMediumAndTempVeryHighAndHrLow = new FuzzyRuleAntecedent();
    ifGsrMediumAndTempVeryHighAndHrLow->joinWithAND(gsrMedium, tempVeryHigh);
    ifGsrMediumAndTempVeryHighAndHrLow->joinWithAND(ifGsrMediumAndTempVeryHighAndHrLow, hrLow);
    
    FuzzyRule* fuzzyRule29 = new FuzzyRule(29, ifGsrMediumAndTempVeryHighAndHrLow, thenCalm);
    fuzzy->addFuzzyRule(fuzzyRule29);
    
    // Rule 30: IF GSR is medium AND body_temp is very_high AND heart_rate is medium THEN stress_level is anxious
    FuzzyRuleAntecedent* ifGsrMediumAndTempVeryHighAndHrMedium = new FuzzyRuleAntecedent();
    ifGsrMediumAndTempVeryHighAndHrMedium->joinWithAND(gsrMedium, tempVeryHigh);
    ifGsrMediumAndTempVeryHighAndHrMedium->joinWithAND(ifGsrMediumAndTempVeryHighAndHrMedium, hrMedium);
    
    FuzzyRule* fuzzyRule30 = new FuzzyRule(30, ifGsrMediumAndTempVeryHighAndHrMedium, thenAnxious);
    fuzzy->addFuzzyRule(fuzzyRule30);
    
    // Rule 31: IF GSR is medium AND body_temp is very_high AND heart_rate is high THEN stress_level is anxious
    FuzzyRuleAntecedent* ifGsrMediumAndTempVeryHighAndHrHigh = new FuzzyRuleAntecedent();
    ifGsrMediumAndTempVeryHighAndHrHigh->joinWithAND(gsrMedium, tempVeryHigh);
    ifGsrMediumAndTempVeryHighAndHrHigh->joinWithAND(ifGsrMediumAndTempVeryHighAndHrHigh, hrHigh);
    
    FuzzyRule* fuzzyRule31 = new FuzzyRule(31, ifGsrMediumAndTempVeryHighAndHrHigh, thenAnxious);
    fuzzy->addFuzzyRule(fuzzyRule31);
    
    // Rule 32: IF GSR is medium AND body_temp is very_high AND heart_rate is very_high THEN stress_level is stressed
    FuzzyRuleAntecedent* ifGsrMediumAndTempVeryHighAndHrVeryHigh = new FuzzyRuleAntecedent();
    ifGsrMediumAndTempVeryHighAndHrVeryHigh->joinWithAND(gsrMedium, tempVeryHigh);
    ifGsrMediumAndTempVeryHighAndHrVeryHigh->joinWithAND(ifGsrMediumAndTempVeryHighAndHrVeryHigh, hrVeryHigh);
    
    FuzzyRule* fuzzyRule32 = new FuzzyRule(32, ifGsrMediumAndTempVeryHighAndHrVeryHigh, thenStressed);
    fuzzy->addFuzzyRule(fuzzyRule32);
    
    // Rule 33: IF GSR is high AND body_temp is low AND heart_rate is low THEN stress_level is calm
    FuzzyRuleAntecedent* ifGsrHighAndTempLowAndHrLow = new FuzzyRuleAntecedent();
    ifGsrHighAndTempLowAndHrLow->joinWithAND(gsrHigh, tempLow);
    ifGsrHighAndTempLowAndHrLow->joinWithAND(ifGsrHighAndTempLowAndHrLow, hrLow);
    
    FuzzyRule* fuzzyRule33 = new FuzzyRule(33, ifGsrHighAndTempLowAndHrLow, thenCalm);
    fuzzy->addFuzzyRule(fuzzyRule33);
    
    // Rule 34: IF GSR is high AND body_temp is low AND heart_rate is medium THEN stress_level is calm
    FuzzyRuleAntecedent* ifGsrHighAndTempLowAndHrMedium = new FuzzyRuleAntecedent();
    ifGsrHighAndTempLowAndHrMedium->joinWithAND(gsrHigh, tempLow);
    ifGsrHighAndTempLowAndHrMedium->joinWithAND(ifGsrHighAndTempLowAndHrMedium, hrMedium);
    
    FuzzyRule* fuzzyRule34 = new FuzzyRule(34, ifGsrHighAndTempLowAndHrMedium, thenCalm);
    fuzzy->addFuzzyRule(fuzzyRule34);
    
    // Rule 35: IF GSR is high AND body_temp is low AND heart_rate is high THEN stress_level is anxious
    FuzzyRuleAntecedent* ifGsrHighAndTempLowAndHrHigh = new FuzzyRuleAntecedent();
    ifGsrHighAndTempLowAndHrHigh->joinWithAND(gsrHigh, tempLow);
    ifGsrHighAndTempLowAndHrHigh->joinWithAND(ifGsrHighAndTempLowAndHrHigh, hrHigh);
    
    FuzzyRule* fuzzyRule35 = new FuzzyRule(35, ifGsrHighAndTempLowAndHrHigh, thenAnxious);
    fuzzy->addFuzzyRule(fuzzyRule35);
    
    // Rule 36: IF GSR is high AND body_temp is low AND heart_rate is very_high THEN stress_level is anxious
    FuzzyRuleAntecedent* ifGsrHighAndTempLowAndHrVeryHigh = new FuzzyRuleAntecedent();
    ifGsrHighAndTempLowAndHrVeryHigh->joinWithAND(gsrHigh, tempLow);
    ifGsrHighAndTempLowAndHrVeryHigh->joinWithAND(ifGsrHighAndTempLowAndHrVeryHigh, hrVeryHigh);
    
    FuzzyRule* fuzzyRule36 = new FuzzyRule(36, ifGsrHighAndTempLowAndHrVeryHigh, thenAnxious);
    fuzzy->addFuzzyRule(fuzzyRule36);
    
    // Rule 37: IF GSR is high AND body_temp is medium AND heart_rate is low THEN stress_level is calm
    FuzzyRuleAntecedent* ifGsrHighAndTempMediumAndHrLow = new FuzzyRuleAntecedent();
    ifGsrHighAndTempMediumAndHrLow->joinWithAND(gsrHigh, tempMedium);
    ifGsrHighAndTempMediumAndHrLow->joinWithAND(ifGsrHighAndTempMediumAndHrLow, hrLow);
    
    FuzzyRule* fuzzyRule37 = new FuzzyRule(37, ifGsrHighAndTempMediumAndHrLow, thenCalm);
    fuzzy->addFuzzyRule(fuzzyRule37);
    
    // Rule 38: IF GSR is high AND body_temp is medium AND heart_rate is medium THEN stress_level is anxious
    FuzzyRuleAntecedent* ifGsrHighAndTempMediumAndHrMedium = new FuzzyRuleAntecedent();
    ifGsrHighAndTempMediumAndHrMedium->joinWithAND(gsrHigh, tempMedium);
    ifGsrHighAndTempMediumAndHrMedium->joinWithAND(ifGsrHighAndTempMediumAndHrMedium, hrMedium);
    
    FuzzyRule* fuzzyRule38 = new FuzzyRule(38, ifGsrHighAndTempMediumAndHrMedium, thenAnxious);
    fuzzy->addFuzzyRule(fuzzyRule38);
    
    // Rule 39: IF GSR is high AND body_temp is medium AND heart_rate is high THEN stress_level is anxious
    FuzzyRuleAntecedent* ifGsrHighAndTempMediumAndHrHigh = new FuzzyRuleAntecedent();
    ifGsrHighAndTempMediumAndHrHigh->joinWithAND(gsrHigh, tempMedium);
    ifGsrHighAndTempMediumAndHrHigh->joinWithAND(ifGsrHighAndTempMediumAndHrHigh, hrHigh);
    
    FuzzyRule* fuzzyRule39 = new FuzzyRule(39, ifGsrHighAndTempMediumAndHrHigh, thenAnxious);
    fuzzy->addFuzzyRule(fuzzyRule39);
    
    // Rule 40: IF GSR is high AND body_temp is medium AND heart_rate is very_high THEN stress_level is stressed
    FuzzyRuleAntecedent* ifGsrHighAndTempMediumAndHrVeryHigh = new FuzzyRuleAntecedent();
    ifGsrHighAndTempMediumAndHrVeryHigh->joinWithAND(gsrHigh, tempMedium);
    ifGsrHighAndTempMediumAndHrVeryHigh->joinWithAND(ifGsrHighAndTempMediumAndHrVeryHigh, hrVeryHigh);
    
    FuzzyRule* fuzzyRule40 = new FuzzyRule(40, ifGsrHighAndTempMediumAndHrVeryHigh, thenStressed);
    fuzzy->addFuzzyRule(fuzzyRule40);
    
    // Rule 41: IF GSR is high AND body_temp is high AND heart_rate is low THEN stress_level is anxious
    FuzzyRuleAntecedent* ifGsrHighAndTempHighAndHrLow = new FuzzyRuleAntecedent();
    ifGsrHighAndTempHighAndHrLow->joinWithAND(gsrHigh, tempHigh);
    ifGsrHighAndTempHighAndHrLow->joinWithAND(ifGsrHighAndTempHighAndHrLow, hrLow);
    
    FuzzyRule* fuzzyRule41 = new FuzzyRule(41, ifGsrHighAndTempHighAndHrLow, thenAnxious);
    fuzzy->addFuzzyRule(fuzzyRule41);
    
    // Rule 42: IF GSR is high AND body_temp is high AND heart_rate is medium THEN stress_level is anxious
    FuzzyRuleAntecedent* ifGsrHighAndTempHighAndHrMedium = new FuzzyRuleAntecedent();
    ifGsrHighAndTempHighAndHrMedium->joinWithAND(gsrHigh, tempHigh);
    ifGsrHighAndTempHighAndHrMedium->joinWithAND(ifGsrHighAndTempHighAndHrMedium, hrMedium);
    
    FuzzyRule* fuzzyRule42 = new FuzzyRule(42, ifGsrHighAndTempHighAndHrMedium, thenAnxious);
    fuzzy->addFuzzyRule(fuzzyRule42);
    
    // Rule 43: IF GSR is high AND body_temp is high AND heart_rate is high THEN stress_level is stressed
    FuzzyRuleAntecedent* ifGsrHighAndTempHighAndHrHigh = new FuzzyRuleAntecedent();
    ifGsrHighAndTempHighAndHrHigh->joinWithAND(gsrHigh, tempHigh);
    ifGsrHighAndTempHighAndHrHigh->joinWithAND(ifGsrHighAndTempHighAndHrHigh, hrHigh);
    
    FuzzyRule* fuzzyRule43 = new FuzzyRule(43, ifGsrHighAndTempHighAndHrHigh, thenStressed);
    fuzzy->addFuzzyRule(fuzzyRule43);
    
    // Rule 44: IF GSR is high AND body_temp is high AND heart_rate is very_high THEN stress_level is stressed
    FuzzyRuleAntecedent* ifGsrHighAndTempHighAndHrVeryHigh = new FuzzyRuleAntecedent();
    ifGsrHighAndTempHighAndHrVeryHigh->joinWithAND(gsrHigh, tempHigh);
    ifGsrHighAndTempHighAndHrVeryHigh->joinWithAND(ifGsrHighAndTempHighAndHrVeryHigh, hrVeryHigh);
    
    FuzzyRule* fuzzyRule44 = new FuzzyRule(44, ifGsrHighAndTempHighAndHrVeryHigh, thenStressed);
    fuzzy->addFuzzyRule(fuzzyRule44);
    
    // Rule 45: IF GSR is high AND body_temp is very_high AND heart_rate is low THEN stress_level is anxious
    FuzzyRuleAntecedent* ifGsrHighAndTempVeryHighAndHrLow = new FuzzyRuleAntecedent();
    ifGsrHighAndTempVeryHighAndHrLow->joinWithAND(gsrHigh, tempVeryHigh);
    ifGsrHighAndTempVeryHighAndHrLow->joinWithAND(ifGsrHighAndTempVeryHighAndHrLow, hrLow);
    
    FuzzyRule* fuzzyRule45 = new FuzzyRule(45, ifGsrHighAndTempVeryHighAndHrLow, thenAnxious);
    fuzzy->addFuzzyRule(fuzzyRule45);
    
    // Rule 46: IF GSR is high AND body_temp is very_high AND heart_rate is medium THEN stress_level is stressed
    FuzzyRuleAntecedent* ifGsrHighAndTempVeryHighAndHrMedium = new FuzzyRuleAntecedent();
    ifGsrHighAndTempVeryHighAndHrMedium->joinWithAND(gsrHigh, tempVeryHigh);
    ifGsrHighAndTempVeryHighAndHrMedium->joinWithAND(ifGsrHighAndTempVeryHighAndHrMedium, hrMedium);
    
    FuzzyRule* fuzzyRule46 = new FuzzyRule(46, ifGsrHighAndTempVeryHighAndHrMedium, thenStressed);
    fuzzy->addFuzzyRule(fuzzyRule46);

    // Rule 47: IF GSR is high AND body_temp is very_high AND heart_rate is high THEN stress_level is stressed
    FuzzyRuleAntecedent* ifGsrHighAndTempVeryHighAndHrHigh = new FuzzyRuleAntecedent();
    ifGsrHighAndTempVeryHighAndHrHigh->joinWithAND(gsrHigh, tempVeryHigh);
    ifGsrHighAndTempVeryHighAndHrHigh->joinWithAND(ifGsrHighAndTempVeryHighAndHrHigh, hrHigh);
    
    FuzzyRule* fuzzyRule47 = new FuzzyRule(47, ifGsrHighAndTempVeryHighAndHrHigh, thenStressed);
    fuzzy->addFuzzyRule(fuzzyRule47);
    
    // Rule 48: IF GSR is high AND body_temp is very_high AND heart_rate is very_high THEN stress_level is stressed
    FuzzyRuleAntecedent* ifGsrHighAndTempVeryHighAndHrVeryHigh = new FuzzyRuleAntecedent();
    ifGsrHighAndTempVeryHighAndHrVeryHigh->joinWithAND(gsrHigh, tempVeryHigh);
    ifGsrHighAndTempVeryHighAndHrVeryHigh->joinWithAND(ifGsrHighAndTempVeryHighAndHrVeryHigh, hrVeryHigh);
    
    FuzzyRule* fuzzyRule48 = new FuzzyRule(48, ifGsrHighAndTempVeryHighAndHrVeryHigh, thenStressed);
    fuzzy->addFuzzyRule(fuzzyRule48);

    // Rule 49: IF GSR is very_high AND body_temp is medium AND heart_rate is low THEN stress_level is anxious
    FuzzyRuleAntecedent* ifGsrVeryHighAndTempMediumAndHrLow = new FuzzyRuleAntecedent();
    ifGsrVeryHighAndTempMediumAndHrLow->joinWithAND(gsrVeryHigh, tempMedium);
    ifGsrVeryHighAndTempMediumAndHrLow->joinWithAND(ifGsrVeryHighAndTempMediumAndHrLow, hrLow);
    
    FuzzyRule* fuzzyRule49 = new FuzzyRule(49, ifGsrVeryHighAndTempMediumAndHrLow, thenAnxious);
    fuzzy->addFuzzyRule(fuzzyRule49);
    
    // Rule 50: IF GSR is very_high AND body_temp is medium AND heart_rate is medium THEN stress_level is anxious
    FuzzyRuleAntecedent* ifGsrVeryHighAndTempMediumAndHrMedium = new FuzzyRuleAntecedent();
    ifGsrVeryHighAndTempMediumAndHrMedium->joinWithAND(gsrVeryHigh, tempMedium);
    ifGsrVeryHighAndTempMediumAndHrMedium->joinWithAND(ifGsrVeryHighAndTempMediumAndHrMedium, hrMedium);
    
    FuzzyRule* fuzzyRule50 = new FuzzyRule(50, ifGsrVeryHighAndTempMediumAndHrMedium, thenAnxious);
    fuzzy->addFuzzyRule(fuzzyRule50);
    
    // Rule 51: IF GSR is very_high AND body_temp is medium AND heart_rate is high THEN stress_level is anxious
    FuzzyRuleAntecedent* ifGsrVeryHighAndTempMediumAndHrHigh = new FuzzyRuleAntecedent();
    ifGsrVeryHighAndTempMediumAndHrHigh->joinWithAND(gsrVeryHigh, tempMedium);
    ifGsrVeryHighAndTempMediumAndHrHigh->joinWithAND(ifGsrVeryHighAndTempMediumAndHrHigh, hrHigh);
    
    FuzzyRule* fuzzyRule51 = new FuzzyRule(51, ifGsrVeryHighAndTempMediumAndHrHigh, thenAnxious);
    fuzzy->addFuzzyRule(fuzzyRule51);
    
    // Rule 52: IF GSR is very_high AND body_temp is high AND heart_rate is low THEN stress_level is anxious
    FuzzyRuleAntecedent* ifGsrVeryHighAndTempHighAndHrLow = new FuzzyRuleAntecedent();
    ifGsrVeryHighAndTempHighAndHrLow->joinWithAND(gsrVeryHigh, tempHigh);
    ifGsrVeryHighAndTempHighAndHrLow->joinWithAND(ifGsrVeryHighAndTempHighAndHrLow, hrLow);
    
    FuzzyRule* fuzzyRule52 = new FuzzyRule(52, ifGsrVeryHighAndTempHighAndHrLow, thenAnxious);
    fuzzy->addFuzzyRule(fuzzyRule52);
    
    // Rule 53: IF GSR is very_high AND body_temp is high AND heart_rate is medium THEN stress_level is anxious
    FuzzyRuleAntecedent* ifGsrVeryHighAndTempHighAndHrMedium = new FuzzyRuleAntecedent();
    ifGsrVeryHighAndTempHighAndHrMedium->joinWithAND(gsrVeryHigh, tempHigh);
    ifGsrVeryHighAndTempHighAndHrMedium->joinWithAND(ifGsrVeryHighAndTempHighAndHrMedium, hrMedium);
    
    FuzzyRule* fuzzyRule53 = new FuzzyRule(53, ifGsrVeryHighAndTempHighAndHrMedium, thenAnxious);
    fuzzy->addFuzzyRule(fuzzyRule53);
    
    // Rule 54: IF GSR is very_high AND body_temp is high AND heart_rate is high THEN stress_level is anxious
    FuzzyRuleAntecedent* ifGsrVeryHighAndTempHighAndHrHigh = new FuzzyRuleAntecedent();
    ifGsrVeryHighAndTempHighAndHrHigh->joinWithAND(gsrVeryHigh, tempHigh);
    ifGsrVeryHighAndTempHighAndHrHigh->joinWithAND(ifGsrVeryHighAndTempHighAndHrHigh, hrHigh);
    
    FuzzyRule* fuzzyRule54 = new FuzzyRule(54, ifGsrVeryHighAndTempHighAndHrHigh, thenAnxious);
    fuzzy->addFuzzyRule(fuzzyRule54);

    // Rule 55: IF GSR is very high AND body_temp is low AND heart_rate is low THEN stress_level is high
    FuzzyRuleAntecedent* ifGsrVeryHighAndTempLowAndHrLow = new FuzzyRuleAntecedent();
    ifGsrVeryHighAndTempLowAndHrLow->joinWithAND(gsrVeryHigh, tempLow);
    ifGsrVeryHighAndTempLowAndHrLow->joinWithAND(ifGsrVeryHighAndTempLowAndHrLow, hrLow);
    
    FuzzyRule* fuzzyRule55 = new FuzzyRule(55, ifGsrVeryHighAndTempLowAndHrLow, thenHigh);
    fuzzy->addFuzzyRule(fuzzyRule55);
    
    // Rule 56: IF GSR is very high AND body_temp is low AND heart_rate is medium THEN stress_level is high
    FuzzyRuleAntecedent* ifGsrVeryHighAndTempLowAndHrMedium = new FuzzyRuleAntecedent();
    ifGsrVeryHighAndTempLowAndHrMedium->joinWithAND(gsrVeryHigh, tempLow);
    ifGsrVeryHighAndTempLowAndHrMedium->joinWithAND(ifGsrVeryHighAndTempLowAndHrMedium, hrMedium);
    
    FuzzyRule* fuzzyRule56 = new FuzzyRule(56, ifGsrVeryHighAndTempLowAndHrMedium, thenHigh);
    fuzzy->addFuzzyRule(fuzzyRule56);
    
    // Rule 57: IF GSR is very high AND body_temp is low AND heart_rate is high THEN stress_level is high
    FuzzyRuleAntecedent* ifGsrVeryHighAndTempLowAndHrHigh = new FuzzyRuleAntecedent();
    ifGsrVeryHighAndTempLowAndHrHigh->joinWithAND(gsrVeryHigh, tempLow);
    ifGsrVeryHighAndTempLowAndHrHigh->joinWithAND(ifGsrVeryHighAndTempLowAndHrHigh, hrHigh);
    
    FuzzyRule* fuzzyRule57 = new FuzzyRule(57, ifGsrVeryHighAndTempLowAndHrHigh, thenHigh);
    fuzzy->addFuzzyRule(fuzzyRule57);
    
    // Rule 58: IF GSR is very high AND body_temp is medium AND heart_rate is low THEN stress_level is high
    FuzzyRuleAntecedent* ifGsrVeryHighAndTempMediumAndHrLow = new FuzzyRuleAntecedent();
    ifGsrVeryHighAndTempMediumAndHrLow->joinWithAND(gsrVeryHigh, tempMedium);
    ifGsrVeryHighAndTempMediumAndHrLow->joinWithAND(ifGsrVeryHighAndTempMediumAndHrLow, hrLow);
    
    FuzzyRule* fuzzyRule58 = new FuzzyRule(58, ifGsrVeryHighAndTempMediumAndHrLow, thenHigh);
    fuzzy->addFuzzyRule(fuzzyRule58);
    
    // Rule 59: IF GSR is very high AND body_temp is medium AND heart_rate is medium THEN stress_level is high
    FuzzyRuleAntecedent* ifGsrVeryHighAndTempMediumAndHrMedium = new FuzzyRuleAntecedent();
    ifGsrVeryHighAndTempMediumAndHrMedium->joinWithAND(gsrVeryHigh, tempMedium);
    ifGsrVeryHighAndTempMediumAndHrMedium->joinWithAND(ifGsrVeryHighAndTempMediumAndHrMedium, hrMedium);
    
    FuzzyRule* fuzzyRule59 = new FuzzyRule(59, ifGsrVeryHighAndTempMediumAndHrMedium, thenHigh);
    fuzzy->addFuzzyRule(fuzzyRule59);
    
    // Rule 60: IF GSR is very high AND body_temp is medium AND heart_rate is high THEN stress_level is high
    FuzzyRuleAntecedent* ifGsrVeryHighAndTempMediumAndHrHigh = new FuzzyRuleAntecedent();
    ifGsrVeryHighAndTempMediumAndHrHigh->joinWithAND(gsrVeryHigh, tempMedium);
    ifGsrVeryHighAndTempMediumAndHrHigh->joinWithAND(ifGsrVeryHighAndTempMediumAndHrHigh, hrHigh);
    
    FuzzyRule* fuzzyRule60 = new FuzzyRule(60, ifGsrVeryHighAndTempMediumAndHrHigh, thenHigh);
    fuzzy->addFuzzyRule(fuzzyRule60);

    // Rule 61: IF GSR is very high AND body_temp is high AND heart_rate is low THEN stress_level is high
    FuzzyRuleAntecedent* ifGsrVeryHighAndTempHighAndHrLow = new FuzzyRuleAntecedent();
    ifGsrVeryHighAndTempHighAndHrLow->joinWithAND(gsrVeryHigh, tempHigh);
    ifGsrVeryHighAndTempHighAndHrLow->joinWithAND(ifGsrVeryHighAndTempHighAndHrLow, hrLow);
    
    FuzzyRule* fuzzyRule61 = new FuzzyRule(61, ifGsrVeryHighAndTempHighAndHrLow, thenHigh);
    fuzzy->addFuzzyRule(fuzzyRule61);
    
    // Rule 62: IF GSR is very high AND body_temp is high AND heart_rate is medium THEN stress_level is high
    FuzzyRuleAntecedent* ifGsrVeryHighAndTempHighAndHrMedium = new FuzzyRuleAntecedent();
    ifGsrVeryHighAndTempHighAndHrMedium->joinWithAND(gsrVeryHigh, tempHigh);
    ifGsrVeryHighAndTempHighAndHrMedium->joinWithAND(ifGsrVeryHighAndTempHighAndHrMedium, hrMedium);
    
    FuzzyRule* fuzzyRule62 = new FuzzyRule(62, ifGsrVeryHighAndTempHighAndHrMedium, thenHigh);
    fuzzy->addFuzzyRule(fuzzyRule62);
    
    // Rule 63: IF GSR is very high AND body_temp is high AND heart_rate is high THEN stress_level is very high
    FuzzyRuleAntecedent* ifGsrVeryHighAndTempHighAndHrHigh = new FuzzyRuleAntecedent();
    ifGsrVeryHighAndTempHighAndHrHigh->joinWithAND(gsrVeryHigh, tempHigh);
    ifGsrVeryHighAndTempHighAndHrHigh->joinWithAND(ifGsrVeryHighAndTempHighAndHrHigh, hrHigh);
    
    FuzzyRule* fuzzyRule63 = new FuzzyRule(63, ifGsrVeryHighAndTempHighAndHrHigh, thenVeryHigh);
    fuzzy->addFuzzyRule(fuzzyRule63);
    
    // Rule 64: IF GSR is very high AND body_temp is very high AND heart_rate is low THEN stress_level is very high
    FuzzyRuleAntecedent* ifGsrVeryHighAndTempVeryHighAndHrLow = new FuzzyRuleAntecedent();
    ifGsrVeryHighAndTempVeryHighAndHrLow->joinWithAND(gsrVeryHigh, tempVeryHigh);
    ifGsrVeryHighAndTempVeryHighAndHrLow->joinWithAND(ifGsrVeryHighAndTempVeryHighAndHrLow, hrLow);
    
    FuzzyRule* fuzzyRule64 = new FuzzyRule(64, ifGsrVeryHighAndTempVeryHighAndHrLow, thenVeryHigh);
    fuzzy->addFuzzyRule(fuzzyRule64);



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

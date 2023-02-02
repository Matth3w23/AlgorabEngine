#include "unlimited_float.h"

/*
* Example number:
* 123.456 represented by:
* integral: [3,2,1]
* decimal: [6,5,4]
* i.e. least significant digits first
*/

UFloat UFloat::posPosSum(UFloat& pNum1, UFloat& pNum2) {
    UFloat temp;

    int carry, a, b;
    carry = a = b = 0;
    int aSize = pNum1.decimal.size();
    int bSize = pNum2.decimal.size();

    //sum decimal parts
    int maxIts = std::max(aSize, bSize);
    int indexModA = std::min(0, aSize - bSize);
    int indexModB = std::min(0, bSize - aSize);

    int sum;

    bool nonZeroDecimalPushed = false;

    for (int i = 0; i < maxIts; i++) {
        if (i + indexModA < 0) {
            a = 0;
        } else {
            a = pNum1.decimal[i + indexModA];
        }
        if (i + indexModB < 0) {
            b = 0;
        } else {
            b = pNum2.decimal[i + indexModB];
        }

        sum = a + b + carry;

        if (sum >= 10) {
            carry = 1;
            sum -= 10;
        } else {
            carry = 0;
        }

        //Prevent trailing zeroes like 0.12300000
        if (!nonZeroDecimalPushed) {
            if (sum != 0) {
                temp.decimal.push_back(sum);
                nonZeroDecimalPushed = true;
            }
        } else {
            temp.decimal.push_back(sum);
        }
    }

    maxIts = std::max(pNum1.integral.size(), pNum2.integral.size());

    int zeroesToPush = 0;
    
    for (int i = 0; i < maxIts; i++) {
        if (i >= pNum1.integral.size()) {
            a = 0;
        } else {
            a = pNum1.integral[i];
        }
        if (i >= pNum2.integral.size()) {
            b = 0;
        } else {
            b = pNum2.integral[i];
        }

        sum = a + b + carry;

        if (sum >= 10) {
            carry = 1;
            sum -= 10;
        } else {
            carry = 0;
        }

        //push result to integral part while preventing leading zeroes
        if (sum == 0) {
            zeroesToPush++;
        } else {
            if (zeroesToPush) { //i.e. != 0
                for (int j = 0; j < zeroesToPush; j++) {
                    temp.integral.push_back(0);
                }
                zeroesToPush = 0;
            }
            temp.integral.push_back(sum);
        }
        
    }

    if (carry) {
        if (zeroesToPush) { //i.e. != 0
            for (int j = 0; j < zeroesToPush; j++) {
                temp.integral.push_back(0);
            }
            zeroesToPush = 0;
        }
        temp.integral.push_back(1);
    }

    return temp;
}

UFloat UFloat::posNegSum(UFloat& pNum, UFloat& nNum) { //|pos| must be greater than |neg|
    UFloat temp;

    int carry, a, b;
    carry = a = b = 0;
    int aSize = pNum.decimal.size();
    int bSize = nNum.decimal.size();

    //sum decimal parts
    int maxIts = std::max(pNum.decimal.size(), nNum.decimal.size());
    int indexModA = std::min(0, aSize - bSize);
    int indexModB = std::min(0, bSize - aSize);

    int sum = 0;

    bool nonZeroDecimalPushed = false;

    for (int i = 0; i < maxIts; i++) {
        if (i + indexModA < 0) {
            a = 0;
        } else {
            a = pNum.decimal[i + indexModA];
        }
        if (i + indexModB < 0) {
            b = 0;
        } else {
            b = nNum.decimal[i + indexModB];
        }

        sum = a - b - carry;
        if (sum < 0) {
            carry = 1;
            sum += 10;
        } else {
            carry = 0;
        }

        //Prevent trailing zeroes like 0.12300000
        if (!nonZeroDecimalPushed) {
            if (sum != 0) {
                temp.decimal.push_back(sum);
                nonZeroDecimalPushed = true;
            }
        } else {
            temp.decimal.push_back(sum);
        }
    }

    //sum integral parts
    int zeroesToPush = 0;

    aSize = pNum.integral.size();
    bSize = nNum.integral.size();
    maxIts = std::max(aSize, bSize);

    for (int i = 0; i < maxIts; i++) {
        if (i >= aSize) {
            a = 0;
        } else {
            a = pNum.integral[i];
        }
        if (i >= bSize) {
            b = 0;
        } else {
            b = nNum.integral[i];
        }

        sum = a - b - carry;
        if (sum < 0) {
            carry = 1;
            sum += 10;
        } else {
            carry = 0;
        }
         
        //TODO: Combine these if statements?

        if (sum == 0) {
            zeroesToPush++;
        } else {
            if (zeroesToPush) { //i.e. != 0
                for (int j = 0; j < zeroesToPush; j++) {
                    temp.integral.push_back(0);  
                }
                zeroesToPush = 0;
            }
            temp.integral.push_back(sum);
        }
    }

    if (carry) {
        if (zeroesToPush) { //i.e. != 0
            for (int j = 0; j < zeroesToPush; j++) {
                temp.integral.push_back(0);
            }
            zeroesToPush = 0;
        }
        temp.integral.push_back(1);
    }

    return temp;
}

int UFloat::compareUFloats(UFloat num1, UFloat num2, bool absolute) { //return 1 if num1 larger, -1 if num2, 0 if equal, assume stripped of zeroes
    int returnModifier = 1;
    if (!absolute) {
        if (num1.positive) {
            if (!num2.positive) {
                return 1;
            }
        } else {
            if (num2.positive) {
                return -1;
            } else {
                returnModifier = -1;
            }
        }
    }


    if (num1.integral.size() > num2.integral.size()) {
        return 1 * returnModifier;
    } else if (num1.integral.size() < num2.integral.size()) {
        return -1 * returnModifier;
    }

    //compare integrals
    int a = 0, b = 0;
    for (int i = num1.integral.size() - 1; i >= 0; i--) {
        a = num1.integral[i];
        b = num2.integral[i];

        if (a > b) {
            return 1 * returnModifier;
        } else if (a < b) {
            return -1 * returnModifier;
        }
    }

    //compare decimal
        
    /*
    * For decimal from largest to smallest
    * Compare
    * If one doesn't have that decimal, then it is 0
    * If both don't have decimal, then must be equal (will end loop then anyway)
    */
    int aSize = num1.decimal.size();
    int bSize = num2.decimal.size();
    int aIndex = 0, bIndex = 0;
    int maxIts = std::max(aSize, bSize);

    /* The last column is 1/10ths, say [6] for a and [3] for b.
    Then, indexModA = 0, index mod b = -3.
    Therefore, an index of 6 will refer to 6+0 = 6 for a and 6+-3 = 3 for b.
    If negative (e.g. the 1/100000ths for b) then that number doesn't have that decimal.
    The 1/10000ths for a and b are a[2+0] = a[3], and b[2+-3] = b[-1] for b.
    */
    int indexModA = std::min(0, aSize - bSize);
    int indexModB = std::min(0, bSize - aSize);

    for (int i = maxIts-1; i > 0; i--) {
        aIndex = i + indexModA;
        bIndex = i + indexModB;

        if (aIndex < 0) {
            a = 0;
        } else {
            a = num1.decimal[aIndex];
        }
        if (bIndex < 0) {
            b = 0;
        } else {
            b = num2.decimal[bIndex];
        }

        if (a > b) {
            return 1 * returnModifier;
        } else if (a < b) {
            return -1 * returnModifier;
        }
    }

    return 0;
}

UFloat UFloat::sum(UFloat& num1, UFloat& num2) {
    if (num1.positive) {
        if (num2.positive) {
            return UFloat::posPosSum(num1, num2);
        } else {

            int comparision = compareUFloats(num1, num2, true);
            if (comparision == 0) {
                UFloat temp;
                return temp;
            } else if (comparision == -1) { //|num2| larger
                UFloat temp = posNegSum(num2, num1);
                temp.flipPositive();
                return temp;
            } else {
                return posNegSum(num1, num2);
            }

        }
    } else {
        if (num2.positive) {
            int comparision = compareUFloats(num1, num2, true);
            if (comparision == 0) {
                UFloat temp;
                return temp;
            } else if (comparision == -1) { //|num2| larger
                return posNegSum(num2, num1); //-1+2 = 2+-1
            } else {
                UFloat temp = posNegSum(num1, num2);
                temp.flipPositive();
                return temp;
            }
        } else {
            UFloat temp = UFloat::posPosSum(num1, num2);
            temp.flipPositive();
            return temp;
        }
    }
}

UFloat UFloat::mult(UFloat& num1, UFloat& num2) {
    /*
    * 123.456 * 135.246
    * 
    * [321], [654]
    * [531], [642]
    * 
    */

    UFloat total = UFloat();

    int a, b, carry = 0;
    int mul = 0;
    int remainingZeroes;

    int zeroesToPush = 0;
    bool pushedNonZero = false;

    UFloat temp = UFloat();

    for (int i = 0; i < num2.decimal.size(); i++) {
        temp.clear();

        b = num2.decimal[i];

        
        pushedNonZero = false;

        for (int j = 0; j < num1.decimal.size(); j++) {
            a = num1.decimal[j];

            mul = a * b + carry;

            div_t res = div(mul, 10);
            carry = res.quot;

            if (!pushedNonZero) {
                if (res.rem != 0) {
                    temp.decimal.push_back(res.rem);
                    pushedNonZero = true;
                }
            } else {
                temp.decimal.push_back(res.rem);
            }
            
        }

        remainingZeroes = (num2.decimal.size() - i);
        zeroesToPush = 0;

        for (int j = 0; j < num1.integral.size(); j++) {
            a = num1.integral[j];

            mul = a * b + carry;

            div_t res = div(mul, 10);
            carry = res.quot;

            if (remainingZeroes > 0) {
                temp.decimal.push_back(res.rem);
                remainingZeroes--;
            } else {
                if (res.rem != 0) {
                    while (zeroesToPush > 0) {
                        temp.integral.push_back(0);
                        zeroesToPush--;
                    }
                    temp.integral.push_back(res.rem);
                } else {
                    zeroesToPush++;
                }
                
            }
        }
        
        if (carry) {
            if (remainingZeroes > 0) {
                temp.decimal.push_back(carry);
                remainingZeroes--;
            } else {
                while (zeroesToPush > 0) {
                    temp.integral.push_back(0);
                    zeroesToPush--;
                }
                temp.integral.push_back(carry);
            }
        }

        while (remainingZeroes > 0) {
            temp.decimal.push_back(0);
            remainingZeroes--;
        }

        total.add(temp);
        carry = 0;

    }

    

    for (int i = 0; i < num2.integral.size(); i++) {
        temp.clear();

        b = num2.integral[i];

        int pushesToDecimal = num1.decimal.size() - i;

        while (pushesToDecimal < 0) {
            temp.integral.push_back(0);
            pushesToDecimal++;
        }

        pushedNonZero = false;

        for (int j = 0; j < num1.decimal.size(); j++) {
            a = num1.decimal[j];

            mul = a * b + carry;

            div_t res = div(mul, 10);
            carry = res.quot;

            if (pushesToDecimal > 0) {
                if (!pushedNonZero) {
                    if (res.rem != 0) {
                        temp.decimal.push_back(res.rem);
                        pushedNonZero = true;
                    }
                } else {
                    temp.decimal.push_back(res.rem);
                }

                pushesToDecimal--;


            } else {
                temp.integral.push_back(res.rem);
            }
        }

        zeroesToPush = 0;

        for (int j = 0; j < num1.integral.size(); j++) {
            a = num1.integral[j];

            mul = a * b + carry;

            div_t res = div(mul, 10);
            carry = res.quot;

            if (res.rem != 0) {
                while (zeroesToPush > 0) {
                    temp.integral.push_back(0);
                    zeroesToPush--;
                }
                temp.integral.push_back(res.rem);
            } else {
                zeroesToPush++;
            }
        }
        

        if (carry != 0) {
            while (zeroesToPush > 0) {
                temp.integral.push_back(0);
                zeroesToPush--;
            }
            temp.integral.push_back(carry);
        }

        total.add(temp);
    }
    

    total.positive = (num1.positive == num2.positive);
    return total;
}

UFloat UFloat::floatMult(UFloat& uNum, float fNum) {
    UFloat total = UFloat();
    if (fNum == 0) {
        return total;
    }

    if (uNum.positive != (fNum > 0)) {
        total.positive = false;
    }

    //integral part
    for (int i = 0; i < uNum.integral.size(); i++) {
        UFloat temp = UFloat();

        int a = uNum.integral[i];
        float mul = a * fNum; //e.g. "234354.2345", want to times by 10^i

    }
}

UFloat UFloat::floatToUFloat(float num) {
    UFloat temp = UFloat(num);
    return temp;
}

UFloat UFloat::intToUFloat(int num) {
    UFloat temp = UFloat(num);
    return temp;
}

UFloat UFloat::stringToUFloat(std::string num) {
    UFloat temp = UFloat(num);
    return temp;
    /*
    std::string in, de;
    std::string validNums = "0123456789";

    unsigned int minusMod = 0;

    if (num.size() >= 1 && num[0] == '-') {
        temp.positive = false;
        minusMod = 1;
    }

    size_t decimalPos = num.find(".");
    if (decimalPos != std::string::npos) {
        in = num.substr(minusMod, decimalPos-minusMod);
        de = num.substr(std::min(decimalPos + 1, num.size() - 1), num.size() - 1 - decimalPos);
    } else {
        in = num;
    }

    for (int i = in.size()-1; i >= 0; i--) {
        char digit = in[i];
        if (validNums.find(digit) != std::string::npos) {
            temp.integral.push_back((int) digit - '0');
        } else {
            std::cerr << "UFloat Error: Encountered '" << digit << "' when converting '" << num << "' to UFloat" << std::endl;
            return temp; //will cause weird behaviour but shouldn't crash
        }
    }
    for (int i = de.size()-1; i >= 0 ; i--) {
        char digit = de[i];
        if (validNums.find(digit) != std::string::npos) {
            temp.decimal.push_back((int) digit - '0');
        } else {
            std::cerr << "UFloat Error: Encountered '" << digit << "' when converting '" << num << "' to UFloat" << std::endl;
            return temp; //will cause weird behaviour but shouldn't crash
        }
    }

    return temp;*/
}

float UFloat::uFloatToFloat(UFloat num) {
    return std::stof(uFloatToString(num));
}

int UFloat::uFloatToInt(UFloat num) {
    return std::stoi(uFloatToString(num));
}

std::string UFloat::uFloatToString(UFloat num) {
    std::string temp = "";
    if (!num.positive) {
        temp.append("-");
    }
    if (!num.integral.size()) {
        temp.append("0");
    } else {
        for (int i = num.integral.size() - 1; i >= 0; i--) {
            temp.append(std::to_string(num.integral[i]));
        }
    }
    
    if (num.decimal.size()) {
        temp.append(".");
    }
    for (int i = num.decimal.size() - 1; i >= 0; i--) {
        temp.append(std::to_string(num.decimal[i]));
    }

    return temp;
}

UFloat::UFloat() {
}

UFloat::UFloat(int num) :
    UFloat(std::to_string(num)) {
    ;
}

UFloat::UFloat(float num) :
    UFloat(std::to_string(num)) {
    ;
}

UFloat::UFloat(std::string num) {

    std::string in, de;
    std::string validNums = "0123456789";

    unsigned int minusMod = 0;

    if (num.size() >= 1 && num[0] == '-') {
        positive = false;
        minusMod = 1;
    }

    size_t decimalPos = num.find(".");
    if (decimalPos != std::string::npos) {
        in = num.substr(minusMod, decimalPos-minusMod);
        de = num.substr(std::min(decimalPos + 1, num.size() - 1), num.size() - 1 - decimalPos);
    } else {
        in = num.substr(minusMod, num.size()-minusMod);;
    }

    for (int i = in.size() - 1; i >= 0; i--) {
        char digit = in[i];
        if (validNums.find(digit) != std::string::npos) {
            integral.push_back((int)digit - '0');
        } else {
            std::cerr << "UFloat Error: Encountered '" << digit << "' when converting '" << num << "' to UFloat" << std::endl;
        }
    }
    for (int i = de.size() - 1; i >= 0; i--) {
        char digit = de[i];
        if (validNums.find(digit) != std::string::npos) {
            decimal.push_back((int)digit - '0');
        } else {
            std::cerr << "UFloat Error: Encountered '" << digit << "' when converting '" << num << "' to UFloat" << std::endl;
        }
    }
}

UFloat::UFloat(const char* num) :
    UFloat(std::string(num)) {
    ;
}

void UFloat::add(UFloat& num) {
    /*UFloat temp = sum(*this, num);
    positive = temp.positive;
    integral = temp.integral;
    decimal = temp.decimal;*/
    //*this = temp?
    *this = sum(*this, num);
}

void UFloat::sub(UFloat& num) {
    UFloat temp = num;
    temp.flipPositive();

    *this = sum(*this, temp);
    
}

void UFloat::mult(UFloat& num) {
    *this = mult(*this, num);
}

std::string UFloat::toString() {
    return uFloatToString(*this);
}

void UFloat::flipPositive() {
    positive = !positive;
}

void UFloat::clear() {
    positive = true;
    integral.clear();
    decimal.clear();
}

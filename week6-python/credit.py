number = int(input("Number: "))
numberLength = len(str(number))
card = None


# Multiply every other digit by 2, starting with the number’s second-to-last digit, and then add those products’ digits together.
# Add the sum to the sum of the digits that weren’t multiplied by 2.
# If the total’s last digit is 0 (or, put more formally, if the total modulo 10 is congruent to 0), the number is valid!


def isNumberLuhn(number):
    numberString = str(number)
    underlinedDigits = []

    for i in range(numberLength-2, -1, -2):
        underlinedDigits.append(int(numberString[i])*2)

    productsDigitsSum = 0

    for i in underlinedDigits:
        for digit in str(i):
            productsDigitsSum += int(digit)

    totalSum = productsDigitsSum

    for i in range(numberLength-1, -1, -2):
        totalSum += int(numberString[i])

    if str(totalSum)[-1] == "0":
        return True
    else:
        return False


isLuhn = isNumberLuhn(number)

firstTwoNumbers = int(str(number)[0:2])
if isLuhn == True:
    if numberLength == 15 and firstTwoNumbers in [34, 37]:
        print("AMEX")
    elif numberLength == 16 and firstTwoNumbers in [51, 52, 53, 54, 55]:
        print("MASTERCARD")
    elif numberLength in [13, 16] and str(number)[0] == "4":
        print("VISA")
    else:
        print("INVALID")
else:
    print("INVALID")

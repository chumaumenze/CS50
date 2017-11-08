#include <stdio.h>
//#include <cs50.h>

int countCC(long long cardnum);
void ccDigit(long long cardigit);

long long ccNumber;
int count, total, eve, odd, cardRev[100];

int main()
{
    printf("Number: ");
    scanf("%19lli", &ccNumber);
    //ccNumber = get_long_long();
    countCC(ccNumber);
    ccDigit(ccNumber);
	if (total == 0){
		switch (count){
			case 13:
				if(cardRev[count-1]== 4){
					printf("VISA\n");
					break;
				}
			case 15:
				if((cardRev[count-1] == 3) && (cardRev[count-2] == 7)){
					printf("AMEX\n");
					break;
				}
			case 16:
				if ((cardRev[count-1] == 5) && ((cardRev[count-2] >= 51) || (cardRev[count-2] <= 56))){
					printf("MASTERCARD\n");
					break;
				}
				else {
				    if ((count == 16) && ((cardRev[count-1]) == 4)){
					    printf("VISA\n");
					    break;
				    }
				}
			default:
			printf("INVALID\n");
			break;
		}
	}
	else{
	    printf("INVALID\n");
	}
    return 0;
}


// Getting total number of digits.
int countCC(long long cardnum){
    long long CCnt = cardnum;
    while (CCnt > 0){
        CCnt /= 10;
        count++;
    }
    return count;
}

void ccDigit (long long cardigit){
    long long card = cardigit;
    while (card != 0){
        for (int i = 0; i < count; i++){
        int temp = 0;
        temp = card % 10;
        cardRev[i] = temp;
        card /= 10;
        }
    }
    
    for(int i = 0; i < 16; i++){
        while (i != 16) {
            odd += cardRev[i];
            int temp = 0;
            temp += (cardRev[i+1] * 2);
            if (temp > 9){
                temp -= 9;
            }
            eve += temp;
            i += 2;
        }
    }
    total = eve + odd;
    total %= 10;
}
//Purpose: Make a command line interface baccarat game in C
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#ifdef _WIN32
#include <windows.h>
#define sleep(seconds) Sleep((seconds) * 1000) // note windows sleep uses milliseconds
#else
#include <unistd.h>
#endif
// i hope i implemented a cross platform solution properly here for the sleep function

//defining global variables
int playerCard1; int playerCard2; int playerCard3; int playerTotal;
int bankerCard1; int bankerCard2; int bankerCard3; int bankerTotal;
int bankroll; char betName; int betAmount;
int broke = 1;

//gives a random card to player and banker that's 0-9, I removed 10's because they're equal to 0
int getCard() {
    return rand() % 10;
}

//draws cards for player and banker and displays them
void drawCards() {
    playerCard1 = getCard(); playerCard2 = getCard();
    bankerCard1 = getCard(); bankerCard2 = getCard();
    playerTotal = (playerCard1 + playerCard2) % 10;
    bankerTotal = (bankerCard1 + bankerCard2) % 10;
    printf("Player cards: %d %d (Total: %d)\n", playerCard1, playerCard2, playerTotal);
    printf("Banker cards: %d %d (Total: %d)\n", bankerCard1, bankerCard2, bankerTotal);

    //check for natural 9s/8s here
    if (playerTotal == 8 || playerTotal == 9 || bankerTotal == 8 || bankerTotal == 9) {
        printf("Natural 8 or 9. No third card draw.\n");
        return;
    }

    //players third card logic
    if (playerTotal < 6) {
        playerCard3 = getCard();
        playerTotal = (playerTotal + playerCard3) % 10;
        printf("Player draws a third card: %d (New Total: %d)\n", playerCard3, playerTotal);
    } else {
        printf("Player stands with: %d\n", playerTotal);
    }

    //bankers third card logic
    if (bankerTotal <= 2) {
        bankerCard3 = getCard();
    } else if (bankerTotal == 3 && (playerCard3 != 8)) {
        bankerCard3 = getCard();
    } else if (bankerTotal == 4 && (playerCard3 >= 2 && playerCard3 <= 7)) {
        bankerCard3 = getCard();
    } else if (bankerTotal == 5 && (playerCard3 >= 4 && playerCard3 <= 7)) {
        bankerCard3 = getCard();
    } else if (bankerTotal == 6 && (playerCard3 == 6 || playerCard3 == 7)) {
        bankerCard3 = getCard();
    } else {
        bankerCard3 = -1; //banker stands, no third card
    }

    if (bankerCard3 != -1) {
        bankerTotal = (bankerTotal + bankerCard3) % 10;
        printf("Banker draws a third card: %d (New Total: %d)\n", bankerCard3, bankerTotal);
    } else {
        printf("Banker stands with: %d\n", bankerTotal);
    }
}

//check for winners here
void winnerCheck() {
    if (playerTotal > bankerTotal) { //player win here
        printf("Player wins\n");
        if (betName == 'p') { //if user bets player they win here
            bankroll += (betAmount * 2);
            printf("Your new balance is: %d\n", bankroll);
        } else { //if user bets banker they lose here
            bankroll -= betAmount;
            printf("Your new balance is: %d\n", bankroll);
        }
    } else if (bankerTotal > playerTotal) { //banker win here
        printf("Banker wins\n");
        if (betName == 'b') { //if user bet banker they win here
            bankroll += (int)(betAmount * 2 * 0.95); //calculate the %5 commission that casinos take here
            printf("Your new balance is: %d\n", bankroll);
        } else { //if user bet player they lose here
            bankroll -= betAmount;
            printf("Your new balance is: %d\n", bankroll);
        }
    } else { //tie resolved here
        printf("It's a tie!\n");
        if (betName == 'p' || betName == 'b') { //if user didn't bet on the tie they get their money back
            printf("Your bet is returned.\n");
        } else { //if user bet on the tie they win here
            bankroll += betAmount * 8;
            printf("You win 8:1. Your new balance is: %d\n", bankroll);
        }
    }
    if (bankroll == 0) broke = 0; //if the user runs out of money game ends here
}

//sets a bankroll for the session
void init() {printf("Enter your desired bankroll: "); scanf("%d", &bankroll);}

//user places bets here
void placeBet() {
    while (1) {
        printf("Who would you like to place your bet on? (p/b/t): "); scanf(" %c", &betName); //note do not remove that space in scanf, otherwise it'll glitch
        if (betName == 'p' || betName == 'b' || betName == 't') break;
        printf("Please try again\n");
    }
    while (1) {
        printf("Enter how much you would like to bet: "); scanf("%d", &betAmount);
        if (betAmount <= bankroll && betAmount > 0) break;
        printf("Please enter an amount that is less than or equal to your bankroll\n");
    }
}

int main() {
    printf("Welcome to Baccarat\n");
    init();
    srand(time(NULL)); //I think this is important for number randomization
    while (broke == 1) {
        placeBet();
        drawCards();
        winnerCheck();
    }
    //game over sequence starts here
    printf("You've lost your bankroll, game over.\n"); //roasts the user
    for (int i = 5; i > 0; i--) {
        if (i == 1) {printf("This window will close in 1 second."); sleep(1);} //schizo formatting
        else {
            printf("This window will close in %d seconds.\n", i); //gives player time to contemplate life choices
            sleep(1);
        }
    }
    return 0; //everything works fine!
}
/* 
 * Program: adventurer random tester
 * Author: Allan Slocum
 * Date: 2/27/19 
 * Description: a random test program for dominion cards using seeds and rngo functions to generate random hand possibilities.
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>
#include "time.h"

#define TESTCARD "adventurer"

int main() {
	srand(time(0));
    	int newCards = 0;
    	int discarded = 1;
    	int xtraCoins = 0;
    	int shuffledCards = 0;
    	int actions = 0;
	int testStatus = 0;
	int drawntreasure = 0;
	int cardDrawn;
	int temphand[MAX_HAND];
    	int i;
	int z = 0;
    	int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
    	int seed;
    	int numPlayers = 2;
    	int thisPlayer = 0;
	struct gameState G, testG;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
			sea_hag, tribute, smithy, council_room};

	for (i = 1; i < 101; i++){
		// initialize a game state and player cards
		seed = (rand()%1000);
		initializeGame(numPlayers, k, seed, &G);

		printf("----------------- Test %d Card: %s ----------------\n", i, TESTCARD);
		printf("seed = %d\n", seed);

		// copy the game state to a test case
		memcpy(&testG, &G, sizeof(struct gameState));
		cardEffect(adventurer, choice1, choice2, choice3, &testG, handpos, &bonus);
		z = 0;
		drawntreasure = 0;	
		while(drawntreasure<2){
			if (G.deckCount[thisPlayer] <1){
				shuffle(thisPlayer, &G);
			}
			drawCard(thisPlayer, &G);
			cardDrawn = G.hand[thisPlayer][G.handCount[thisPlayer]-1];
			if (cardDrawn == copper || cardDrawn == silver || cardDrawn == gold)
			  drawntreasure++;
			else{
			  	temphand[z]=cardDrawn;
			  	G.handCount[thisPlayer]--;
			  	z++;
			}
		}
	      while(z-1>=0){
		G.discard[thisPlayer][G.discardCount[thisPlayer]++]=temphand[z-1]; 
		z=z-1;
	      }

		printf("hand count = %d, expected = %d\n", testG.handCount[thisPlayer], G.handCount[thisPlayer] + newCards - discarded);
		printf("deck count = %d, expected = %d\n", testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - newCards + shuffledCards);
		printf("coins = %d, expected = %d\n", testG.coins, G.coins + xtraCoins);
		printf("actions = %d, expected = %d\n", testG.numActions, G.numActions + actions);
		if(testG.handCount[thisPlayer] != G.handCount[thisPlayer] + newCards - discarded){
			testStatus++;
		}
		if(testG.deckCount[thisPlayer] != G.deckCount[thisPlayer] - newCards + shuffledCards){
			testStatus++;
}
		if(testG.coins != G.coins + xtraCoins){
			testStatus++;
		}
		if(testG.numActions != G.numActions + actions){
			testStatus++;
		}
	}
	if(testStatus == 0){
		printf("\n >>>>> SUCCESS: Testing complete %s <<<<<\n\n", TESTCARD);
	}
	else{
		printf("\n >>>>> FAILURE: Testing complete %s, %d errors <<<<<\n\n", TESTCARD, testStatus);
	}
	return 0;
}


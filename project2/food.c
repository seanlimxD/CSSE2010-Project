/*
** food.c
**
** Written by Peter Sutton
**
*/
#include <stdlib.h>

#include "position.h"
#include "food.h"
#include "snake.h"
#include "board.h"

/*
** Global variables.
** foodPositions records the (x,y) positions of each food item.
** numFoodItems records the number of items. The index into the
** array is the food ID. Food IDs will range between 0 and 
** numFoodItems-1. If we delete a food item, we rearrange the 
** array.
**
*/
PosnType foodPositions[MAX_FOOD];
PosnType superfood_position;
int8_t numFoodItems;

/* 
** Initialise food details.
*/
void init_food(void) {
	numFoodItems = 0; 
}

/* Returns true if there is food at the given position, false (0) otherwise.
*/
uint8_t is_food_at(PosnType posn) {
	return (food_at(posn) != -1);
}

/* Returns a food ID if there is food at the given position,
** otherwise returns -1
*/
int8_t food_at(PosnType posn) {
    int8_t id;
	// Iterate over all the food items and see if the position matches
    for(id=0; id < numFoodItems; id++) {
        if(foodPositions[id] == posn) {
            // Food found at this position 
            return id;
        }
    }
    // No food found at the given position.
    return -1;
}

int8_t superfood_at(PosnType posn) {
	if (posn == superfood_position){
		return 1;
	}
	return 0;
}

/* Add a food item and return the position - or 
** INVALID_POSITION if we can't.
*/
PosnType add_food_item(void) {
	if(numFoodItems >= MAX_FOOD) {
		// Can't fit any more food items in our list
		return INVALID_POSITION;
	}
	/* Generate "random" positions until we get one which
	** is not occupied by a snake or food.
	*/
	int8_t x, y, attempts;
	PosnType test_position;
	x = 0;
	y = 0;
	attempts = 0;
	do {
		// Generate a new position - this is based on a sequence rather
		// then being random
        x = (x+3+attempts)%BOARD_WIDTH;
        y = (y+5)%BOARD_HEIGHT;
		test_position = position(x,y);
        attempts++;
    } while(attempts < 100 && 
                (is_snake_at(test_position) || is_food_at(test_position)));
        
    if(attempts >= 100) {
        /* We tried 100 times to generate a position
        ** but they were all occupied.
        */
        return INVALID_POSITION;
    }
	
	// If we get here, we've found an unoccupied position (test_position)
	// Add it to our list, display it, and return its ID.
	int8_t newFoodID = numFoodItems;
	foodPositions[newFoodID] = test_position;
	numFoodItems++;
	return test_position;
}

PosnType add_super_food(void){
	int8_t x, y, attempts;
	PosnType test_position;
	x = 0;
	y = 0;
	attempts = 0;
	do {
		// Generate a new position - this is based on a sequence rather
		// then being random
		x = rand()%BOARD_WIDTH;
		y = rand()%BOARD_HEIGHT;
		test_position = position(x,y);
		attempts++;
	} while(attempts < 100 && (is_snake_at(test_position) || is_food_at(test_position)));
	if (attempts >= 100) {
		return INVALID_POSITION;
	}
	superfood_position = test_position;
	return test_position;
}


/* Return the position of the given food item. The ID is assumed
** to be valid.
*/
PosnType get_position_of_food(int8_t foodID) {
	return foodPositions[foodID];
}

PosnType get_superfood_position(){
	return superfood_position;
}

/*
** Remove the food item from our list of food
*/
void remove_food(int8_t foodID) {
    int8_t i;
        
    if(foodID < 0 || foodID >= numFoodItems) {
        /* Invalid foodID */
        return;
    }
	     
    /* Shuffle our list of food items along so there are
	** no holes in our list 
	*/
    for(i=foodID+1; i <numFoodItems; i++) {
        foodPositions[i-1] = foodPositions[i];
    }
    numFoodItems--;
}

void remove_super_food(){
	superfood_position = position(NULL,NULL);
}

int8_t move_rat(void) {
	int8_t ratX;	/* head X position */
	int8_t ratY;	/* head Y position */
	PosnType newRatPosn;
    
	/* Current head position */
	ratX = x_position(foodPositions[0]);
	ratY = y_position(foodPositions[0]);
    
    /* Work out where the new head position should be - we
    ** move 1 position in our NEXT direction of movement if we can.
	** If we're at the edge of the board, then we wrap around to
	** the other edge.
    */
    switch (rand()%4) {
        case 1:
			if(ratY == BOARD_HEIGHT - 1 || (is_snake_at(newRatPosn) == 1 || food_at(newRatPosn) || superfood_at(newRatPosn))) {
				// Head is already at the top of the board - wrap around
				ratY -= 1;
			} else {
	            ratY += 1;
			}
            break;
        case 2:
			if(ratX == BOARD_WIDTH - 1 || (is_snake_at(newRatPosn) == 1 || food_at(newRatPosn) || superfood_at(newRatPosn))) {
				// Snake head is already at the right hand edge of the board
				// - wrap around to left hand side
				ratX -= 1;
			} else {
				ratX += 1;
			}
            break;
		/* YOUR CODE HERE to deal with other directions */
		case 3:
			if(ratY == 0 || (is_snake_at(newRatPosn) == 1 || food_at(newRatPosn) || superfood_at(newRatPosn))) {
			// Head is already at the bottom of the board - wrap around
			ratY += 1;
			} else {
			ratY -= 1;
			}
			break;
		case 4:
			if(ratX == 0 || (is_snake_at(newRatPosn) == 1 || food_at(newRatPosn) || superfood_at(newRatPosn))) {
				// Snake head is already at the right hand edge of the board
				// - wrap around to left hand side
				ratX += 1;
			} else {
				ratX -= 1;
			}
			break;
    }

	newRatPosn = position(ratX, ratY);
	/* Store the head position */
	foodPositions[0] = newRatPosn;
	return MOVE_OK;
}

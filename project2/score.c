/*
 * score.c
 *
 * Written by Peter Sutton
 */

#include <stdio.h>
#include <stdint.h>
#include <avr/pgmspace.h>

#include "score.h"
#include "terminalio.h"

// Variable to keep track of the score. We declare it as static
// to ensure that it is only visible within this module - other
// modules should call the functions below to modify/access the
// variable.
static uint32_t score;

void init_score(void) {
	score = 0;
}

void add_to_score(uint16_t value) {
	score += value;
	hide_cursor();	// We don't need to see the cursor when we're just doing output
	move_cursor(50,0);
	printf_P(PSTR("Score: %ld"), score);
}

uint32_t get_score(void) {
	return score;
}

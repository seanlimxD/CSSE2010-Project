/*
 * score.c
 *
 * Written by Peter Sutton
 */

#include <stdio.h>
#include <stdint.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>

#include "score.h"
#include "terminalio.h"
#include "serialio.h"

// Variable to keep track of the score. We declare it as static
// to ensure that it is only visible within this module - other
// modules should call the functions below to modify/access the
// variable.
static uint32_t score;
#define NAME_SIZE 10
uint8_t* EEMEM leaderp1[NAME_SIZE];
uint8_t* EEMEM leaderp2[NAME_SIZE];
uint8_t* EEMEM leaderp3[NAME_SIZE];
uint8_t* EEMEM leaderp4[NAME_SIZE];
uint8_t* EEMEM leaderp5[NAME_SIZE];
uint8_t leader1[]="leader1";
uint8_t leader2[]="leader2";
uint8_t leader3[]="leader3";
uint8_t leader4[]="leader4";
uint8_t leader5[]="leader5";
uint16_t* EEMEM topscore1;
uint16_t* EEMEM topscore2;
uint16_t* EEMEM topscore3;
uint16_t* EEMEM topscore4;
uint16_t* EEMEM topscore5;
uint16_t top1 = 10;
uint16_t top2 = 9;
uint16_t top3 = 8;
uint16_t top4 = 7;
uint16_t top5 = 6;

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

void show_leaders(){	
	  if ( !eeprom_is_ready () ) {
		  printf_P (PSTR ("Waiting for EEPROM to become ready...\n"));
		  eeprom_busy_wait ();
	  }
	  eeprom_read_block(leader1, leaderp1, NAME_SIZE);
	  eeprom_read_block(leader2, leaderp2, NAME_SIZE);
	  eeprom_read_block(leader3, leaderp3, NAME_SIZE);
	  eeprom_read_block(leader4, leaderp4, NAME_SIZE);
	  eeprom_read_block(leader5, leaderp5, NAME_SIZE);
	  printf_P(PSTR("\n"));
	  printf_P (PSTR ("*******************************************\n"));
	  printf_P (PSTR ("*            Our Leader Board             *\n"));
	  printf_P (PSTR ("*******************************************\n"));
	  printf_P (PSTR ("*   First :  %s , Score: %u\n"), leader1, top1 = eeprom_read_word(topscore1));
	  printf_P (PSTR ("*   Second:  %s , Score: %u\n"), leader1, top2 = eeprom_read_word(topscore2));
	  printf_P (PSTR ("*   Third :  %s , Score: %u\n"), leader1, top3 = eeprom_read_word(topscore3));
	  printf_P (PSTR ("*   Fourth:  %s , Score: %u\n"), leader1, top4 = eeprom_read_word(topscore4));
	  printf_P (PSTR ("*   Fifth :  %s , Score: %u\n"), leader1, top5 = eeprom_read_word(topscore5));
	  printf_P (PSTR ("*******************************************\n"));	  
}

uint8_t is_leader(){
	score = 5000;
	eeprom_update_word(topscore5, score);
	uint8_t new_leader[NAME_SIZE]="NewLeader";
	if ( !eeprom_is_ready () ) {
		printf_P (PSTR ("Waiting for EEPROM to become ready...\n"));
		eeprom_busy_wait ();
	}
		if (score > top5){
			eeprom_update_word(topscore5, score);
			eeprom_update_block(&new_leader, &leaderp5, NAME_SIZE);
		}
		if (score > top4){
			eeprom_update_word(topscore5, top4);
			eeprom_update_block(&leader4, &leaderp5, NAME_SIZE);
			eeprom_update_word(topscore4, score);
			eeprom_update_block(&new_leader, &leaderp4, NAME_SIZE);
		}
		if (score > top3){
			eeprom_update_word(topscore4, top3);
			eeprom_update_block(&leader3, &leaderp4, NAME_SIZE);
			eeprom_update_word(topscore3, score);
			eeprom_update_block(&new_leader, &leaderp3, NAME_SIZE);
		}
		if (score > top2){
			eeprom_update_block(&leader2, &leaderp3, NAME_SIZE);
			eeprom_update_word(topscore3, top2);
			eeprom_update_block(&new_leader, &leaderp2, NAME_SIZE);
			eeprom_update_word(topscore2, score);
		}
		if (score > top1){
			eeprom_update_block(&leader1, &leaderp2, NAME_SIZE);
			eeprom_update_word(topscore2, top1);
			eeprom_update_word(topscore1, score);
			eeprom_update_block(new_leader, leaderp1, NAME_SIZE);
		}

	if (score > top5) return 1; else return 0;
	
}

void write_leaders(){
	uint8_t new_leader[NAME_SIZE]="Anonymous";
	if (input_user()>2) {
//		no time to update the name of the new_leader		
	}
}
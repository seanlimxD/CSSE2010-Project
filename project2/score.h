	/*
 * score.h
 * 
 * Author: Peter Sutton
 */

#ifndef SCORE_H_
#define SCORE_H_

#include <stdint.h>

void init_score(void);
void add_to_score(uint16_t value);
uint32_t get_score(void);
void write_to_EEPROM(uint8_t leader);
void show_leaders();
uint8_t is_leader();

#endif /* SCORE_H_ */
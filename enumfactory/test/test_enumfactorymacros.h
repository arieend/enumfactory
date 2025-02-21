/*
 * =====================================================================================
 *
 *       Filename:  test_enumfactorymacros.h
 *
 *    Description:  Tests for enumfactorymacros.h
 *
 *        Version:  1.0
 *        Created:  10/07/2023
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  GitHub Copilot
 *
 * =====================================================================================
 */

#ifndef  __TEST_ENUMFACTORYMACROS_H__
#define  __TEST_ENUMFACTORYMACROS_H__

#include "enumfactorymacros.h"
#include <assert.h>
#include <string.h>

/* Define an example enum using ENUMS_AUTOMATIC */
#define COLORS(_) \
  _(RED) \
  _(GREEN) \
  _(BLUE)

ENUMS_AUTOMATIC(COLORS)

/* Define an example enum using ENUMS_ASSIGNED */
#define STATUS(_) \
  _(OK, 0) \
  _(ERROR, 1) \
  _(UNKNOWN, -1)

ENUMS_ASSIGNED(STATUS)

void test_enum_automatic() {
  assert(RED == 0);
  assert(GREEN == 1);
  assert(BLUE == 2);
  assert(COLORS_total == 3);

  assert(strcmp(COLORS_label[RED], "RED") == 0);
  assert(strcmp(COLORS_label[GREEN], "GREEN") == 0);
  assert(strcmp(COLORS_label[BLUE], "BLUE") == 0);
}

void test_enum_assigned() {
  assert(OK == 0);
  assert(ERROR == 1);
  assert(UNKNOWN == -1);
  assert(STATUS_total == 3);

  assert(strcmp(STATUS_label[OK], "OK") == 0);
  assert(strcmp(STATUS_label[ERROR], "ERROR") == 0);
  assert(strcmp(STATUS_label[UNKNOWN], "UNKNOWN") == 0);
}

int main() {
  test_enum_automatic();
  test_enum_assigned();
  return 0;
}

#endif   /*  __TEST_ENUMFACTORYMACROS_H__ */
#include <stdio.h>

void print_enum_automatic() {
	for (int i = 0; i < COLORS_total; ++i) {
		printf("Color %d: %s\n", i, COLORS_label[i]);
	}
}

void print_enum_assigned() {
	for (int i = 0; i < STATUS_total; ++i) {
		printf("Status %d: %s\n", i, STATUS_label[i]);
	}
}

int main() {
	test_enum_automatic();
	test_enum_assigned();
	print_enum_automatic();
	print_enum_assigned();
	return 0;
}
//
// Created by victor on 10/30/23.
//

#ifndef ATMELSAMD21_EVSYS_IMP_HPP
#define ATMELSAMD21_EVSYS_IMP_HPP
#include <sam.h>

void setup_evsys_handler();

void EVSYS_Handler_EVD0(void) __attribute__((weak));
void EVSYS_Handler_EVD1(void) __attribute__((weak));
#endif //ATMELSAMD21_EVSYS_IMP_HPP

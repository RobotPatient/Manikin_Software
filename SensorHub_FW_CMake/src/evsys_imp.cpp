#include "evsys_imp.hpp"

void EVSYS_Handler(){
    if(EVSYS->INTFLAG.reg & EVSYS_INTFLAG_EVD0) {
        EVSYS->INTFLAG.reg = EVSYS_INTFLAG_EVD0;
        EVSYS_Handler_EVD0();
    }

    if (EVSYS->INTFLAG.reg & EVSYS_INTFLAG_EVD1) {
        EVSYS->INTFLAG.reg = EVSYS_INTFLAG_EVD1;
        EVSYS_Handler_EVD1();
    }

}

void EVSYS_Handler_EVD0(void) {

}

void EVSYS_Handler_EVD1(void) {

}

void setup_evsys_handler() {
    PM->APBCMASK.reg |= PM_APBCMASK_EVSYS;
    GCLK->CLKCTRL.reg = GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_ID_EVSYS_1 | GCLK_CLKCTRL_CLKEN;
    while (GCLK->STATUS.bit.SYNCBUSY);

    EVSYS->CHANNEL.reg = EVSYS_CHANNEL_PATH_RESYNCHRONIZED | EVSYS_CHANNEL_CHANNEL(1) | EVSYS_CHANNEL_EDGSEL_RISING_EDGE;
    EVSYS->INTENSET.reg = EVSYS_INTENSET_EVD(2);
    NVIC_ClearPendingIRQ(EVSYS_IRQn);
    NVIC_EnableIRQ(EVSYS_IRQn);
    NVIC_SetPriority(EVSYS_IRQn, 2);
}




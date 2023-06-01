#include "dfPlayer.hpp"


/* *******************************************************************************************
 * Copyright (c) 2023 by RobotPatient Simulators
 *
 * Authors: Zein Sabbagh
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction,
 *
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software
 * is furnished to do so,
 *
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
***********************************************************************************************/


dfPlayer& dfPlayer::dfPlayerGetInstance()  {
    static dfPlayer dfPlayerInstance;
    return dfPlayerInstance;
}

const dfPlayer::dfPlayereDetails_t dfPlayer::begin(Uart &serial) {
    serial_ = &serial;
    serial_->begin(DF_PLAYER_BAUD_RATE);
    if(!serial_)
        return dfPlayereDetails_t::SerialError;

    delay(DFPLAYER_BOOT_DELAY);


    //dfPlayerStop();
    //dfPlayerReset();
    //dfPlayerSetSource(2);

    //dfPlayerSetVolume(0);

    return dfPlayereDetails_t::SerialEstablished;
}

void dfPlayer::dfPlayerStop() {
    const uint8_t data[2] = {0x00, 0x00};
    serialSendData(DFPLAYER_STOP_PLAYBACK, data);
}

void dfPlayer::dfPlayerReset() {
    const uint8_t data[2] = {0x00, 0x00};
    serialSendData(DFPLAYER_RESET, data);
    delay(DFPLAYER_BOOT_DELAY);
}

void dfPlayer::dfPlayerEnd() {
    dfPlayerStop();
    dfPlayerReset();
    serial_->end();
}

void dfPlayer::dfPlayerSetSource(const uint8_t src) {
    const uint8_t data[2] = {0x00, src};
    serialSendData(DFPLAYER_SET_PLAY_SRC, data);
    delay(200);
}

void dfPlayer::dfPlayerSetEQ(const uint8_t EQ) {
    if(EQ < 0 || EQ > 5)
        return;
    const uint8_t data[2] = {0x00, EQ};
    serialSendData(DFPLAYER_SET_EQ, data);
}

void dfPlayer::dfPlayerSetVolume(const uint8_t volume) {
    if(volume < 0 || volume > 30)
        return;

    const uint8_t data[2] = {0x00, volume};
    serialSendData(DFPLAYER_SET_VOL, data);
}

void dfPlayer::dfPlayerVolumeUp() {
    const uint8_t data[2] = {0x00, 0x00};
    serialSendData(DFPLAYER_SET_VOL_UP, data);
}

void dfPlayer::dfPlayerVolumeDown() {
    const uint8_t data[2] = {0x00, 0x00};
    serialSendData(DFPLAYER_SET_VOL_DOWN, data);
}

const int dfPlayer::fdPlayerGetVolume() { // To be tested
    const uint8_t data[2] = {0x00, 0x00};
    serialSendData(DFPLAYER_GET_VOL, data);
    
    while(!serial_->available());

    return serial_->read();
}

void dfPlayer::dfPlayerPlay(const uint16_t mp3File) {
    if(mp3File <= 0 || mp3File > 9999)
        return;

    const uint8_t data[2] = {(uint8_t)((mp3File & 0xFF00) >> 8), (uint8_t)(mp3File & 0x00FF)};
    serialSendData(DFPLAYER_PLAY_TRACK, data);
}

void dfPlayer::dfPlayerPlayPrevious() {
    const uint8_t data[2] = {0x00, 0x00};
    serialSendData(DFPLAYER_PLAY_PREV, data);
}

void dfPlayer::dfPlayerPlayNext() {
    const uint8_t data[2] = {0x00, 0x00};
    serialSendData(DFPLAYER_PLAY_NEXT, data);
}

void dfPlayer::dfPlayerPlayFolder(const uint8_t folderName, const uint8_t mp3File) {
    if(folderName < 1 || folderName > 99)
        return;
    if(mp3File < 1 || mp3File > 255)
        return;

    const uint8_t data[2] = {folderName, mp3File};
    serialSendData(DFPLAYER_PLAY_NEXT, data);
}

void dfPlayer::dfPlayerStopPlaying() {
    const uint8_t data[2] = {0x00, 0x00};
    serialSendData(DFPLAYER_STOP_PLAYBACK, data);
}

void dfPlayer::dfPlayerPause() {
    const uint8_t data[2] = {0x00, 0x00};
    serialSendData(DFPLAYER_PAUSE, data);
}

void dfPlayer::dfPlayerResume(){
    const uint8_t data[2] = {0x00, 0x00};
    serialSendData(DFPLAYER_RESUME_PLAYBACK, data);
}

void dfPlayer::dfPlayerRepeat(const bool repeat) {
    //0=repeat, 1=stop repeat
    const uint8_t data[2] = {0x00, !repeat};
    serialSendData(DFPLAYER_LOOP_CURRENT_TRACK, data);
}

void dfPlayer::dfPlayerRepeatFolder(const uint8_t folderName) {
    if(folderName < 1 || folderName > 99)
        return;

    const uint8_t data[2] = {0x00, folderName};
    serialSendData(DFPLAYER_REPEAT_FOLDER, data);    
    
}

void dfPlayer::dfPlayerSleep() {
    dfPlayerSetSource(6);
}

void dfPlayer::dfPlayerWakeUp() {
    dfPlayerSetSource(2);
}

void dfPlayer::serialSendData(const uint8_t cmd, const uint8_t *data) {
    const auto ackIsDisabled = true;
    const uint8_t buff[DFPLAYER_UART_FRAME_SIZE] = {
        DFPLAYER_UART_START_BYTE,
        DFPLAYER_UART_VERSION,
        DFPLAYER_UART_DATA_LEN,
        cmd,
        !ackIsDisabled,
        data[0],
        data[1],
        DFPLAYER_UART_END_BYTE
    };

    serial_->write(buff, (DFPLAYER_UART_FRAME_SIZE - 2));
}

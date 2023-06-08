#include <Arduino.h>
#include "variant.h"


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

 *This is an Arduino library for DFPlayer Mini MP3 module.
 *DFPlayer Mini features:
 *3.2v..5.0v, typical 4.2v.
 *15mA without flash drive, typical 24mA.
 *micro SD-card, up to 32GB (FAT16, FAT32).
 *USB-Disk up to 32GB (FAT16, FAT32).
 *supports mp3 sampling rate 8KHz, 11.025KHz, 12KHz, 16KHz, 22.05KHz, 24KHz, 32KHz, 44.1KHz, 48KHz.
 *supports up to 100 folders, each folder can be assigned to 001..255 songs.
 *built-in 3W mono amplifier, NS8002 AB-Class with standby function.
 *UART to communicate, 9600bps (parity:none, data bits:8, stop bits:1, flow control:none).
 *if you hear a loud noise, add a 1K resistor in series with DFPlayer TX-pin.
 ***************************************************************************************************/



static constexpr auto DFPLAYER_UART_FRAME_SIZE{0x0A};      //total number of bytes in UART packet, same for cmd & feedback
static constexpr auto DFPLAYER_UART_START_BYTE{0x7E};      //start byte
static constexpr auto DFPLAYER_UART_VERSION{0xFF};         //protocol version
static constexpr auto DFPLAYER_UART_DATA_LEN{0x06};        //number of data bytes, except start byte, checksum & end byte
static constexpr auto DFPLAYER_UART_END_BYTE{0xEF};        //end byte

/* command controls */
static constexpr auto DFPLAYER_PLAY_NEXT{0x01};            //play next uploaded file
static constexpr auto DFPLAYER_PLAY_PREV{0x02};            //play prev uploaded file
static constexpr auto DFPLAYER_PLAY_TRACK{0x03};           //play tracks in chronological order, by upload time
static constexpr auto DFPLAYER_SET_VOL_UP{0x04};           //increment volume by 1
static constexpr auto DFPLAYER_SET_VOL_DOWN{0x05};         //decrement volume by 1
static constexpr auto DFPLAYER_SET_VOL{0x06};              //set volume, range 0..30
static constexpr auto DFPLAYER_SET_EQ{0x07};               //0=Off, 1=Pop, 2=Rock, 3=Jazz, 4=Classic, 5=Bass (may not be supported by some modules)
static constexpr auto DFPLAYER_LOOP_TRACK{0x08};           //playing & looping track number 0001..9999
static constexpr auto DFPLAYER_SET_PLAY_SRC{0x09};         //1=USB-Disk, 2=TF-Card, 3=Aux, 4=sleep(YX5200)/NOR-Flash(GD3200), 5=NOR-Flash, 6=Sleep (3..6 may not be supported by some modules)
static constexpr auto DFPLAYER_SET_STANDBY_MODE{0x0A};     //put player in standby mode, not the same as sleep mode
static constexpr auto DFPLAYER_SET_NORMAL_MODE{0x0B};      //pull player out of standby mode (may not be supported by some modules)
static constexpr auto DFPLAYER_RESET{0x0C};                //reset module, set all settings to factory default
static constexpr auto DFPLAYER_RESUME_PLAYBACK{0x0D};      //resume playing current track
static constexpr auto DFPLAYER_PAUSE{0x0E};                //pause playing current track
static constexpr auto DFPLAYER_PLAY_FOLDER{0x0F};          //play track number 1..255 from folder number 1..99
static constexpr auto DFPLAYER_SET_DAC_GAIN{0x10};         //set DAC output gain/output voltage swing (may not be supported by some modules)
static constexpr auto DFPLAYER_REPEAT_ALL{0x11};           //repeat playback all files in chronological order
static constexpr auto DFPLAYER_PLAY_MP3_FOLDER{0x12};      //play track number 0001..9999 from "mp3" folder 
static constexpr auto DFPLAYER_PLAY_ADVERT_FOLDER{0x13};   //interrupt current track & play track number 0001..9999 from "advert" folder, than resume current track
static constexpr auto DFPLAYER_PLAY_3000_FOLDER{0x14};     //play track number 0001..3000 from folder that supports 3000 tracks (may not be supported by some modules)
static constexpr auto DFPLAYER_STOP_ADVERT_FOLDER{0x15};   //stop interrupting current track while playing track from "advert" folder
static constexpr auto DFPLAYER_STOP_PLAYBACK{0x16};        //stop playing current track
static constexpr auto DFPLAYER_REPEAT_FOLDER{0x17};        //repeat playback folder number 01..99
static constexpr auto DFPLAYER_RANDOM_ALL_FILES{0x18};     //play all tracks in random order
static constexpr auto DFPLAYER_LOOP_CURRENT_TRACK{0x19};   //loop currently played track
static constexpr auto DFPLAYER_SET_DAC{0x1A};              //enable/disable DAC (mute/unmute), 0=enable, 1=disable
static constexpr auto DFPLAYER_PLAY_ADVERT_FOLDER_N{0x25}; //interrupt current track & play track number 001..255 from "advert9" folder, than resume current track (may not be supported by some modules)

/* request command controls */
static constexpr auto DFPLAYER_GET_STATUS{0x42};           //get current stutus, see NOTE
static constexpr auto DFPLAYER_GET_VOL{0x43};              //get current volume, range 0..30
static constexpr auto DFPLAYER_GET_EQ{0x44};               //get current EQ, 0=Off, 1=Pop, 2=Rock, 3=Jazz, 4=Classic, 5=Bass (may not be supported by some modules)
static constexpr auto DFPLAYER_GET_PLAY_MODE{0x45};        //get current loop mode, 0=loop all, 1=loop folder, 2=loop track, 3=random, 4=disable (may not be supported by some modules)
static constexpr auto DFPLAYER_GET_VERSION{0x46};          //get software version
static constexpr auto DFPLAYER_GET_QNT_USB_FILES{0x47};    //get total number of tracks on USB-Disk
static constexpr auto DFPLAYER_GET_QNT_TF_FILES{0x48};     //get total number of tracks on TF-card
static constexpr auto DFPLAYER_GET_QNT_FLASH_FILES{0x49};  //get total number of tracks on NOR-Flash (may not be supported by some modules)
static constexpr auto DFPLAYER_GET_USB_TRACK{0x4B};        //get currently playing track number on USB-Disk
static constexpr auto DFPLAYER_GET_TF_TRACK{0x4C};         //get currently playing track number on TF-card
static constexpr auto DFPLAYER_GET_FLASH_TRACK{0x4D};      //get currently playing track number on NOR-Flash (may not be supported by some modules)
static constexpr auto DFPLAYER_GET_QNT_FOLDER_FILES{0x4E}; //get total number of tracks in folder
static constexpr auto DFPLAYER_GET_QNT_FOLDERS{0x4F};      //get total number of folders in current source (may not be supported by some modules)

/* module returned codes at the end of any playback operation or if any command error, located in 4-th RX byte */
static constexpr auto DFPLAYER_RETURN_CODE_OK_ACK{0x41};   //OK, command is accepted (returned only if ACK/feedback byte is set to 0x01)
static constexpr auto DFPLAYER_RETURN_ERROR{0x40};         //error, module return this status automatically if command is not accepted (details located in 7-th RX byte)
static constexpr auto DFPLAYER_RETURN_CODE_DONE{0x3D};     //track playback is is completed, module return this status automatically after the track has been played
static constexpr auto DFPLAYER_RETURN_CODE_READY{0x3F};    //ready after boot or reset, module return this status automatically after boot or reset

/* misc */
static constexpr auto DFPLAYER_BOOT_DELAY{3000};           //average player boot time 1500sec..3000msec, depends on SD-card size
static constexpr auto DFPLAYER_CMD_DELAY{350};             //average read command timeout 200msec..300msec for YX5200/AAxxxx chip & 350msec..500msec for GD3200B/MH2024K chip


class dfPlayer{
public: 
    // The desired baudRate for the dfPlayer
    static constexpr uint16_t DF_PLAYER_BAUD_RATE{9600};
    
    /** 
     * An class enum.
     * This class enum is for reading the dedails from the module.
     */
    enum class dfPlayereDetails_t {
        DF_PlayerIsAvailable,
        SerialError,
        SrialDataIsReceived,
        DF_PlayerTimeOut,
        DF_PlayerWrongStack,
        SD_CardIsInserted,
        SD_CardIsRemoved,
        SD_CardIsAvailable,
        CardNotFound,
        DF_PlayerIsFinished,
        DF_PlayerIsSleeping,
        DF_PlayerSerialWrongStack,
        DF_PlayerFileIndexOut,
        DF_PlayerCheckSumNotMatch,
        FD_PlayerIsAdvertising
    };

    /**
     * @brief A delete copy constructor for preventing copying objects.
     * @param dfPlayer object from the class dfPlyer.
     */
    dfPlayer(const dfPlayer&) = delete;

    /**
     * @brief A delete copy equal operator to prevent copying objects.
     * @param dfPlayer object from the class dfPlyer.
     * @return an object or instance from the class dfPlayer.
     */
    dfPlayer& operator =(const dfPlayer&) = delete;

    /**
     * @brief A function for creating an instance.
     * @return A static object or instance from the class dfPlayer.
     */
    static dfPlayer& dfPlayerGetInstance();

    /**
     * @brief A function for initializing the dfPlayer.
     * @param Uart instance to the serial communication object.
     * @return the state of the communication.
     */
    const dfPlayereDetails_t begin(Uart &);

    /**
     * @brief A funtion to stop the dfPlayer.
     */
    void dfPlayerStop();

    /**
     * @brief A function for ending the serialCommunication and reseting the dfPlayer.
     */
    void dfPlayerEnd();
    
    /**
     * @brief A function for reseting the dfPlayer.
     */
    void dfPlayerReset();

    
    /**
     * @brief A function for selecting the desired source.
     * @param src is the selected source.
     * 1=USB-Disk.
     * 2=TF-Card.
     * 3=Aux.
     * 4=sleep (for YX5200)/NOR-Flash (for GD3200).
     * 5=NOR-Flash.
     * 6=Sleep.
     * source 3..6 may not be supported by some modules.
     * module automatically detect source if source is on-line.
     * module automatically enter standby after setting source.
     * this command interrupt playback!!!
     * wait 200ms to select source. 
     */
    void dfPlayerSetSource(const uint8_t src);

    /**
     * @brief A function for setting the equalizer for the dfPlayer.
     * @param EQ The desired equalizer
     * 0=Off, 1=Pop, 2=Rock, 3=Jazz, 4=Classic, 5=Bass
     */
    void dfPlayerSetEQ(const uint8_t EQ);

    /**
     * @brief A function for setting the volume for the dfPlayer.
     * @param volume the desired volume. 
    */
    void dfPlayerSetVolume(const uint8_t volume);

    // A function for increasing the volume
    /**
     * @brief A function for increasing the volume.
    */
    void dfPlayerVolumeUp();

    /**
     * @brief A function for decreasing the volume.
    */
    void dfPlayerVolumeDown();

    /**
     * @brief A function for getting the volume.
    */
    const int fdPlayerGetVolume();

    /**
     * @brief A function for playing a specifice mp3file
     * @param mp3File the mp3 file to be played
    */
    void dfPlayerPlay(const uint16_t mp3File);

    /**
     * @brief A function for playing the previous mp3File in the folder.
    */
    void dfPlayerPlayPrevious();

    /**
     * @brief A function for playing the next mp3File in the folder.
    */
    void dfPlayerPlayNext();

    /**
     * @brief A function for playing the whole mp3Files in a specifice folder.
     * @param folderName is the name of the folder where the mp3 file.
     * @param mp3file is the mp3 file to be played.
    */
    void dfPlayerPlayFolder(const uint8_t folderName, const uint8_t mp3File);

    /**
     * @brief A function for playing the songs randomly
    */
    void dfPlayerPlayRandom();

    /**
     * @brief A funtion for stop playing the current mp3File.
    */
    void dfPlayerStopPlaying();

    /**
     * @brief A function for pausing the played mp3File.
    */
    void dfPlayerPause();

    /**
     * @brief A function for resume playing the played mp3File.
    */
    void dfPlayerResume();

    /**
     * @brief A function for repeat playing the last cosen mp3File.
    */
    void dfPlayerRepeat(const bool repeat);

    /**
     * @brief A function for repeat play....
     * @param folderName 
    */
    void dfPlayerRepeatFolder(const uint8_t folderName);

    /**
     * @brief A function for making the dfPlayer sleeping.
    */
    void dfPlayerSleep();

    /**
     * @brief A function for waking up the dfPlayer after sleeping.
    */
    void dfPlayerWakeUp();

    /**
     * @brief A helpFunction for sending data and commands to the dfPlayer.
     * @param cmd is the command to be sended to the dfPlayer
     * @param data is an array of data to be sended to the dfPlayer
    */
    void serialSendData(const uint8_t cmd, const uint8_t *data);
    const dfPlayereDetails_t serialReadData(uint8_t *data);


private:
    /**
     * @brief A default constructor
    */
    dfPlayer() = default;

    /**
     * @brief A default destructor
    */
    ~dfPlayer() = default;

    /**
     * @brief An atribute holding the SerialCommunication object
    */
    Uart* serial_;
};
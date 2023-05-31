#include <Arduino.h>
#include "variant.h"


/***************************************************************************************************/
/*
   This is an Arduino library for DFPlayer Mini MP3 module

   written by : Zein Sabbagh

   DFPlayer Mini features:
   - 3.2v..5.0v, typical 4.2v
   - 15mA without flash drive, typical 24mA
   - micro SD-card, up to 32GB (FAT16, FAT32)
   - USB-Disk up to 32GB (FAT16, FAT32)
   - supports mp3 sampling rate 8KHz, 11.025KHz, 12KHz, 16KHz, 22.05KHz, 24KHz, 32KHz, 44.1KHz, 48KHz
   - supports up to 100 folders, each folder can be assigned to 001..255 songs
   - built-in 3W mono amplifier, NS8002 AB-Class with standby function
   - UART to communicate, 9600bps (parity:none, data bits:8, stop bits:1, flow control:none)

   NOTE:
   - if you hear a loud noise, add a 1K resistor in series with DFPlayer TX-pin
*/
/***************************************************************************************************/



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
    
    enum class dfPlayereDetails_t {
        SerialEstablished,
        SerialError,
        DF_PlayerTimeOut,
        DF_PlayerWrongStack,
        SD_CardIsInserted,
        SD_CardIsRemoved,
        SD_CardIsAvailable,
        DF_PlayerIsAvailable,
        CardNotFound,
        DF_PlayerIsFinished,
        DF_PlayerIsSleeping,
        DF_PlayerSerialWrongStack,
        DF_PlayerFileIndexOut,
        DF_PlayerCheckSumNotMatch,
        FD_PlayerIsAdvertising
    };

    // Copy constructor is deleted
    dfPlayer(const dfPlayer&) = delete;
    // Delete equal operator is deleted
    dfPlayer& operator =(const dfPlayer&) = delete;

    // A function for creating an instance
    static dfPlayer& dfPlayerGetInstance();

    // A function for initializing the dfPlayer
    const dfPlayereDetails_t begin(Uart &);

    // A funtion to stop the player
    void dfPlayerStop();

    // A function for ending the serialCommunication and reseting the dfPlayer
    void dfPlayerEnd();

    // A function for reseting the dfPlayer
    void dfPlayerReset();

    
    /*Set playback source

    NOTE:
    - source:
      - 1=USB-Disk
      - 2=TF-Card
      - 3=Aux
      - 4=sleep (for YX5200)/NOR-Flash (for GD3200)
      - 5=NOR-Flash
      - 6=Sleep
    - source 3..6 may not be supported by some modules!!!

    - module automatically detect source if source is on-line
    - module automatically enter standby after setting source
    - this command interrupt playback!!!
    - wait 200ms to select source*/
    void dfPlayerSetSource(const uint8_t src);

    /*Set equalizer

    NOTE:
    - 0=Off, 1=Pop, 2=Rock, 3=Jazz, 4=Classic, 5=Bass
    - feature may not be supported by some modules!!!*/
    void dfPlayerSetEQ(const uint8_t EQ);

    // A function for setting the volume
    void dfPlayerSetVolume(const uint8_t volume);

    // A function for increasing the volume
    void dfPlayerVolumeUp();

    // A function for decreasing the volume
    void dfPlayerVolumeDown();

    // A function for getting the volume
    const int fdPlayerGetVolume();

    // A function for playing a specifice mp3file
    void dfPlayerPlay(const uint16_t mp3File);

    // A function for playing the previous mp3File in the folder
    void dfPlayerPlayPrevious();

    // A function for playing the next mp3File in the folder
    void dfPlayerPlayNext();

    // A function for playing the whole mp3Files in a specifice folder
    void dfPlayerPlayFolder(const uint8_t folderName, const uint8_t mp3File);

    // A funtion for stop playing the current mp3File
    void dfPlayerStopPlaying();

    // A function for pausing the played mp3File
    void dfPlayerPause();

    // A function for resume playing the played mp3File
    void dfPlayerResume();

    // A function for repeat playing the last cosen mp3File
    void dfPlayerRepeat(const bool repeat);

    // A function for repeat play....
    void dfPlayerRepeatFolder(const uint8_t folderName);

    // A function for making the dfPlayer sleeping
    void dfPlayerSleep();

    // A function for waking up the dfPlayer after sleeping 
    void dfPlayerWakeUp();

    // A helpFunction for sending data and commands to the dfPlayer
    void serialSendData(const uint8_t cmd, const uint8_t *data);

private:
    // Default constructor
    dfPlayer() = default;
    // Default destructor
    ~dfPlayer() = default;

    // An atribute holding the SerialCommunication
    Uart* Serial;
};
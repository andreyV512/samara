//  USPC7100_exports.h
//
//  SOCOMATE INTERNATIONAL
//
#pragma once
#define USPC7100_MAX_ROW       100 // Size of parameter's array
#define USPC7100_MAX_HEADER    100 // Size of parameter's array to build header of A-scan

//
// Add extern "C" when this file is included in a C++ code file.
//
#if __cplusplus
extern "C" {
#endif

// Prototypes
ULONG WINAPI USPC7100_Open
    (
    INT boot                                // [in]
    );

ULONG WINAPI USPC7100_Load
    (
    INT Board,                              // [in]
    INT Test,                               // [in]
    LPCSTR file                             // [in]
    );

ULONG WINAPI USPC7100_Write
    (
    INT Board,                              // [in]
    INT Test,                               // [in]
    INT Unit,                               // [in]
    LPCSTR strParam,                        // [in]
    double *dblValue,                       // [in/out]
    double *dblArrayValue1,                 // [in/out]
    double *dblArrayValue2,                 // [in/out]
    LPSTR StrValue,                         // [in/out]
    PINT Clipped                            // [out]
    );

ULONG WINAPI USPC7100_Read
    (
    INT Board,                              // [in]
    INT Test,                               // [in]
    INT Unit,                               // [in]
    LPCSTR strParam,                        // [in]
    double *dblValue,                       // [out]
    double *dblArrayValue1,                 // [out]
    double *dblArrayValue2,                 // [out]
    LPSTR StringValue                       // [out]
    );

ULONG WINAPI USPC7100_Save
    (
    INT Board,                              // [in]
    INT Test,                               // [in]
    LPCSTR file                             // [in]
    );

ULONG WINAPI USPC7100_Close(void);

ULONG WINAPI USPC7100_Get_Ascan
    (
    INT Board,                              // [in]
    INT Test,                               // [in]
    ULONG *A_scan,                          // [out]
    INT Timeout                             // [in]
    );

ULONG WINAPI USPC7100_Get_Ascans
    ( 
    INT Board,                              //  [in]
    INT *NbChannels,                        //  [out]
    ULONG *A_scans,                         //  [out]
    INT Timeout                             //  [in]
    );


ULONG WINAPI USPC7100_Get_number_of_boards(void);
ULONG WINAPI USPC7100_Get_number_of_probes(void);

ULONG WINAPI USPC7100_Get_Serial_Number     
    (                                       // Socomate recommands to use USPC7100_Read(,,,"board_serial_number",,,)
    INT Board,                              // [in]
    INT *SerialNumber                       // [out]
    );

ULONG WINAPI USPC7100_Get_MUX_RCPP_Serial_Number 
    (                                       // Socomate recommands to use USPC7100_Read(,,,"mux_rcpp_serial_number",,,)
    INT Board,                              // [in]
    INT *SerialNumber                       // [out]
    );

BOOL  WINAPI USPC7100_Notify(void);

VOID  WINAPI USPC7100_Get_Flag_Alarm
    (
    INT *Alarm                              // [out]
    );

VOID  WINAPI USPC7100_Get_Alarms
    (
    INT *Alarm1,        // [out]
    INT *Alarm2,        // [out]
    INT *Alarm3         // [out]
    );



void  WINAPI USPC7100_Clear_All_Alarms(void);

ULONG WINAPI USPC7100_Acq_Config
    (
    INT Board,                              // [in]
    ULONG Acq_mode,                         // [in]
    ULONG Start_mode,                       // [in]
    ULONG Alarm[8],                         // [in]
    INT PrePost_trigger,                    // [in]
    INT Discriminant,                       // [in]
    ULONG NumberOfScans,                    // [in]
    INT *Unused,			                // [in/out]
    ULONG *SimplificationParam				// [in]
    );

ULONG WINAPI USPC7100_Acq_Start
    (
    INT Board,                              // [in]
    INT NumberOfScansToAcquire              // [in]
    );

ULONG WINAPI USPC7100_Acq_Start_PA
    (
    INT iProbe                              // [in]
    );

ULONG WINAPI USPC7100_Acq_Stop_PA
    (
    INT iProbe                              // [in]
    );

ULONG WINAPI USPC7100_Acq_Read
    (
    INT Board,                              // [in]
    INT NumberOfScansToRead,                // [in]
    INT TimeOut,                            // [in]
    ULONG *NumberRead,                      // [out]
    ULONG *ScansBacklog,                    // [out]
    UCHAR *pData                            // [out]
    );

ULONG WINAPI USPC7100_Acq_Stop
    (
    INT Board                               // [in]
    );

ULONG WINAPI USPC7100_Acq_Clear
    (
    INT Board                               // [in]
    );

ULONG WINAPI USPC7100_Acq_Get_Status
    (
    INT Board,                              // [in]
    ULONG *Status,                          // [out]
    ULONG *NumberOfScansAcquired,           // [out]
    ULONG *NumberOfScansRead,               // [out]
    ULONG *BufferSize,                      // [out]
    ULONG *BlocSize                         // [out]
    );

VOID WINAPI USPC7100_Build_Header_Ascan
    (
    double Header[USPC7100_MAX_HEADER],     // [in]
                                            // Header[0] = gate1_trigger
                                            // Header[1] = gate1_position (in us)
                                            // Header[2] = gate1_width (in us)
                                            // Header[3] = gate1_level
                                            // Header[4] = gate2_trigger
                                            // Header[5] = gate2_position (in us)
                                            // Header[6] = gate2_width (in us)
                                            // Header[7] = gate2_level
                                            // Header[8] = gateIF_position (in us)
                                            // Header[9] = gateIF_width (in us)
                                            // Header[10] = gateIF_level
                                            // Header[11] = scope_trigger
                                            // Header[12] = scope_offset (in us)
                                            // Header[13] = scope_range (in us)
                                            // Header[14] = gate1_level_alarm
                                            // Header[15] = gate2_level_alarm
    ULONG *pAscan                           // [in/out]
    );

// Set byte alignment from here
#pragma pack(push, previous_pack)
#pragma pack(1)

// Description of the data block returned by USPC7100_Get_Ascan(s)(...)

typedef struct
{
    DWORD G1Begin;              // Begin of gate 1 in ns.
    DWORD G1End;                // Begin of gate 1 in ns.
    DWORD G2Begin;              // Begin of gate 2 in ns.
    DWORD G2End;                // Begin of gate 2 in ns.
    DWORD IfBegin;              // Begin of IF gate in ns.
    DWORD IfEnd;                // Begin of IF gate in [ns].
    BYTE G1Level;               // Gate 1 level in [%].
    BYTE G2Level;               // Gate 2 level in [%].
    BYTE IfLevel;               // IF gate level in [%].
    BYTE G1AlarmFilterLevel: 4; // Gate 1 alarm filter level [dB].
    BYTE G2AlarmFilterLevel: 4; // Gate 2 alarm filter level [dB].
    DWORD AscanBegin;           // Begin of the A-scan in [ns].
    DWORD Spare[6];

    BYTE G1WTlsb;               // WT lsb or image of TOF/WT analog output.
    BYTE G1WTmsb;               // WT msb
    BYTE G1Spare1: 1;           
    BYTE G1Det: 1;              // On/off.
    BYTE G1ThickMin: 1;         // On/off.
    BYTE G1ThickMax: 1;         // On/off.
    BYTE G1WTFormat: 1;         // On = 16 bit format
    BYTE G1Spare2: 3;
    BYTE G1Spare3: 1;
    BYTE G1StartInAscan: 1;
    BYTE G1Spare4: 2;
    BYTE G1EndInAscan: 1;
    BYTE Enable: 1;             // Enable input stat
    BYTE G1Spare5: 2;
    DWORD G1Tof: 24;            // In steps of 5 [ns].
    DWORD G1Amp:8;              // Amplitude in [%]    
    
    BYTE G2WTlsb;               // WT lsb or image of TOF/WT analog output.
    BYTE G2WTmsb;               // WT msb
    BYTE G2Spare1: 1;
    BYTE G2Det: 1;              // On/off.
    BYTE G2ThickMin: 1;         // On/off.
    BYTE G2ThickMax: 1;         // On/off.
    BYTE G2WTFormat: 1;         // On = 16 bit format
    BYTE G2Spare2: 3;
    BYTE G2Spare3: 1;
    BYTE G2StartInAscan: 1;
    BYTE G2Spare4: 2;
    BYTE G2EndInAscan: 1;
    BYTE G2Spare5: 1;
    BYTE GIFStartInAscan: 1;
    BYTE GIFEndInAscan: 1;
    DWORD G2Tof: 24;            // In steps of 5 [ns].
    DWORD G2Amp:8;              // Amplitude in [%]    

    BYTE GIFWTlsb;              // WT lsb or image of TOF/WT analog output.
    BYTE GIFWTmsb;              // WT msb
    BYTE GIFSpare1: 1;
    BYTE GIFDet: 1;             // On/off.
    BYTE GIFSpare2: 2;
    BYTE GIFWTFormat: 1;        // On = 16 bit format
    BYTE GIFThickMin: 1;        // On/off.
    BYTE GIFThickMax: 1;        // On/off.
    BYTE GIFSpare3: 1;
    BYTE GIFSpare4;				
    DWORD GIFTof: 24;           // In steps of 5 [ns].

    DWORD Channel: 8;           // Channel number

	WORD AscanIndex;            // Ascan index of scanning
	WORD Spare1;

    UINT Spare2: 20;
    UINT CycleAlarm: 1;         // cycle alarm
    UINT PrfAlarm: 1;           // PRF alarm    
    UINT Spare3: 7;
    UINT PowerAlarm: 1;         // Power Alarm
    UINT Spare4: 2;
    DWORD Spare5;
    DWORD Spare6;
    WORD DataSize;              // Amount of points in the a-scan.
    WORD Spare7;
    DWORD TimeEqu;              // Time equivalent in [ns].
    BYTE Point[512];            // Ascan data point.
        
} USPC7100_ASCANHEADER;
//
// Data header type for acquisition modes 0x0800 ansd 0x1000.
// Used in ReadData(...) function.
//
typedef struct
{
    DWORD ScanCounter;
    DWORD PulseCounter;

    BYTE G1WTlsb;               // WT lsb or image of TOF/WT analog output.
    BYTE G1WTmsb;               // WT msb
    BYTE G1Spare1: 1;           
    BYTE G1Det: 1;              // On/off.
    BYTE G1ThickMin: 1;         // On/off.
    BYTE G1ThickMax: 1;         // On/off.
    BYTE G1WTFormat: 1;         // On = 16 bit format
    BYTE G1Spare2: 3;
    BYTE G1Spare3: 1;
    BYTE G1StartInAscan: 1;
    BYTE G1Spare4: 2;
    BYTE G1EndInAscan: 1;
    BYTE Enable: 1;             // Enable input stat
    BYTE G1Spare5: 2;
    DWORD G1Tof: 24;            // In steps of 5 [ns].
    DWORD G1Amp:8;              // Amplitude in [%]    
    
    BYTE G2WTlsb;               // WT lsb or image of TOF/WT analog output.
    BYTE G2WTmsb;               // WT msb
    BYTE G2Spare1: 1;
    BYTE G2Det: 1;              // On/off.
    BYTE G2ThickMin: 1;         // On/off.
    BYTE G2ThickMax: 1;         // On/off.
    BYTE G2WTFormat: 1;         // On = 16 bit format
    BYTE G2Spare2: 3;
    BYTE G2Spare3: 1;
    BYTE G2StartInAscan: 1;
    BYTE G2Spare4: 2;
    BYTE G2EndInAscan: 1;
    BYTE G2Spare5: 1;
    BYTE GIFStartInAscan: 1;
    BYTE GIFEndInAscan: 1;
    DWORD G2Tof: 24;            // In steps of 5 [ns].
    DWORD G2Amp:8;              // Amplitude in [%]    

    BYTE GIFWTlsb;              // WT lsb or image of TOF/WT analog output.
    BYTE GIFWTmsb;              // WT msb
    BYTE GIFSpare1: 1;
    BYTE GIFDet: 1;             // On/off.
    BYTE GIFSpare2: 2;
    BYTE GIFWTFormat: 1;        // On = 16 bit format
    BYTE GIFThickMin: 1;        // On/off.
    BYTE GIFThickMax: 1;        // On/off.
    BYTE GIFSpare3: 1;
    BYTE GIFSpare4;		
    DWORD GIFTof: 24;           // In steps of 5 [ns].

    DWORD Channel: 8;           // Channel number
	WORD AscanIndex;            // Ascan index of scanning
	WORD Spare1;
} USPC7100_DATAHEADER;


typedef struct
{
    USPC7100_DATAHEADER hdr;    // Header
    // From here acquisition mode 0x1000 applies.
    UINT Spare: 20;
    UINT CycleAlarm: 1;
    UINT PrfAlarm: 1;
    UINT Spare1: 7;
    UINT PowerAlarm: 1;
    UINT Spare2: 2;
    DWORD Spare3;
    WORD Channel;
    WORD Spare4;
    WORD DataSize;              // Amount of points in the a-scan.
    WORD Spare5;
    DWORD TimeEqu;
    BYTE Point[512];
} USPC7100_ASCANDATAHEADER;

typedef struct
{
    DWORD ScanCounter;   
    DWORD PulseCounter;
    // From here acquisition mode 0x2000 applies.
    WORD DataSize;              // Amount of points in the a-scan.
    BYTE Channel;
    BYTE Spare1: 4;
    BYTE CycleAlarm: 1;
    BYTE PrfAlarm: 1;
    BYTE Spare2: 2;
    DWORD TimeEqu;
    BYTE Point[1];
} USPC7100_ASCANHRDATAHEADER;

// Restore the aligment as it was before.
#pragma pack(pop, previous_pack)

#if __cplusplus
} // extern "C"
#endif

//
// Error codes
//

// Generic errors
#define USPC7100_NO_ERROR                           0
#define USPC7100_ERROR                              1

// Base error codes
#define USPC7100_OPEN_ERROR                         0x10000000
#define USPC7100_CLOSE_ERROR                        0x20000000
#define USPC7100_READ_ERROR                         0x30000000
#define USPC7100_WRITE_ERROR                        0x40000000
#define USPC7100_LOAD_ERROR                         0x50000000
#define USPC7100_SAVE_ERROR                         0x60000000
#define USPC7100_ASCAN_ERROR                        0x70000000
#define USPC7100_ACQ_CONFIG_ERROR                   0x80000000
#define USPC7100_ACQ_START_ERROR                    0x81000000
#define USPC7100_ACQ_READ_ERROR                     0x82000000
#define USPC7100_ACQ_STOP_ERROR                     0x83000000
#define USPC7100_ACQ_CLEAR_ERROR                    0x84000000
#define USPC7100_ACQ_STATUS_ERROR                   0x85000000
#define USPC7100_ACQ_START_PA_ERROR                 0x86000000
#define USPC7100_ACQ_STOP_PA_ERROR					0x87000000
#define USPC7100_DUPLIC_ERROR                       0x90000000
#define USPC7100_ERR_FLASH                          0x91000000

// Additional codes for checking input parameters
#define USPC7100_INVALID_BOARD                      0x100
#define USPC7100_INVALID_CHANNEL                    0x200
#define USPC7100_INVALID_DEVICE                     0x300
#define USPC7100_INVALID_PROBE                      0x400
#define USPC7100_INVALID_SETTING                    0x500
#define USPC7100_INVALID_PARAMETER                  0x600
#define USPC7100_INVALID_BOOT                       0x700
#define USPC7100_INVALID_FILE                       0x800
#define USPC7100_INVALID_FILE_HT                    0x801

// Additional codes for system errors
#define USPC7100_INVALID_HANDLE_ERROR               0x1000
#define USPC7100_EXCEPTION                          0x2000
#define USPC7100_INVALID_POINTER_ERROR              0x3000
#define USPC7100_ALLOC_ERROR                        0x4000

// Additional codes for acquisition
#define USPC7100_ACQ_CONFIG_MODE_ERROR              2
#define USPC7100_ACQ_CONFIG_START_ERROR             3
#define USPC7100_ACQ_CONFIG_ALARM_ERROR             4
#define USPC7100_ACQ_CONFIG_PREPOST_TRIGGER_ERROR   5
#define USPC7100_ACQ_CONFIG_DISCRIMINATOR_ERROR     6
#define USPC7100_ACQ_CONFIG_BUFFER_SIZE_ERROR       7
#define USPC7100_ACQ_CONFIGURED_ERROR               8
#define USPC7100_ACQ_NO_CONFIG_ERROR                0xB 
#define USPC7100_ACQ_RUNNING_ERROR                  0xC
#define USPC7100_ACQ_RUNNING_HR_ERROR               0xD
#define USPC7100_ACQ_INCOMPLET_ERROR                0xE
#define USPC7100_ACQ_TIMEOUT_ERROR                  0xF
#define USPC7100_ACQ_CONFIG_HR_DIGIT_ERROR          0x15
#define USPC7100_ACQ_CONFIG_HR_GATE_ERROR           0x17
#define USPC7100_ACQ_CONFIG_PRF_ERROR               0x18
#define USPC7100_ACQ_CONFIG_ECHO_MAX_ERROR          0x20
#define USPC7100_ACQ_CONFIG_FILTER_ERROR            0x21
#define USPC7100_ACQ_FIFO_OVERFLOW_ERROR            0x22
#define USPC7100_ACQ_SIMPLIFIED_FRAME				0x23

// Additional miscellaneous codes
#define USPC7100_INVALID_SERIAL_NUMBER              0x5000
#define USPC7100_EXTENSION_BOARD_NOT_FOUND          0x5001
#define USPC7100_CONNECT_ERROR                      0x5002
#define USPC7100_INVALID_NUMBER_OF_BOARDS           0x5003
#define USPC7100_INVALID_PACKAGE                    0x5004
#define USPC7100_FILE_PROBE_ERROR                   0x5005
#define USPC7100_FILE_LAW_ERROR                     0x5006
#define USPC7100_INVALID_INDEX                      0x5007
#define USPC7100_FILE_EXTERNAL_ERROR                0x5008
#define USPC7100_NOT_OPEN                           0x5009
#define USPC7100_SET_PARAM_ERROR                    0x500A
#define USPC7100_GET_PARAM_ERROR                    0x500B
#define USPC7100_ALREADY_OPEN                       0x500C
#define USPC7100_ERR_CONVERT                        0x500D
#define USPC7100_BOARD_NOT_FOUND                    0x500E
#define USPC7100_RS_ERROR                           0x500F
#define USPC7100_TOO_MANY_API                       0x5010
#define USPC7100_CHECK_ERROR                        0x5011
#define USPC7100_CLR_SENSIBILITY_ERROR              0x5012
#define USPC7100_SCANNING_ERROR						0x5013
#define USPC7100_EXTEND_ERROR						0x5014
#define USPC7100_NO_TOOL_PARAM						0x5015

// Additional codes for setup
#define USPC7100_INVALID_EXTENSION_BOARD            0x6010
#define USPC7100_MUX_NOT_FOUND                      0x6011
#define USPC7100_MUX_HIGH_NOT_FOUND                 0x6012
#define USPC7100_RESET_ERROR                        0x6013
#define USPC7100_LOAD_FIRMWARE1_ERROR               0x6014
#define USPC7100_LOAD_FIRMWARE2_ERROR               0x6015
#define USPC7100_LOAD_FIRMWARE3_ERROR               0x6016
#define USPC7100_SINGLE_CHANNEL_BOARD_REQUESTED     0x6017
#define USPC7100_MULTI_CHANNELS_BOARD_REQUESTED     0x6018
#define USPC7100_HIGH_VERSION_BOARD_REQUESTED       0x6019
#define USPC7100_DAC_SUPPORT_ERROR                  0x601A
#define USPC7100_INVALID_VERSION					0x601B
#define USPC7100_IO_SETUP							0x601C

#define USPC7100_UNKNOWN_ERROR                      0xFFFFFFFF


// Define unit constants
// Basic units (compatible with USPC3100)
#define UNIT_NONE -1
#define UNIT_us 0
#define UNIT_mm 1
#define UNIT_in 2
// Additional units
// Time units
#define UNIT_ns 100
#define UNIT_ms 101
#define UNIT_s 102
// Distance units
#define UNIT_um 200
#define UNIT_m 201
// Frequency units
#define UNIT_Hz 300 
#define UNIT_kHz 301
#define UNIT_MHz 302
// Velocity units
#define UNIT_mps 400
#define UNIT_inpus 401
// Angle units
#define UNIT_deg 500
#define UNIT_rad 501

//
// Define clipped codes
//
#define NO_CLIP                     0   // Ok
#define NORMAL_CLIP                 1   // Out of range (see documentation)

#define AMP_CLIP                    2   // Pulser amplitude is adjusted to protect emitter
#define FREQ_CLIP                   3   // Pulser frequency is adjusted to protect emitter
#define PRF1_CLIP                   4   // Pulser PRF is adjusted to protect emitter
#define PRF2_CLIP                   5   // Pulser PRF is adjusted to be compatible with gates or Ascan
#define PRF3_CLIP                   6   // PRF is adjusted to be compatible with TOF/WT modes
#define PRF_TRIGGER_CLIP            7   // Trigger is adjusted to be compatible with board mode (MASTER, SLAVE...)
#define FILTER_CLIP                 8   // Receiver filter is adjusted to be compatible with high resolution
#define FILTER_FS_CLIP              9   // Receiver filter is adjusted to be compatible with the list of available filters

#define RANGE_CLIP                  10  // Scope range is adjusted to be compatible with gates or PRF
#define OFFSET_CLIP                 11  // Offset range is adjusted to be compatible with gates or PRF
#define SCOPE_TRIG_CLIP             12  // Scope trigger is adjusted to be compatible with gates or PRF

#define GIF_TRIG_CLIP               20  // Gate IF trigger is adjusted to be compatible with gates or PRF
#define GIF_POS_CLIP                21  // Gate IF position is adjusted to be compatible with gates or PRF
#define GIF_WIDTH_CLIP              22  // Gate IF width is adjusted to be compatible with gates or PRF

#define G1_TRIG_CLIP                30  // Gate 1 trigger is adjusted to be compatible with gates or PRF
#define G1_POS_CLIP                 31  // Gate 1 position is adjusted to be compatible with gates or PRF
#define G1_WIDTH_CLIP               32  // Gate 1 width is adjusted to be compatible with gates or PRF
#define G1_DETECT_CLIP              33  // Gate 1 detection mode is adjusted to be compatible with start mode
#define G1_START_CLIP               34  // Gate 1 start mode is adjusted to be compatible with detection mode
#define G1_TRIG_WT_CLIP             35  // Gate 1 trigger is adjusted to be compatible with IF TOF/WT mode

#define G2_TRIG_CLIP                40  // Gate 2 trigger is adjusted to be compatible with gates or PRF
#define G2_POS_CLIP                 41  // Gate 2 position is adjusted to be compatible with gates or PRF
#define G2_WIDTH_CLIP               42  // Gate 2 width is adjusted to be compatible with gates or PRF
#define G2_DETECT_CLIP              43  // Gate 2 detection mode is adjusted to be compatible with start mode
#define G2_START_CLIP               44  // Gate 2 start mode is adjusted to be compatible with detection mode
#define G2_TRIG_WT_CLIP             45  // Gate 2 trigger is adjusted to be compatible with IF TOF/WT mode

#define DAC_DIST_CLIP               50  // DAC point (distance) is adjusted to keep minimum distance between two consecutive DAC points
#define DAC_SLOPE_CLIP              51  // DAC point (gain) is adjusted to keep maximum slope between two consecutive DAC points
#define DAC_CLIP                    52  // DAC point (distance and gain) are adjusted to keep minimum distance and slope between two consecutive DAC points
#define GDAC_TRIG_CLIP              53  // DAC trigger is adjusted to be compatible with gates or PRF
#define GDAC_POS_CLIP               54  // DAC position  is adjusted to be compatible with gates or PRF
#define GDAC_WIDTH_CLIP             55  // DAC width is adjusted to be compatible with gates or PRF

#define WT_MODE_CLIP                60  // Output process mode is adjusted to be compatible with PRF or board model
#define WT_SOURCE_CLIP				61
#define WT_MODE_G1_SOURCE_CLIP		62
#define WT_MODE_G2_SOURCE_CLIP		63
#define WT_MODE_G1_SYNC_CLIP		64
#define WT_MODE_G2_SYNC_CLIP		65

#define PROBE_FREQ_CLIP             70  // Impossible to load probe file for frequency compatibility reason
#define PROBE_USED_CLIP             71  // Impossible to modify a probe used by pulser (probe file is not compatible with current probe)
#define PROBE_RESO_CLIP             72  // Impossible to modify resolution according to number of laws
#define PROBE_ELEMENT_CLIP          73  // Impossible to load probe file because it has too many elements
#define PROBE_HT_CLIP               74  // Impossible to load probe file because it is not compatible with HT management.
#define PROBE_MULTIBEAM_CLIP        75  // Impossible to modify multibeam parameters if probe is used by pulser 

#define PULSER_SETTING_CLIP         80  // Impossible to undefine (delete) a pulser setting used by a sequence 
#define PULSER_NB_LAWS_CLIP         81  // Number of laws is adjusted to be compatibe 
#define PULSER_MULTIBEAM_CLIP		82	// Impossible to build multi-beam waves
#define PULSER_SPACE_FILTER_CLIP	83	// Calibration mode requests no space filter
#define PULSER_SCAN_MB_CLIP			84  // Impossible to duplicate a pulser setting to scan a multi-beam

#define DETECTION_SETTING_CLIP      90  // Impossible to undefine (delete) a detection setting used by a sequence 
#define DETECTION_SETTING_USED_CLIP 91  // Impossible to modify laws used by one or more sequences 
#define DETECTION_NB_LAWS_CLIP      92  // Number of laws is adjusted to be compatibe with FS/FC
#define DETECTION_LAW_CLIP          93  // The law is not compatible with number of elements
#define DETECTION_SPACE_FILTER_CLIP	94	// Calibration mode requests no space filter
#define DETECTION_SCAN_MB_CLIP		95  // Impossible to duplicate a pulser setting to scan a multi-beam

#define SCANNING_NB_INC_CLIP       100  // The number of gain for scanning is out range
#define RUNNING_MODE_FILTER_CLIP   101	// Calibration mode cannot be used with space filter
#define SEQUENCE_SCAN_MB_CLIP	   102	// Impossible to duplicate a sequence to scan a multi-beam
#define SCANNING_SCAN_MB_CLIP	   103	// Not enough elements to scan a multi-beam
#define VELOCITY_CLIP			   110	// Velocity is adjusted to get parameters compatibles with PRF	
//
// Enum TOF/WT out of range codes
//
enum
{
    IN_RANGE,               // Parameter OK
    GATE_ALARM_MIN_MAX,     // TOF/WT Alarm Min > Max
    GATE_SCALE_OFFSET_RANGE,// TOF/WT scale offset+range are outside of gate
    GATE_SCALE_OFSSET,      // TOF/WT scale offset is outside of gate 
    GATE_SCALE_RANGE,       // TOF/WT scale range is outside of gate 
    GATE_DEVIATION,         // TOF/WT Process deviation is outside of scale
    GATE_ALARM_MIN,         // TOF/WT Alarm Min is outside of scale
    GATE_ALARM_MAX,         // TOF/WT Alarm Max is outside of scale
    GATE_OFFSET,            // TOF/WT Output offset is outside of scale
    GATE_RANGE,             // TOF/WT Output range is outside of scale
    GATE_OFFSET_RANGE,      // TOF/WT Output offset+range are outside of scale
};

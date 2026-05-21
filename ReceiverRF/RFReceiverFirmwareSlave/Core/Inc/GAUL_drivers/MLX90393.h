#ifndef INC_GAUL_DRIVERS_MLX90393_H_
#define INC_GAUL_DRIVERS_MLX90393_H_

/*
 * Based on
 * https://www.melexis.com/en/documents/documentation/datasheets/datasheet-mlx90393
 */

#include <stdint.h>

#if __has_include("stm32f1xx_hal.h")
	#include "stm32f1xx_hal.h"
#elif __has_include("stm32c0xx_hal.h")
	#include "stm32c0xx_hal.h"
#elif __has_include("stm32g4xx_hal.h")
	#include "stm32g4xx_hal.h"
#endif


// I2C last five bits fixed address by manufacturer
#define MLX90393_ADDR_FIXED 0b00011


/* Status Byte ---------------------------------------------------------------*/

// If in Burst measurement mode
#define MLX90393_STATUS_BURST_MODE 1 << 7
// If in Wake-up on change measurement mode
#define MLX90393_STATUS_WOC_MODE 1 << 6
// If in Single measurement mode
#define MLX90393_STATUS_SM_MODE 1 << 5
// If an error eccured. An error can occur if a command was rejected or an
// uncorrectable error is detected in the memory. If the latter is the case, all
// commands but the RT (Reset) command are not allowed.
#define MLX90393_STATUS_ERROR 1 << 4
// Single error detection. On when an error in memory was detected and fixed.
#define MLX90393_STATUS_SED 1 << 3
// Whether the RT is going to be performed. On after calling RT.
#define MLX90393_STATUS_RS 1 << 2
// Only used when issuing RM or RR commands. Specifies how much data to expect.
// The number of response bytes corresponds to 2 * D[1:0] + 2, so the expected
// byte count is either 2, 4, 6 or 8
#define MLX90393_STATUS_D 0b11


/* Memory map ----------------------------------------------------------------*/
// Retrieve a value using (mem[ADDR] >> OFST) & ((1U << LEN) - 1)

#define MLX90393_MEM_OFST_Z_SERIES 0x7
#define MLX90393_MEM_ADDR_Z_SERIES 0x0
#define MLX90393_MEM_LEN_Z_SERIES  0x1

#define MLX90393_MEM_OFST_GAIN_SEL 0x4
#define MLX90393_MEM_ADDR_GAIN_SEL 0x0
#define MLX90393_MEM_LEN_GAIN_SEL  0x3

#define MLX90393_MEM_OFST_HALLCONF 0x0
#define MLX90393_MEM_ADDR_HALLCONF 0x0
#define MLX90393_MEM_LEN_HALLCONF  0x4

#define MLX90393_MEM_OFST_TRIG_INT_SEL 0xF
#define MLX90393_MEM_ADDR_TRIG_INT_SEL 0x1
#define MLX90393_MEM_LEN_TRIG_INT_SEL  0x1

#define MLX90393_MEM_OFST_COMM_MODE 0xD
#define MLX90393_MEM_ADDR_COMM_MODE 0x1
#define MLX90393_MEM_LEN_COMM_MODE  0x2

#define MLX90393_MEM_OFST_WOC_DIFF 0xC
#define MLX90393_MEM_ADDR_WOC_DIFF 0x1
#define MLX90393_MEM_LEN_WOC_DIFF  0x1

#define MLX90393_MEM_OFST_EXT_TRIG 0xB
#define MLX90393_MEM_ADDR_EXT_TRIG 0x1
#define MLX90393_MEM_LEN_EXT_TRIG  0x1

#define MLX90393_MEM_OFST_TCMP_EN 0xA
#define MLX90393_MEM_ADDR_TCMP_EN 0x1
#define MLX90393_MEM_LEN_TCMP_EN  0x1

#define MLX90393_MEM_OFST_BURST_SEL 0x6
#define MLX90393_MEM_ADDR_BURST_SEL 0x1
#define MLX90393_MEM_LEN_BURST_SEL  0x4

#define MLX90393_MEM_OFST_BURST_DATA_RATE 0x0
#define MLX90393_MEM_ADDR_BURST_DATA_RATE 0x1
#define MLX90393_MEM_LEN_BURST_DATA_RATE  0x6

#define MLX90393_MEM_OFST_OSR2 0xB
#define MLX90393_MEM_ADDR_OSR2 0x2
#define MLX90393_MEM_LEN_OSR2  0x2

#define MLX90393_MEM_OFST_RES_XYZ 0x5
#define MLX90393_MEM_ADDR_RES_XYZ 0x2
#define MLX90393_MEM_LEN_RES_XYZ  0x6

#define MLX90393_MEM_OFST_DIG_FILT 0x2
#define MLX90393_MEM_ADDR_DIG_FILT 0x2
#define MLX90393_MEM_LEN_DIG_FILT  0x3

#define MLX90393_MEM_OFST_OSR 0x0
#define MLX90393_MEM_ADDR_OSR 0x2
#define MLX90393_MEM_LEN_OSR  0x2

#define MLX90393_MEM_OFST_SENS_TC_HT 0x8
#define MLX90393_MEM_ADDR_SENS_TC_HT 0x3
#define MLX90393_MEM_LEN_SENS_TC_HT  0x8

#define MLX90393_MEM_OFST_SENS_TC_LT 0x0
#define MLX90393_MEM_ADDR_SENS_TC_LT 0x3
#define MLX90393_MEM_LEN_SENS_TC_LT  0x8

#define MLX90393_MEM_OFST_OFFSET_X 0x0
#define MLX90393_MEM_ADDR_OFFSET_X 0x4
#define MLX90393_MEM_LEN_OFFSET_X  0xF

#define MLX90393_MEM_OFST_OFFSET_Y 0x0
#define MLX90393_MEM_ADDR_OFFSET_Y 0x5
#define MLX90393_MEM_LEN_OFFSET_Y  0xF

#define MLX90393_MEM_OFST_OFFSET_Z 0x0
#define MLX90393_MEM_ADDR_OFFSET_Z 0x6
#define MLX90393_MEM_LEN_OFFSET_Z  0xF

#define MLX90393_MEM_OFST_WOXY_THRESHOLD 0x0
#define MLX90393_MEM_ADDR_WOXY_THRESHOLD 0x6
#define MLX90393_MEM_LEN_WOXY_THRESHOLD  0xF

#define MLX90393_MEM_OFST_WOZ_THRESHOLD 0x0
#define MLX90393_MEM_ADDR_WOZ_THRESHOLD 0x6
#define MLX90393_MEM_LEN_WOZ_THRESHOLD  0xF

#define MLX90393_MEM_OFST_WOT_THRESHOLD 0x0
#define MLX90393_MEM_ADDR_WOT_THRESHOLD 0x6
#define MLX90393_MEM_LEN_WOT_THRESHOLD  0xF

// ... The remaining registers until register 31 is free memory


/* Type Definitions ----------------------------------------------------------*/

/**
 * @brief First four most significant bytes of MLX90393 commands
 *
 * @note Notes are taken directly from the MLX90393 datasheet provided by Melexis.
 */
typedef enum {
  /**
   * @brief Force the IC into idle mode.
   *
   * @note The command will be used to exit the burst mode and the
   * wake-up-on-change mode. During a single measurement in polling mode, the
   * command is rejected. The status byte will then show an error and indicate
   * that the IC is in polling mode (SM_mode).
   */
  MLX90393_CMD_EX = 0b1000,
  /**
   * @brief Start burst measuring mode
   *
   * @note It consists of a single byte with four user-defined bits: z, y, x,
   * and t. These four bits determine which axes will be converted whenever they
   * are set to a 1. Setting an axis to 0 results in the MLX90393 not performing
   * a conversion on that axis and therefore yields a faster conversion cycle
   * and reduced current consumption. When zyxt = 0x0, the SB command will
   * enable the measurements specified by the NVRAM parameter BURST_SEL. The
   * command will only be accepted in case the IC is not operating in another
   * mode (WOC or SM mode). If this is the case, the error flag is raised in the
   * status byte. In case the command is received successfully, the Burst_mode
   * flag of the status byte will be set. After the measurement is complete, the
   * DRDY flag is set (INT pin high). On the start of a new measurement, the
   * DRDY flag is set low again, even if the previous measurement has not been
   * read out.
   */
  MLX90393_CMD_SB = 0b0001,
  /**
   * @brief Start Wake-up on Change mode
   *
   * @note If this command is sent, the WOC mode is entered. The IC will raise
   * the DRDY flag (INT pin) when the thresholds are crossed specified by
   * WOXY_THRESHOLD, WOZ_THRESHOLD and WOT_THRESHOLD. Both a differential mode
   * and absolute mode is available (WOC_DIFF). For the differential mode, each
   * change equal to the threshold after a RM command will be flagged. The x, y,
   * z and t in the command specify which measurements should be done. When zyxt
   * = 0x0, the SWOC command will enable the measurements specified by the NVRAM
   * parameter BURST_SEL. The status byte will show the WOC mode has been
   * entered.
   */
  MLX90393_CMD_SWOC = 0b0010,
  /**
   * @brief Start measurement (polling mode)
   *
   * @note The single measurement command is used to instruct the MLX90393 to
   * perform an acquisition cycle. It consists of a single byte with four
   * user-defined bits: z, y, x, and t. These four bits determine which axes
   * will be converted whenever they are set to a 1. Setting an axis to 0
   * results in the MLX90393 not performing a conversion on that axis and
   * therefore yields a faster conversion cycle and reduced current consumption.
   * When zyxt = 0x0, the SM command will enable the measurements specified by
   * the NVRAM parameter BURST_SEL (like with SB command). The command will only
   * be accepted in case the IC is not operating in another mode (WOC or burst
   * mode). If this is the case, the error flag is raised in the status byte. In
   * case the command is received successfully, the SM_mode flag of the status
   * byte will be set. The next command following this command usually is RM.
   * After the measurement is complete, the DRDY flag is set (INT pin high).
   */
  MLX90393_CMD_SM = 0b0011,
  /**
   * @brief Read measurement
   *
   * @note The Read Measurement command is used to retrieve the data previously
   * acquired by the SM or SB command. Similar to these commands, the RM command
   * can also select which data is transmitted. Typically the value for z, y, x,
   * and t will be the same as the SM command preceding the RM command. The
   * status byte received from the MLX90393 will indicate the number of data
   * bytes waiting to be read out. In the case where all axes and temp are
   * converted the number of bytes will be 8. The data is output in the
   * following order: T (MSB), T (LSB), X (MSB), X (LSB), Y (MSB), Y (LSB), Z
   * (MSB), Z (LSB) If an axis wasn’t selected to be read or converted then that
   * value will be skipped in the transmission.
   */
  MLX90393_CMD_RM = 0b0100,
  /**
   * @brief Read out the content of one specific address of the volatile RAM.
   *
   * @note The address ranges from 0 to 63. Remind that the last 32 addresses
   * are MLX area which cannot be changed by the user. They are read-only. Note
   * that in the command, the address will be sent as if it is multiplied by 4
   * (shifted two bits).
   */
  MLX90393_CMD_RR = 0b0101,
  /**
   * @brief write directly in the volatile RAM
   *
   * @note The address ranges from 0 to 63. Remind that the last 32 addresses
   * are MLX area which cannot be changed by the user. They are read-only. Note
   * that in the command, the address will be sent as if it is multiplied by 4
   * (shifted two bits). Always full words will be written. Words 0x00 to 0x09
   * are used to store operating parameters. Words 0x0A to 0x1F are free and
   * usable to store any other data. Warning: The WR command modifies volatile
   * memory and changes will be lost upon a power cycle. If the modified values
   * are required to be permanent then a memory store (HS) command must be
   * issued after modifying the desired registers.
   */
  MLX90393_CMD_WR = 0b0110,
  /**
   * @brief Reset the IC
   */
  MLX90393_CMD_RT = 0b1111,
  /**
   * @brief Memory Recall
   *
   * @note The content from the non-volatile RAM is overwritten in the volatile
   * RAM. The same is automatically done at start-up.
   */
  MLX90393_CMD_HR = 0b1101,
  /**
   * @brief Memory Store
   *
   * @note The content from the volatile RAM is overwritten in the non-volatile
   * RAM. On the next reset, this content will be loaded into the volatile RAM.
   * Warning: To reliably store the register it is required to supply the IC
   * with 3.3V (VDD)
   */
  MLX90393_CMD_HS = 0b1110,
  /**
   * @brief No Operation
   */
  MLX90393_CMD_NOP = 0b0000,
} MLX90393_Command;

/**
 * @brief MLX90393 I2C Handler with address
 */
typedef struct {
    I2C_HandleTypeDef *hi2c;
    uint8_t address;
} I2C_MLX90393_HandleTypeDef;

/**
 *  @brief MLX90393 Memory Cache
 *
 *  @note Should be updated everytime the WR and RR command are sent. It is
 * invalidated if the device resets.
 */
typedef struct {
  uint16_t memory[32];
  uint32_t cached_registers;
} MLX90393_MemoryCache;


/* I2C Core Methods ----------------------------------------------------------*/

/**
 * @brief Make a `I2C_MLX90393_HandleTypeDef` from two bits (A1 and A0)
 *
 * @param hi2c: I2C handle
 * @param addr: two bits representing A1 and A0
 *
 * @return Initialized `I2C_MLX90393_HandleTypeDef` struct with a seven bit address
 *
 * @note The last five bits of the address are set to `MLX90393_ADDR_FIXED`
 */
I2C_MLX90393_HandleTypeDef I2C_MLX90393_At(
  I2C_HandleTypeDef *hi2c,
  uint8_t addr
);

/**
 * @brief Initializes the LCD.
 *
 * @param hi2cd: Handle and adresss of a MLX90393
 */
HAL_StatusTypeDef I2C_MLX90393_Init(I2C_MLX90393_HandleTypeDef *hi2cd);

/**
 * @brief Transmit a command to the IC 
 *
 * @param hi2cd: Handle and adresss of a MLX90393
 * @param cmd: Command byte to send to the MLX90393
 */
HAL_StatusTypeDef I2C_MLX90393_WriteCommand(
  I2C_MLX90393_HandleTypeDef *hi2cd,
  uint8_t cmd
);

/**
 * @brief Receive bytes from the IC
 *
 * @param hi2cd: Handle and adresss of a MLX90393
 * @param buf: Received data buffer, must have reserved at least `len` bytes
 * @param len: Length of received data in bytes
 */
HAL_StatusTypeDef I2C_MLX90393_Receive(
  I2C_MLX90393_HandleTypeDef *hi2cd,
  uint8_t *buf,
  size_t len
);



/* I2C Convenience Commands  Methods -----------------------------------------*/

/**
 * @brief Convenience method to enter Idle mode
 *
 * @param hi2cd: Handle and adresss of a MLX90393
 * @param status: The status byte
 *
 * @note Internally writes the EX command and reads the status byte.
 */
HAL_StatusTypeDef I2C_MLX90393_Exit(
  I2C_MLX90393_HandleTypeDef *hi2cd,
  uint8_t *status
);

/**
 * @brief Convenience method to enter Burst Measurement mode
 *
 * @param hi2cd: Handle and adresss of a MLX90393
 * @param sel: zyxt selection bits
 * @param status: The status byte
 *
 * @note Internally writes the SB command with `sel` as the last four bits, then
 * reads the status byte.
 */
HAL_StatusTypeDef I2C_MLX90393_StartBurstMode(
  I2C_MLX90393_HandleTypeDef *hi2cd,
  uint8_t sel,
  uint8_t *status
);

/**
 * @brief Convenience method to enter Wake-up On Change Measurement mode
 *
 * @param hi2cd: Handle and adresss of a MLX90393
 * @param sel: zyxt selection bits
 * @param status: The status byte
 *
 * @note Internally writes the SWOC command with `sel` as the last four bits,
 * then reads the status byte.
 */
HAL_StatusTypeDef I2C_MLX90393_StartWakeUpOnChangeMode(
  I2C_MLX90393_HandleTypeDef *hi2cd,
  uint8_t sel,
  uint8_t *status
);

/**
 * @brief Convenience method to enter Polling Measurement mode
 *
 * @param hi2cd: Handle and adresss of a MLX90393
 * @param sel: zyxt selection bits
 * @param status: The status byte
 *
 * @note Internally writes the SM command with `sel` as the last four bits,
 * then reads the status byte.
 */
HAL_StatusTypeDef I2C_MLX90393_StartPollingMode(
  I2C_MLX90393_HandleTypeDef *hi2cd,
  uint8_t sel,
  uint8_t *status
);

/**
 * @brief Convenience method to read a measurement
 *
 * @param hi2cd: Handle and adresss of a MLX90393
 * @param sel: zyxt selection bits
 * @param status: The status byte
 * @param meas: Buffer for the requested measurements
 *
 * @note Internally writes the RM command with `sel` as the last four bits,
 * then reads the status byte and the data.
 */
HAL_StatusTypeDef I2C_MLX90393_ReadMeasurement(
  I2C_MLX90393_HandleTypeDef *hi2cd,
  uint8_t sel,
  uint8_t *status,
  uint8_t *meas
);

/**
 * @brief Convenience method to read from a register
 *
 * @param hi2cd: Handle and adresss of a MLX90393
 * @param cache: The memory cache to use
 * @param reg: The register's five bit address
 * @param status: The status byte (only set on a cache miss)
 *
 * @note Internally writes the RR command, followed by another command write to
 * select the register `reg`, then reads the status byte and the data.
 */
HAL_StatusTypeDef I2C_MLX90393_ReadFromRegister(
  I2C_MLX90393_HandleTypeDef *hi2cd,
  MLX90393_MemoryCache *cache,
  uint8_t reg,
  uint16_t *data,
  uint8_t *status
);

/**
 * @brief Convenience method to write to a register
 *
 * @param hi2cd: Handle and adresss of a MLX90393
 * @param cache: The memory cache to be updated
 * @param reg: The register's five bit address
 * @param status: The status byte
 *
 * @note Internally writes the WR command, followed by two data command bytes
 * and another command write to select the register `reg`, then reads the status
 * byte and the data.
 */
HAL_StatusTypeDef I2C_MLX90393_WriteToRegister(
  I2C_MLX90393_HandleTypeDef *hi2cd,
  MLX90393_MemoryCache *cache,
  uint8_t reg,
  uint16_t data,
  uint8_t *status
);

/**
 * @brief Convenience method to reset the IC
 *
 * @param hi2cd: Handle and adresss of a MLX90393
 * @param status: The status byte
 *
 * @note Internally writes the RT command and reads the status byte.
 */
HAL_StatusTypeDef I2C_MLX90393_Reset(
  I2C_MLX90393_HandleTypeDef *hi2cd,
  uint8_t *status
);

/**
 * @brief Convenience method to recall non-volatile memory to the volatile memory
 *
 * @param hi2cd: Handle and adresss of a MLX90393
 * @param status: The status byte
 *
 * @note Internally writes the HR command and reads the status byte. It's effect
 * is reproduced on startup.
 */
HAL_StatusTypeDef I2C_MLX90393_MemoryRecall(
  I2C_MLX90393_HandleTypeDef *hi2cd,
  uint8_t *status
);

/**
 * @brief Convenience method to store volatile memory to the non volatile memory
 *
 * @param hi2cd: Handle and adresss of a MLX90393
 * @param status: The status byte
 *
 * @note Internally writes the HS command and reads the status byte.
 */
HAL_StatusTypeDef I2C_MLX90393_MemoryStore(
  I2C_MLX90393_HandleTypeDef *hi2cd,
  uint8_t *status
);

/**
 * @brief Convenience method to send a no operation command
 *
 * @param hi2cd: Handle and adresss of a MLX90393
 * @param status: The status byte
 *
 * @note Internally writes the NOP command and reads the status byte.
 */
HAL_StatusTypeDef I2C_MLX90393_Nop(
  I2C_MLX90393_HandleTypeDef *hi2cd,
  uint8_t *status
);

/**
 * @brief Convenience method to read only from a specific range from a register
 *
 * @param hi2cd: Handle and adresss of a MLX90393
 * @param cache: The memory cache to use
 * @param reg: The register address (0-31)
 * @param offset: The offset of the start of the range (0-15)
 * @param len: The length of the range (1-16)
 * @param status: The status byte
 *
 */
HAL_StatusTypeDef I2C_MLX90393_ReadRangeFromRegister(
  I2C_MLX90393_HandleTypeDef *hi2cd,
  MLX90393_MemoryCache *cache,
  uint8_t reg,
  uint8_t offset,
  uint8_t len,
  uint16_t *data,
  uint8_t *status
);

/**
 * @brief Convenience method to write only from a specific range from a register
 *
 * @param hi2cd: Handle and adresss of a MLX90393
 * @param cache: The memory cache to update
 * @param reg: The register address
 * @param offset: The offset of the start of the range
 * @param len: The length of the range
 * @param status: The status byte
 *
 * @note Internally reads the full register first, then write only in the range.
 */
HAL_StatusTypeDef I2C_MLX90393_WriteRangeFromRegister(
  I2C_MLX90393_HandleTypeDef *hi2cd,
  MLX90393_MemoryCache *cache,
  uint8_t reg,
  uint8_t offset,
  uint8_t len,
  uint16_t data,
  uint8_t *status
);


/* Read and Write configuration ----------------------------------------------*/

/**
 * @brief Selection of the gain, adjustable from 0 to 7
 *
 * @note See sensitivity table in the datasheet for sensitivity of the IC
 *       in LSB/G for each setting of the gain and the resolution.
 */
inline HAL_StatusTypeDef I2C_MLX90393_WriteGainSel(
  I2C_MLX90393_HandleTypeDef *hi2cd,
  MLX90393_MemoryCache *cache,
  uint16_t data,
  uint8_t *status
) {
  return I2C_MLX90393_WriteRangeFromRegister(
      hi2cd,
      cache,
      MLX90393_MEM_ADDR_GAIN_SEL,
      MLX90393_MEM_OFST_GAIN_SEL,
      MLX90393_MEM_LEN_GAIN_SEL,
      data,
      status
  );
};

/**
 * @brief Selection of the gain, adjustable from 0 to 7
 *
 * @note See sensitivity table in the datasheet for sensitivity of the IC
 *       in LSB/G for each setting of the gain and the resolution.
 */
inline HAL_StatusTypeDef I2C_MLX90393_ReadGainSel(
  I2C_MLX90393_HandleTypeDef *hi2cd,
  MLX90393_MemoryCache *cache,
  uint16_t *data,
  uint8_t *status
) {
  return I2C_MLX90393_ReadRangeFromRegister(
      hi2cd,
      cache,
      MLX90393_MEM_ADDR_GAIN_SEL,
      MLX90393_MEM_OFST_GAIN_SEL,
      MLX90393_MEM_LEN_GAIN_SEL,
      data,
      status
  );
};

/**
 * @brief Spinning and chopping rate adjustment
 *
 * @note HALLCONF[3:2] defines duration of one spinning phase:
 *       #clocks/spinning_phase = 8 * 2^HALLCONF[3:2].
 *       HALLCONF[1:0] defines number of amplifier chopping periods:
 *       #chopping_periods/spinning_phase = 2^HALLCONF[1:0].
 *       Default HALLCONF = 0xC.
 */
inline HAL_StatusTypeDef I2C_MLX90393_WriteHallConf(
  I2C_MLX90393_HandleTypeDef *hi2cd,
  MLX90393_MemoryCache *cache,
  uint16_t data,
  uint8_t *status
) {
  return I2C_MLX90393_WriteRangeFromRegister(
      hi2cd,
      cache,
      MLX90393_MEM_ADDR_HALLCONF,
      MLX90393_MEM_OFST_HALLCONF,
      MLX90393_MEM_LEN_HALLCONF,
      data,
      status
  );
};

/**
 * @brief Spinning and chopping rate adjustment
 *
 * @note HALLCONF[3:2] defines duration of one spinning phase:
 *       #clocks/spinning_phase = 8 * 2^HALLCONF[3:2].
 *       HALLCONF[1:0] defines number of amplifier chopping periods:
 *       #chopping_periods/spinning_phase = 2^HALLCONF[1:0].
 *       Default HALLCONF = 0xC.
 */
inline HAL_StatusTypeDef I2C_MLX90393_ReadHallConf(
  I2C_MLX90393_HandleTypeDef *hi2cd,
  MLX90393_MemoryCache *cache,
  uint16_t *data,
  uint8_t *status
) {
  return I2C_MLX90393_ReadRangeFromRegister(
      hi2cd,
      cache,
      MLX90393_MEM_ADDR_HALLCONF,
      MLX90393_MEM_OFST_HALLCONF,
      MLX90393_MEM_LEN_HALLCONF,
      data,
      status
  );
};

/**
 * @brief Connection of the Hall plates for Z-axis measurements
 *
 * @note Recommended setting: 0x0.
 */
inline HAL_StatusTypeDef I2C_MLX90393_WriteZSeries(
  I2C_MLX90393_HandleTypeDef *hi2cd,
  MLX90393_MemoryCache *cache,
  uint16_t data,
  uint8_t *status
) {
  return I2C_MLX90393_WriteRangeFromRegister(
      hi2cd,
      cache,
      MLX90393_MEM_ADDR_Z_SERIES,
      MLX90393_MEM_OFST_Z_SERIES,
      MLX90393_MEM_LEN_Z_SERIES,
      data,
      status
  );
};

/**
 * @brief Connection of the Hall plates for Z-axis measurements
 *
 * @note Recommended setting: 0x0.
 */
inline HAL_StatusTypeDef I2C_MLX90393_ReadZSeries(
  I2C_MLX90393_HandleTypeDef *hi2cd,
  MLX90393_MemoryCache *cache,
  uint16_t *data,
  uint8_t *status
) {
  return I2C_MLX90393_ReadRangeFromRegister(
      hi2cd,
      cache,
      MLX90393_MEM_ADDR_Z_SERIES,
      MLX90393_MEM_OFST_Z_SERIES,
      MLX90393_MEM_LEN_Z_SERIES,
      data,
      status
  );
};

/**
 * @brief Controls the data rate in burst and wake-on-change (WOC) modes
 *
 * @note Rate = 20 ms × BURST_DATA_RATE. 0x0 = continuous mode.
 *       Max 0x3F = 1260 ms interval.
 */
inline HAL_StatusTypeDef I2C_MLX90393_WriteBurstDataRate(
  I2C_MLX90393_HandleTypeDef *hi2cd,
  MLX90393_MemoryCache *cache,
  uint16_t data,
  uint8_t *status
) {
  return I2C_MLX90393_WriteRangeFromRegister(
      hi2cd,
      cache,
      MLX90393_MEM_ADDR_BURST_DATA_RATE,
      MLX90393_MEM_OFST_BURST_DATA_RATE,
      MLX90393_MEM_LEN_BURST_DATA_RATE,
      data,
      status
  );
}

/**
 * @brief Controls the data rate in burst and wake-on-change (WOC) modes
 *
 * @note Rate = 20 ms × BURST_DATA_RATE. 0x0 = continuous mode.
 *       Max 0x3F = 1260 ms interval.
 */
inline HAL_StatusTypeDef I2C_MLX90393_ReadBurstDataRate(
  I2C_MLX90393_HandleTypeDef *hi2cd,
  MLX90393_MemoryCache *cache,
  uint16_t *data,
  uint8_t *status
) {
  return I2C_MLX90393_ReadRangeFromRegister(
      hi2cd,
      cache,
      MLX90393_MEM_ADDR_BURST_DATA_RATE,
      MLX90393_MEM_OFST_BURST_DATA_RATE,
      MLX90393_MEM_LEN_BURST_DATA_RATE,
      data,
      status
  );
}

/**
 * @brief Determines which measurements are performed in burst/WOC mode
 *
 * @note Bit order: ZYXT. Specifies which axes and temperature are measured
 *       when SB or SWOC commands omit explicit selection.
 */
inline HAL_StatusTypeDef I2C_MLX90393_WriteBurstSel(
  I2C_MLX90393_HandleTypeDef *hi2cd,
  MLX90393_MemoryCache *cache,
  uint16_t data,
  uint8_t *status
) {
  return I2C_MLX90393_WriteRangeFromRegister(
      hi2cd,
      cache,
      MLX90393_MEM_ADDR_BURST_SEL,
      MLX90393_MEM_OFST_BURST_SEL,
      MLX90393_MEM_LEN_BURST_SEL,
      data,
      status
  );
}

/**
 * @brief Determines which measurements are performed in burst/WOC mode
 *
 * @note Bit order: ZYXT. Specifies which axes and temperature are measured
 *       when SB or SWOC commands omit explicit selection.
 */
inline HAL_StatusTypeDef I2C_MLX90393_ReadBurstSel(
  I2C_MLX90393_HandleTypeDef *hi2cd,
  MLX90393_MemoryCache *cache,
  uint16_t *data,
  uint8_t *status
) {
  return I2C_MLX90393_ReadRangeFromRegister(
      hi2cd,
      cache,
      MLX90393_MEM_ADDR_BURST_SEL,
      MLX90393_MEM_OFST_BURST_SEL,
      MLX90393_MEM_LEN_BURST_SEL,
      data,
      status
  );
}

/**
 * @brief Enables temperature compensation
 *
 * @note See application note for temperature compensation details.
 */
inline HAL_StatusTypeDef I2C_MLX90393_WriteTcmpEn(
  I2C_MLX90393_HandleTypeDef *hi2cd,
  MLX90393_MemoryCache *cache,
  uint16_t data,
  uint8_t *status
) {
  return I2C_MLX90393_WriteRangeFromRegister(
      hi2cd,
      cache,
      MLX90393_MEM_ADDR_TCMP_EN,
      MLX90393_MEM_OFST_TCMP_EN,
      MLX90393_MEM_LEN_TCMP_EN,
      data,
      status
  );
}

/**
 * @brief Enables temperature compensation
 *
 * @note See application note for temperature compensation details.
 */
inline HAL_StatusTypeDef I2C_MLX90393_ReadTcmpEn(
  I2C_MLX90393_HandleTypeDef *hi2cd,
  MLX90393_MemoryCache *cache,
  uint16_t *data,
  uint8_t *status
) {
  return I2C_MLX90393_ReadRangeFromRegister(
      hi2cd,
      cache,
      MLX90393_MEM_ADDR_TCMP_EN,
      MLX90393_MEM_OFST_TCMP_EN,
      MLX90393_MEM_LEN_TCMP_EN,
      data,
      status
  );
}

/**
 * @brief Enables use of INT/TRIG pin as external trigger input
 *
 * @note Only works when TRIG_INT_SEL = 0 (digital input mode).
 *       When set, a pulse on INT/TRIG starts a measurement
 *       defined by BURST_SEL.
 */
inline HAL_StatusTypeDef I2C_MLX90393_WriteExtTrig(
  I2C_MLX90393_HandleTypeDef *hi2cd,
  MLX90393_MemoryCache *cache,
  uint16_t data,
  uint8_t *status
) {
  return I2C_MLX90393_WriteRangeFromRegister(
      hi2cd,
      cache,
      MLX90393_MEM_ADDR_EXT_TRIG,
      MLX90393_MEM_OFST_EXT_TRIG,
      MLX90393_MEM_LEN_EXT_TRIG,
      data,
      status
  );
}

/**
 * @brief Enables use of INT/TRIG pin as external trigger input
 *
 * @note Only works when TRIG_INT_SEL = 0 (digital input mode).
 *       When set, a pulse on INT/TRIG starts a measurement
 *       defined by BURST_SEL.
 */
inline HAL_StatusTypeDef I2C_MLX90393_ReadExtTrig(
  I2C_MLX90393_HandleTypeDef *hi2cd,
  MLX90393_MemoryCache *cache,
  uint16_t *data,
  uint8_t *status
) {
  return I2C_MLX90393_ReadRangeFromRegister(
      hi2cd,
      cache,
      MLX90393_MEM_ADDR_EXT_TRIG,
      MLX90393_MEM_OFST_EXT_TRIG,
      MLX90393_MEM_LEN_EXT_TRIG,
      data,
      status
  );
}

/**
 * @brief Selects absolute or derivative Wake-on-Change mode
 *
 * @note 0 = absolute (compare to first measurement),
 *       1 = relative (compare to previous measurement).
 */
inline HAL_StatusTypeDef I2C_MLX90393_WriteWocDiff(
  I2C_MLX90393_HandleTypeDef *hi2cd,
  MLX90393_MemoryCache *cache,
  uint16_t data,
  uint8_t *status
) {
  return I2C_MLX90393_WriteRangeFromRegister(
      hi2cd,
      cache,
      MLX90393_MEM_ADDR_WOC_DIFF,
      MLX90393_MEM_OFST_WOC_DIFF,
      MLX90393_MEM_LEN_WOC_DIFF,
      data,
      status
  );
}

/**
 * @brief Selects absolute or derivative Wake-on-Change mode
 *
 * @note 0 = absolute (compare to first measurement),
 *       1 = relative (compare to previous measurement).
 */
inline HAL_StatusTypeDef I2C_MLX90393_ReadWocDiff(
  I2C_MLX90393_HandleTypeDef *hi2cd,
  MLX90393_MemoryCache *cache,
  uint16_t *data,
  uint8_t *status
) {
  return I2C_MLX90393_ReadRangeFromRegister(
      hi2cd,
      cache,
      MLX90393_MEM_ADDR_WOC_DIFF,
      MLX90393_MEM_OFST_WOC_DIFF,
      MLX90393_MEM_LEN_WOC_DIFF,
      data,
      status
  );
}

/**
 * @brief Defines the communication mode
 *
 * @note 0x0 / 0x1 = I²C + SPI enabled,
 *       0x2 = SPI only,
 *       0x3 = I²C only.
 */
inline HAL_StatusTypeDef I2C_MLX90393_WriteCommMode(
  I2C_MLX90393_HandleTypeDef *hi2cd,
  MLX90393_MemoryCache *cache,
  uint16_t data,
  uint8_t *status
) {
  return I2C_MLX90393_WriteRangeFromRegister(
      hi2cd,
      cache,
      MLX90393_MEM_ADDR_COMM_MODE,
      MLX90393_MEM_OFST_COMM_MODE,
      MLX90393_MEM_LEN_COMM_MODE,
      data,
      status
  );
}

/**
 * @brief Defines the communication mode
 *
 * @note 0x0 / 0x1 = I²C + SPI enabled,
 *       0x2 = SPI only,
 *       0x3 = I²C only.
 */
inline HAL_StatusTypeDef I2C_MLX90393_ReadCommMode(
  I2C_MLX90393_HandleTypeDef *hi2cd,
  MLX90393_MemoryCache *cache,
  uint16_t *data,
  uint8_t *status
) {
  return I2C_MLX90393_ReadRangeFromRegister(
      hi2cd,
      cache,
      MLX90393_MEM_ADDR_COMM_MODE,
      MLX90393_MEM_OFST_COMM_MODE,
      MLX90393_MEM_LEN_COMM_MODE,
      data,
      status
  );
}

/**
 * @brief Selects INT/TRIG pin function
 *
 * @note When set, the pin is digital output; otherwise it is input.
 */
inline HAL_StatusTypeDef I2C_MLX90393_WriteTrigIntSel(
  I2C_MLX90393_HandleTypeDef *hi2cd,
  MLX90393_MemoryCache *cache,
  uint16_t data,
  uint8_t *status
) {
  return I2C_MLX90393_WriteRangeFromRegister(
      hi2cd,
      cache,
      MLX90393_MEM_ADDR_TRIG_INT_SEL,
      MLX90393_MEM_OFST_TRIG_INT_SEL,
      MLX90393_MEM_LEN_TRIG_INT_SEL,
      data,
      status
  );
}


/**
 * @brief Selects INT/TRIG pin function
 *
 * @note When set, the pin is digital output; otherwise it is input.
 */
inline HAL_StatusTypeDef I2C_MLX90393_ReadTrigIntSel(
  I2C_MLX90393_HandleTypeDef *hi2cd,
  MLX90393_MemoryCache *cache,
  uint16_t *data,
  uint8_t *status
) {
  return I2C_MLX90393_ReadRangeFromRegister(
      hi2cd,
      cache,
      MLX90393_MEM_ADDR_TRIG_INT_SEL,
      MLX90393_MEM_OFST_TRIG_INT_SEL,
      MLX90393_MEM_LEN_TRIG_INT_SEL,
      data,
      status
  );
}

/**
 * @brief Defines the magnetic-measurement ADC oversampling rate
 *
 * @note OSR_ADC = 64 × 2^OSR. Affects measurement time.
 *       Invalid combinations: (OSR=0,DIG_FILT=0/1) and (OSR=1,DIG_FILT=0).
 */
inline HAL_StatusTypeDef I2C_MLX90393_WriteOsr(
  I2C_MLX90393_HandleTypeDef *hi2cd,
  MLX90393_MemoryCache *cache,
  uint16_t data,
  uint8_t *status
) {
  return I2C_MLX90393_WriteRangeFromRegister(
      hi2cd,
      cache,
      MLX90393_MEM_ADDR_OSR,
      MLX90393_MEM_OFST_OSR,
      MLX90393_MEM_LEN_OSR,
      data,
      status
  );
}


/**
 * @brief Defines the magnetic-measurement ADC oversampling rate
 *
 * @note OSR_ADC = 64 × 2^OSR. Affects measurement time.
 *       Invalid combinations: (OSR=0,DIG_FILT=0/1) and (OSR=1,DIG_FILT=0).
 */
inline HAL_StatusTypeDef I2C_MLX90393_ReadOsr(
  I2C_MLX90393_HandleTypeDef *hi2cd,
  MLX90393_MemoryCache *cache,
  uint16_t *data,
  uint8_t *status
) {
  return I2C_MLX90393_ReadRangeFromRegister(
      hi2cd,
      cache,
      MLX90393_MEM_ADDR_OSR,
      MLX90393_MEM_OFST_OSR,
      MLX90393_MEM_LEN_OSR,
      data,
      status
  );
}


/**
 * @brief Controls the digital averaging filter
 *
 * @note Averaging factor = 2^DIG_FILT.
 *       Invalid combinations: (OSR=0,DIG_FILT=0/1) and (OSR=1,DIG_FILT=0).
 */
inline HAL_StatusTypeDef I2C_MLX90393_WriteDigFilt(
  I2C_MLX90393_HandleTypeDef *hi2cd,
  MLX90393_MemoryCache *cache,
  uint16_t data,
  uint8_t *status
) {
  return I2C_MLX90393_WriteRangeFromRegister(
      hi2cd,
      cache,
      MLX90393_MEM_ADDR_DIG_FILT,
      MLX90393_MEM_OFST_DIG_FILT,
      MLX90393_MEM_LEN_DIG_FILT,
      data,
      status
  );
}


/**
 * @brief Controls the digital averaging filter
 *
 * @note Averaging factor = 2^DIG_FILT.
 *       Invalid combinations: (OSR=0,DIG_FILT=0/1) and (OSR=1,DIG_FILT=0).
 */
inline HAL_StatusTypeDef I2C_MLX90393_ReadDigFilt(
  I2C_MLX90393_HandleTypeDef *hi2cd,
  MLX90393_MemoryCache *cache,
  uint16_t *data,
  uint8_t *status
) {
  return I2C_MLX90393_ReadRangeFromRegister(
      hi2cd,
      cache,
      MLX90393_MEM_ADDR_DIG_FILT,
      MLX90393_MEM_OFST_DIG_FILT,
      MLX90393_MEM_LEN_DIG_FILT,
      data,
      status
  );
}

/**
 * @brief Selects which 16 bits of each 19-bit axis measurement are output
 *
 * @note RES_XYZ is 6 bits: 2 per axis (Z Y X).
 */
inline HAL_StatusTypeDef I2C_MLX90393_WriteResXYZ(
  I2C_MLX90393_HandleTypeDef *hi2cd,
  MLX90393_MemoryCache *cache,
  uint16_t data,
  uint8_t *status
) {
  return I2C_MLX90393_WriteRangeFromRegister(
      hi2cd,
      cache,
      MLX90393_MEM_ADDR_RES_XYZ,
      MLX90393_MEM_OFST_RES_XYZ,
      MLX90393_MEM_LEN_RES_XYZ,
      data,
      status
  );
}

/**
 * @brief Selects which 16 bits of each 19-bit axis measurement are output
 *
 * @note RES_XYZ is 6 bits: 2 per axis (Z Y X).
 */
inline HAL_StatusTypeDef I2C_MLX90393_ReadResXYZ(
  I2C_MLX90393_HandleTypeDef *hi2cd,
  MLX90393_MemoryCache *cache,
  uint16_t *data,
  uint8_t *status
) {
  return I2C_MLX90393_ReadRangeFromRegister(
      hi2cd,
      cache,
      MLX90393_MEM_ADDR_RES_XYZ,
      MLX90393_MEM_OFST_RES_XYZ,
      MLX90393_MEM_LEN_RES_XYZ,
      data,
      status
  );
}

/**
 * @brief Defines the temperature-measurement ADC oversampling rate
 *
 * @note OSR_ADC = 64 × 2^OSR2. Affects temperature conversion time.
 */
inline HAL_StatusTypeDef I2C_MLX90393_WriteOsr2(
  I2C_MLX90393_HandleTypeDef *hi2cd,
  MLX90393_MemoryCache *cache,
  uint16_t data,
  uint8_t *status
) {
  return I2C_MLX90393_WriteRangeFromRegister(
      hi2cd,
      cache,
      MLX90393_MEM_ADDR_OSR2,
      MLX90393_MEM_OFST_OSR2,
      MLX90393_MEM_LEN_OSR2,
      data,
      status
  );
}


/**
 * @brief Defines the temperature-measurement ADC oversampling rate
 *
 * @note OSR_ADC = 64 × 2^OSR2. Affects temperature conversion time.
 */
inline HAL_StatusTypeDef I2C_MLX90393_ReadOsr2(
  I2C_MLX90393_HandleTypeDef *hi2cd,
  MLX90393_MemoryCache *cache,
  uint16_t *data,
  uint8_t *status
) {
  return I2C_MLX90393_ReadRangeFromRegister(
      hi2cd,
      cache,
      MLX90393_MEM_ADDR_OSR2,
      MLX90393_MEM_OFST_OSR2,
      MLX90393_MEM_LEN_OSR2,
      data,
      status
  );
}

/**
 * @brief Temperature-compensation coefficient for sensitivity below 35 °C
 *
 * @note Blindly program to value recommended in datasheet if not set.
 */
inline HAL_StatusTypeDef I2C_MLX90393_WriteSensTcLt(
  I2C_MLX90393_HandleTypeDef *hi2cd,
  MLX90393_MemoryCache *cache,
  uint16_t data,
  uint8_t *status
) {
  return I2C_MLX90393_WriteRangeFromRegister(
      hi2cd,
      cache,
      MLX90393_MEM_ADDR_SENS_TC_LT,
      MLX90393_MEM_OFST_SENS_TC_LT,
      MLX90393_MEM_LEN_SENS_TC_LT,
      data,
      status
  );
}

/**
 * @brief Temperature-compensation coefficient for sensitivity below 35 °C
 *
 * @note Blindly program to value recommended in datasheet if not set.
 */
inline HAL_StatusTypeDef I2C_MLX90393_ReadSensTcLt(
  I2C_MLX90393_HandleTypeDef *hi2cd,
  MLX90393_MemoryCache *cache,
  uint16_t *data,
  uint8_t *status
) {
  return I2C_MLX90393_ReadRangeFromRegister(
      hi2cd,
      cache,
      MLX90393_MEM_ADDR_SENS_TC_LT,
      MLX90393_MEM_OFST_SENS_TC_LT,
      MLX90393_MEM_LEN_SENS_TC_LT,
      data,
      status
  );
}

/**
 * @brief Temperature-compensation coefficient for sensitivity above 35 °C
 *
 * @note Blindly program to value recommended in datasheet if not set.
 */
inline HAL_StatusTypeDef I2C_MLX90393_WriteSensTcHt(
  I2C_MLX90393_HandleTypeDef *hi2cd,
  MLX90393_MemoryCache *cache,
  uint16_t data,
  uint8_t *status
) {
  return I2C_MLX90393_WriteRangeFromRegister(
      hi2cd,
      cache,
      MLX90393_MEM_ADDR_SENS_TC_HT,
      MLX90393_MEM_OFST_SENS_TC_HT,
      MLX90393_MEM_LEN_SENS_TC_HT,
      data,
      status
  );
}

/**
 * @brief Temperature-compensation coefficient for sensitivity above 35 °C
 *
 * @note Blindly program to value recommended in datasheet if not set.
 */
inline HAL_StatusTypeDef I2C_MLX90393_ReadSensTcHt(
  I2C_MLX90393_HandleTypeDef *hi2cd,
  MLX90393_MemoryCache *cache,
  uint16_t *data,
  uint8_t *status
) {
  return I2C_MLX90393_ReadRangeFromRegister(
      hi2cd,
      cache,
      MLX90393_MEM_ADDR_SENS_TC_HT,
      MLX90393_MEM_OFST_SENS_TC_HT,
      MLX90393_MEM_LEN_SENS_TC_HT,
      data,
      status
  );
}


/**
 * @brief Offset-compensation parameter for X-axis
 *
 * @note Used only when temperature compensation is enabled.
 */
inline HAL_StatusTypeDef I2C_MLX90393_WriteOffsetX(
  I2C_MLX90393_HandleTypeDef *hi2cd,
  MLX90393_MemoryCache *cache,
  uint16_t data,
  uint8_t *status
) {
  return I2C_MLX90393_WriteRangeFromRegister(
      hi2cd,
      cache,
      MLX90393_MEM_ADDR_OFFSET_X,
      MLX90393_MEM_OFST_OFFSET_X,
      MLX90393_MEM_LEN_OFFSET_X,
      data,
      status
  );
}


/**
 * @brief Offset-compensation parameter for X-axis
 *
 * @note Used only when temperature compensation is enabled.
 */
inline HAL_StatusTypeDef I2C_MLX90393_ReadOffsetX(
  I2C_MLX90393_HandleTypeDef *hi2cd,
  MLX90393_MemoryCache *cache,
  uint16_t *data,
  uint8_t *status
) {
  return I2C_MLX90393_ReadRangeFromRegister(
      hi2cd,
      cache,
      MLX90393_MEM_ADDR_OFFSET_X,
      MLX90393_MEM_OFST_OFFSET_X,
      MLX90393_MEM_LEN_OFFSET_X,
      data,
      status
  );
}

/**
 * @brief Offset-compensation parameter for Y-axis
 *
 * @note Used only when temperature compensation is enabled.
 */
inline HAL_StatusTypeDef I2C_MLX90393_WriteOffsetY(
  I2C_MLX90393_HandleTypeDef *hi2cd,
  MLX90393_MemoryCache *cache,
  uint16_t data,
  uint8_t *status
) {
  return I2C_MLX90393_WriteRangeFromRegister(
      hi2cd,
      cache,
      MLX90393_MEM_ADDR_OFFSET_Y,
      MLX90393_MEM_OFST_OFFSET_Y,
      MLX90393_MEM_LEN_OFFSET_Y,
      data,
      status
  );
}

/**
 * @brief Offset-compensation parameter for Y-axis
 *
 * @note Used only when temperature compensation is enabled.
 */
inline HAL_StatusTypeDef I2C_MLX90393_ReadOffsetY(
  I2C_MLX90393_HandleTypeDef *hi2cd,
  MLX90393_MemoryCache *cache,
  uint16_t *data,
  uint8_t *status
) {
  return I2C_MLX90393_ReadRangeFromRegister(
      hi2cd,
      cache,
      MLX90393_MEM_ADDR_OFFSET_Y,
      MLX90393_MEM_OFST_OFFSET_Y,
      MLX90393_MEM_LEN_OFFSET_Y,
      data,
      status
  );
}

/**
 * @brief Offset-compensation parameter for Z-axis
 *
 * @note Used only when temperature compensation is enabled.
 */
inline HAL_StatusTypeDef I2C_MLX90393_WriteOffsetZ(
  I2C_MLX90393_HandleTypeDef *hi2cd,
  MLX90393_MemoryCache *cache,
  uint16_t data,
  uint8_t *status
) {
  return I2C_MLX90393_WriteRangeFromRegister(
      hi2cd,
      cache,
      MLX90393_MEM_ADDR_OFFSET_Z,
      MLX90393_MEM_OFST_OFFSET_Z,
      MLX90393_MEM_LEN_OFFSET_Z,
      data,
      status
  );
}

/**
 * @brief Offset-compensation parameter for Z-axis
 *
 * @note Used only when temperature compensation is enabled.
 */
inline HAL_StatusTypeDef I2C_MLX90393_ReadOffsetZ(
  I2C_MLX90393_HandleTypeDef *hi2cd,
  MLX90393_MemoryCache *cache,
  uint16_t *data,
  uint8_t *status
) {
  return I2C_MLX90393_ReadRangeFromRegister(
      hi2cd,
      cache,
      MLX90393_MEM_ADDR_OFFSET_Z,
      MLX90393_MEM_OFST_OFFSET_Z,
      MLX90393_MEM_LEN_OFFSET_Z,
      data,
      status
  );
}

/**
 * @brief Wake-up threshold on X/Y axes
 *
 * @note Defines absolute/relative trigger levels for WOC mode.
 */
inline HAL_StatusTypeDef I2C_MLX90393_WriteWoxyThreshold(
  I2C_MLX90393_HandleTypeDef *hi2cd,
  MLX90393_MemoryCache *cache,
  uint16_t data,
  uint8_t *status
) {
  return I2C_MLX90393_WriteRangeFromRegister(
      hi2cd,
      cache,
      MLX90393_MEM_ADDR_WOXY_THRESHOLD,
      MLX90393_MEM_OFST_WOXY_THRESHOLD,
      MLX90393_MEM_LEN_WOXY_THRESHOLD,
      data,
      status
  );
}

/**
 * @brief Wake-up threshold on X/Y axes
 *
 * @note Defines absolute/relative trigger levels for WOC mode.
 */
inline HAL_StatusTypeDef I2C_MLX90393_ReadWoxyThreshold(
  I2C_MLX90393_HandleTypeDef *hi2cd,
  MLX90393_MemoryCache *cache,
  uint16_t *data,
  uint8_t *status
) {
  return I2C_MLX90393_ReadRangeFromRegister(
      hi2cd,
      cache,
      MLX90393_MEM_ADDR_WOXY_THRESHOLD,
      MLX90393_MEM_OFST_WOXY_THRESHOLD,
      MLX90393_MEM_LEN_WOXY_THRESHOLD,
      data,
      status
  );
}

/**
 * @brief Wake-up threshold on Z-axis
 *
 * @note Defines absolute/relative trigger levels for WOC mode.
 */
inline HAL_StatusTypeDef I2C_MLX90393_WriteWozThreshold(
  I2C_MLX90393_HandleTypeDef *hi2cd,
  MLX90393_MemoryCache *cache,
  uint16_t data,
  uint8_t *status
) {
  return I2C_MLX90393_WriteRangeFromRegister(
      hi2cd,
      cache,
      MLX90393_MEM_ADDR_WOZ_THRESHOLD,
      MLX90393_MEM_OFST_WOZ_THRESHOLD,
      MLX90393_MEM_LEN_WOZ_THRESHOLD,
      data,
      status
  );
}

/**
 * @brief Wake-up threshold on Z-axis
 *
 * @note Defines absolute/relative trigger levels for WOC mode.
 */
inline HAL_StatusTypeDef I2C_MLX90393_ReadWozThreshold(
  I2C_MLX90393_HandleTypeDef *hi2cd,
  MLX90393_MemoryCache *cache,
  uint16_t *data,
  uint8_t *status
) {
  return I2C_MLX90393_ReadRangeFromRegister(
      hi2cd,
      cache,
      MLX90393_MEM_ADDR_WOZ_THRESHOLD,
      MLX90393_MEM_OFST_WOZ_THRESHOLD,
      MLX90393_MEM_LEN_WOZ_THRESHOLD,
      data,
      status
  );
}

/**
 * @brief Wake-up threshold on temperature
 *
 * @note Temperature-sensor resolution ≈ 45.2 LSB/K.
 */
inline HAL_StatusTypeDef I2C_MLX90393_WriteWotThreshold(
  I2C_MLX90393_HandleTypeDef *hi2cd,
  MLX90393_MemoryCache *cache,
  uint16_t data,
  uint8_t *status
) {
  return I2C_MLX90393_WriteRangeFromRegister(
      hi2cd,
      cache,
      MLX90393_MEM_ADDR_WOT_THRESHOLD,
      MLX90393_MEM_OFST_WOT_THRESHOLD,
      MLX90393_MEM_LEN_WOT_THRESHOLD,
      data,
      status
  );
}


/**
 * @brief Wake-up threshold on temperature
 *
 * @note Temperature-sensor resolution ≈ 45.2 LSB/K.
 */
inline HAL_StatusTypeDef I2C_MLX90393_ReadWotThreshold(
  I2C_MLX90393_HandleTypeDef *hi2cd,
  MLX90393_MemoryCache *cache,
  uint16_t *data,
  uint8_t *status
) {
  return I2C_MLX90393_ReadRangeFromRegister(
      hi2cd,
      cache,
      MLX90393_MEM_ADDR_WOT_THRESHOLD,
      MLX90393_MEM_OFST_WOT_THRESHOLD,
      MLX90393_MEM_LEN_WOT_THRESHOLD,
      data,
      status
  );
}


#endif /* INC_GAUL_DRIVERS_MLX90393_H_ */

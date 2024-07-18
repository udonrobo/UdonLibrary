/*!
 *  @file Adafruit_BNO055.h
 *
 *  This is a library for the BNO055 orientation sensor
 *
 *  Designed specifically to work with the Adafruit BNO055 Breakout.
 *
 *  Pick one up today in the adafruit shop!
 *  ------> https://www.adafruit.com/product/2472
 *
 *  These sensors use I2C to communicate, 2 pins are required to interface.
 *
 *  Adafruit invests time and resources providing this open source code,
 *  please support Adafruit andopen-source hardware by purchasing products
 *  from Adafruit!
 *
 *  K.Townsend (Adafruit Industries)
 *
 *  MIT license, all text above must be included in any redistribution
 */

#ifndef __ADAFRUIT_BNO055_H__
#define __ADAFRUIT_BNO055_H__

#include "Arduino.h"

#include <limits.h>
#include <math.h>

#include "utility/imumaths.h"
#include <Udon/Thirdparty/Adafruit_BusIO/Adafruit_I2CDevice.h>
#include <Udon/Thirdparty/Adafruit_Sensor/Adafruit_Sensor.h>

/** BNO055 Address A **/
#define BNO055_ADDRESS_A (0x28)
/** BNO055 Address B **/
#define BNO055_ADDRESS_B (0x29)
/** BNO055 ID **/
#define BNO055_ID (0xA0)

/** Offsets registers **/
#define NUM_BNO055_OFFSET_REGISTERS (22)

/** A structure to represent offsets **/
typedef struct {
  int16_t accel_offset_x; /**< x acceleration offset */
  int16_t accel_offset_y; /**< y acceleration offset */
  int16_t accel_offset_z; /**< z acceleration offset */

  int16_t mag_offset_x; /**< x magnetometer offset */
  int16_t mag_offset_y; /**< y magnetometer offset */
  int16_t mag_offset_z; /**< z magnetometer offset */

  int16_t gyro_offset_x; /**< x gyroscrope offset */
  int16_t gyro_offset_y; /**< y gyroscrope offset */
  int16_t gyro_offset_z; /**< z gyroscrope offset */

  int16_t accel_radius; /**< acceleration radius */

  int16_t mag_radius; /**< magnetometer radius */
} adafruit_bno055_offsets_t;

/** Operation mode settings **/
typedef enum {
  OPERATION_MODE_CONFIG = 0X00,
  OPERATION_MODE_ACCONLY = 0X01,
  OPERATION_MODE_MAGONLY = 0X02,
  OPERATION_MODE_GYRONLY = 0X03,
  OPERATION_MODE_ACCMAG = 0X04,
  OPERATION_MODE_ACCGYRO = 0X05,
  OPERATION_MODE_MAGGYRO = 0X06,
  OPERATION_MODE_AMG = 0X07,
  OPERATION_MODE_IMUPLUS = 0X08,
  OPERATION_MODE_COMPASS = 0X09,
  OPERATION_MODE_M4G = 0X0A,
  OPERATION_MODE_NDOF_FMC_OFF = 0X0B,
  OPERATION_MODE_NDOF = 0X0C
} adafruit_bno055_opmode_t;

/*!
 *  @brief  Class that stores state and functions for interacting with
 *          BNO055 Sensor
 */
class Adafruit_BNO055 : public Adafruit_Sensor {
public:
  /** BNO055 Registers **/
  typedef enum {
    /* Page id register definition */
    BNO055_PAGE_ID_ADDR = 0X07,

    /* PAGE0 REGISTER DEFINITION START*/
    BNO055_CHIP_ID_ADDR = 0x00,
    BNO055_ACCEL_REV_ID_ADDR = 0x01,
    BNO055_MAG_REV_ID_ADDR = 0x02,
    BNO055_GYRO_REV_ID_ADDR = 0x03,
    BNO055_SW_REV_ID_LSB_ADDR = 0x04,
    BNO055_SW_REV_ID_MSB_ADDR = 0x05,
    BNO055_BL_REV_ID_ADDR = 0X06,

    /* Accel data register */
    BNO055_ACCEL_DATA_X_LSB_ADDR = 0X08,
    BNO055_ACCEL_DATA_X_MSB_ADDR = 0X09,
    BNO055_ACCEL_DATA_Y_LSB_ADDR = 0X0A,
    BNO055_ACCEL_DATA_Y_MSB_ADDR = 0X0B,
    BNO055_ACCEL_DATA_Z_LSB_ADDR = 0X0C,
    BNO055_ACCEL_DATA_Z_MSB_ADDR = 0X0D,

    /* Mag data register */
    BNO055_MAG_DATA_X_LSB_ADDR = 0X0E,
    BNO055_MAG_DATA_X_MSB_ADDR = 0X0F,
    BNO055_MAG_DATA_Y_LSB_ADDR = 0X10,
    BNO055_MAG_DATA_Y_MSB_ADDR = 0X11,
    BNO055_MAG_DATA_Z_LSB_ADDR = 0X12,
    BNO055_MAG_DATA_Z_MSB_ADDR = 0X13,

    /* Gyro data registers */
    BNO055_GYRO_DATA_X_LSB_ADDR = 0X14,
    BNO055_GYRO_DATA_X_MSB_ADDR = 0X15,
    BNO055_GYRO_DATA_Y_LSB_ADDR = 0X16,
    BNO055_GYRO_DATA_Y_MSB_ADDR = 0X17,
    BNO055_GYRO_DATA_Z_LSB_ADDR = 0X18,
    BNO055_GYRO_DATA_Z_MSB_ADDR = 0X19,

    /* Euler data registers */
    BNO055_EULER_H_LSB_ADDR = 0X1A,
    BNO055_EULER_H_MSB_ADDR = 0X1B,
    BNO055_EULER_R_LSB_ADDR = 0X1C,
    BNO055_EULER_R_MSB_ADDR = 0X1D,
    BNO055_EULER_P_LSB_ADDR = 0X1E,
    BNO055_EULER_P_MSB_ADDR = 0X1F,

    /* Quaternion data registers */
    BNO055_QUATERNION_DATA_W_LSB_ADDR = 0X20,
    BNO055_QUATERNION_DATA_W_MSB_ADDR = 0X21,
    BNO055_QUATERNION_DATA_X_LSB_ADDR = 0X22,
    BNO055_QUATERNION_DATA_X_MSB_ADDR = 0X23,
    BNO055_QUATERNION_DATA_Y_LSB_ADDR = 0X24,
    BNO055_QUATERNION_DATA_Y_MSB_ADDR = 0X25,
    BNO055_QUATERNION_DATA_Z_LSB_ADDR = 0X26,
    BNO055_QUATERNION_DATA_Z_MSB_ADDR = 0X27,

    /* Linear acceleration data registers */
    BNO055_LINEAR_ACCEL_DATA_X_LSB_ADDR = 0X28,
    BNO055_LINEAR_ACCEL_DATA_X_MSB_ADDR = 0X29,
    BNO055_LINEAR_ACCEL_DATA_Y_LSB_ADDR = 0X2A,
    BNO055_LINEAR_ACCEL_DATA_Y_MSB_ADDR = 0X2B,
    BNO055_LINEAR_ACCEL_DATA_Z_LSB_ADDR = 0X2C,
    BNO055_LINEAR_ACCEL_DATA_Z_MSB_ADDR = 0X2D,

    /* Gravity data registers */
    BNO055_GRAVITY_DATA_X_LSB_ADDR = 0X2E,
    BNO055_GRAVITY_DATA_X_MSB_ADDR = 0X2F,
    BNO055_GRAVITY_DATA_Y_LSB_ADDR = 0X30,
    BNO055_GRAVITY_DATA_Y_MSB_ADDR = 0X31,
    BNO055_GRAVITY_DATA_Z_LSB_ADDR = 0X32,
    BNO055_GRAVITY_DATA_Z_MSB_ADDR = 0X33,

    /* Temperature data register */
    BNO055_TEMP_ADDR = 0X34,

    /* Status registers */
    BNO055_CALIB_STAT_ADDR = 0X35,
    BNO055_SELFTEST_RESULT_ADDR = 0X36,
    BNO055_INTR_STAT_ADDR = 0X37,

    BNO055_SYS_CLK_STAT_ADDR = 0X38,
    BNO055_SYS_STAT_ADDR = 0X39,
    BNO055_SYS_ERR_ADDR = 0X3A,

    /* Unit selection register */
    BNO055_UNIT_SEL_ADDR = 0X3B,

    /* Mode registers */
    BNO055_OPR_MODE_ADDR = 0X3D,
    BNO055_PWR_MODE_ADDR = 0X3E,

    BNO055_SYS_TRIGGER_ADDR = 0X3F,
    BNO055_TEMP_SOURCE_ADDR = 0X40,

    /* Axis remap registers */
    BNO055_AXIS_MAP_CONFIG_ADDR = 0X41,
    BNO055_AXIS_MAP_SIGN_ADDR = 0X42,

    /* SIC registers */
    BNO055_SIC_MATRIX_0_LSB_ADDR = 0X43,
    BNO055_SIC_MATRIX_0_MSB_ADDR = 0X44,
    BNO055_SIC_MATRIX_1_LSB_ADDR = 0X45,
    BNO055_SIC_MATRIX_1_MSB_ADDR = 0X46,
    BNO055_SIC_MATRIX_2_LSB_ADDR = 0X47,
    BNO055_SIC_MATRIX_2_MSB_ADDR = 0X48,
    BNO055_SIC_MATRIX_3_LSB_ADDR = 0X49,
    BNO055_SIC_MATRIX_3_MSB_ADDR = 0X4A,
    BNO055_SIC_MATRIX_4_LSB_ADDR = 0X4B,
    BNO055_SIC_MATRIX_4_MSB_ADDR = 0X4C,
    BNO055_SIC_MATRIX_5_LSB_ADDR = 0X4D,
    BNO055_SIC_MATRIX_5_MSB_ADDR = 0X4E,
    BNO055_SIC_MATRIX_6_LSB_ADDR = 0X4F,
    BNO055_SIC_MATRIX_6_MSB_ADDR = 0X50,
    BNO055_SIC_MATRIX_7_LSB_ADDR = 0X51,
    BNO055_SIC_MATRIX_7_MSB_ADDR = 0X52,
    BNO055_SIC_MATRIX_8_LSB_ADDR = 0X53,
    BNO055_SIC_MATRIX_8_MSB_ADDR = 0X54,

    /* Accelerometer Offset registers */
    ACCEL_OFFSET_X_LSB_ADDR = 0X55,
    ACCEL_OFFSET_X_MSB_ADDR = 0X56,
    ACCEL_OFFSET_Y_LSB_ADDR = 0X57,
    ACCEL_OFFSET_Y_MSB_ADDR = 0X58,
    ACCEL_OFFSET_Z_LSB_ADDR = 0X59,
    ACCEL_OFFSET_Z_MSB_ADDR = 0X5A,

    /* Magnetometer Offset registers */
    MAG_OFFSET_X_LSB_ADDR = 0X5B,
    MAG_OFFSET_X_MSB_ADDR = 0X5C,
    MAG_OFFSET_Y_LSB_ADDR = 0X5D,
    MAG_OFFSET_Y_MSB_ADDR = 0X5E,
    MAG_OFFSET_Z_LSB_ADDR = 0X5F,
    MAG_OFFSET_Z_MSB_ADDR = 0X60,

    /* Gyroscope Offset register s*/
    GYRO_OFFSET_X_LSB_ADDR = 0X61,
    GYRO_OFFSET_X_MSB_ADDR = 0X62,
    GYRO_OFFSET_Y_LSB_ADDR = 0X63,
    GYRO_OFFSET_Y_MSB_ADDR = 0X64,
    GYRO_OFFSET_Z_LSB_ADDR = 0X65,
    GYRO_OFFSET_Z_MSB_ADDR = 0X66,

    /* Radius registers */
    ACCEL_RADIUS_LSB_ADDR = 0X67,
    ACCEL_RADIUS_MSB_ADDR = 0X68,
    MAG_RADIUS_LSB_ADDR = 0X69,
    MAG_RADIUS_MSB_ADDR = 0X6A
  } adafruit_bno055_reg_t;

  /** BNO055 power settings */
  typedef enum {
    POWER_MODE_NORMAL = 0X00,
    POWER_MODE_LOWPOWER = 0X01,
    POWER_MODE_SUSPEND = 0X02
  } adafruit_bno055_powermode_t;

  /** Remap settings **/
  typedef enum {
    REMAP_CONFIG_P0 = 0x21,
    REMAP_CONFIG_P1 = 0x24, // default
    REMAP_CONFIG_P2 = 0x24,
    REMAP_CONFIG_P3 = 0x21,
    REMAP_CONFIG_P4 = 0x24,
    REMAP_CONFIG_P5 = 0x21,
    REMAP_CONFIG_P6 = 0x21,
    REMAP_CONFIG_P7 = 0x24
  } adafruit_bno055_axis_remap_config_t;

  /** Remap Signs **/
  typedef enum {
    REMAP_SIGN_P0 = 0x04,
    REMAP_SIGN_P1 = 0x00, // default
    REMAP_SIGN_P2 = 0x06,
    REMAP_SIGN_P3 = 0x02,
    REMAP_SIGN_P4 = 0x03,
    REMAP_SIGN_P5 = 0x01,
    REMAP_SIGN_P6 = 0x07,
    REMAP_SIGN_P7 = 0x05
  } adafruit_bno055_axis_remap_sign_t;

  /** A structure to represent revisions **/
  typedef struct {
    uint8_t accel_rev; /**< acceleration rev */
    uint8_t mag_rev;   /**< magnetometer rev */
    uint8_t gyro_rev;  /**< gyroscrope rev */
    uint16_t sw_rev;   /**< SW rev */
    uint8_t bl_rev;    /**< bootloader rev */
  } adafruit_bno055_rev_info_t;

  /** Vector Mappings **/
  typedef enum {
    VECTOR_ACCELEROMETER = BNO055_ACCEL_DATA_X_LSB_ADDR,
    VECTOR_MAGNETOMETER = BNO055_MAG_DATA_X_LSB_ADDR,
    VECTOR_GYROSCOPE = BNO055_GYRO_DATA_X_LSB_ADDR,
    VECTOR_EULER = BNO055_EULER_H_LSB_ADDR,
    VECTOR_LINEARACCEL = BNO055_LINEAR_ACCEL_DATA_X_LSB_ADDR,
    VECTOR_GRAVITY = BNO055_GRAVITY_DATA_X_LSB_ADDR
  } adafruit_vector_type_t;

  Adafruit_BNO055(int32_t sensorID = -1, uint8_t address = BNO055_ADDRESS_A,
                  TwoWire *theWire = &Wire);

  bool begin(adafruit_bno055_opmode_t mode = OPERATION_MODE_NDOF);
  void setMode(adafruit_bno055_opmode_t mode);
  adafruit_bno055_opmode_t getMode();
  void setAxisRemap(adafruit_bno055_axis_remap_config_t remapcode);
  void setAxisSign(adafruit_bno055_axis_remap_sign_t remapsign);
  void getRevInfo(adafruit_bno055_rev_info_t *);
  void setExtCrystalUse(boolean usextal);
  void getSystemStatus(uint8_t *system_status, uint8_t *self_test_result,
                       uint8_t *system_error);
  void getCalibration(uint8_t *system, uint8_t *gyro, uint8_t *accel,
                      uint8_t *mag);

  imu::Vector<3> getVector(adafruit_vector_type_t vector_type);
  imu::Quaternion getQuat();
  int8_t getTemp();

  /* Adafruit_Sensor implementation */
  bool getEvent(sensors_event_t *);
  bool getEvent(sensors_event_t *, adafruit_vector_type_t);
  void getSensor(sensor_t *);

  /* Functions to deal with raw calibration data */
  bool getSensorOffsets(uint8_t *calibData);
  bool getSensorOffsets(adafruit_bno055_offsets_t &offsets_type);
  void setSensorOffsets(const uint8_t *calibData);
  void setSensorOffsets(const adafruit_bno055_offsets_t &offsets_type);
  bool isFullyCalibrated();

  /* Power managments functions */
  void enterSuspendMode();
  void enterNormalMode();

private:
  byte read8(adafruit_bno055_reg_t);
  bool readLen(adafruit_bno055_reg_t, byte *buffer, uint8_t len);
  bool write8(adafruit_bno055_reg_t, byte value);

  Adafruit_I2CDevice *i2c_dev = NULL; ///< Pointer to I2C bus interface

  int32_t _sensorID;
  adafruit_bno055_opmode_t _mode;
};

/*!
 *  @brief  Instantiates a new Adafruit_BNO055 class
 *  @param  sensorID
 *          sensor ID
 *  @param  address
 *          i2c address
 *  @param  theWire
 *          Wire object
 */
inline Adafruit_BNO055::Adafruit_BNO055(int32_t sensorID, uint8_t address,
                                 TwoWire *theWire) {
// BNO055 clock stretches for 500us or more!
#ifdef ESP8266
  theWire->setClockStretchLimit(1000); // Allow for 1000us of clock stretching
#endif

  _sensorID = sensorID;
  i2c_dev = new Adafruit_I2CDevice(address, theWire);
}

/*!
 *  @brief  Sets up the HW
 *  @param  mode
 *          mode values
 *           [OPERATION_MODE_CONFIG,
 *            OPERATION_MODE_ACCONLY,
 *            OPERATION_MODE_MAGONLY,
 *            OPERATION_MODE_GYRONLY,
 *            OPERATION_MODE_ACCMAG,
 *            OPERATION_MODE_ACCGYRO,
 *            OPERATION_MODE_MAGGYRO,
 *            OPERATION_MODE_AMG,
 *            OPERATION_MODE_IMUPLUS,
 *            OPERATION_MODE_COMPASS,
 *            OPERATION_MODE_M4G,
 *            OPERATION_MODE_NDOF_FMC_OFF,
 *            OPERATION_MODE_NDOF]
 *  @return true if process is successful
 */
inline bool Adafruit_BNO055::begin(adafruit_bno055_opmode_t mode) {
  // Start without a detection
  i2c_dev->begin(false);

#if defined(TARGET_RP2040)
  // philhower core seems to work with this speed?
  i2c_dev->setSpeed(50000);
#endif

  // can take 850 ms to boot!
  int timeout = 850; // in ms
  while (timeout > 0) {
    if (i2c_dev->begin()) {
      break;
    }
    // wasnt detected... we'll retry!
    delay(10);
    timeout -= 10;
  }
  if (timeout <= 0)
    return false;

  /* Make sure we have the right device */
  uint8_t id = read8(BNO055_CHIP_ID_ADDR);
  if (id != BNO055_ID) {
    delay(1000); // hold on for boot
    id = read8(BNO055_CHIP_ID_ADDR);
    if (id != BNO055_ID) {
      return false; // still not? ok bail
    }
  }

  /* Switch to config mode (just in case since this is the default) */
  setMode(OPERATION_MODE_CONFIG);

  /* Reset */
  write8(BNO055_SYS_TRIGGER_ADDR, 0x20);
  /* Delay incrased to 30ms due to power issues https://tinyurl.com/y375z699 */
  delay(30);
  while (read8(BNO055_CHIP_ID_ADDR) != BNO055_ID) {
    delay(10);
  }
  delay(50);

  /* Set to normal power mode */
  write8(BNO055_PWR_MODE_ADDR, POWER_MODE_NORMAL);
  delay(10);

  write8(BNO055_PAGE_ID_ADDR, 0);

  /* Set the output units */
  /*
  uint8_t unitsel = (0 << 7) | // Orientation = Android
                    (0 << 4) | // Temperature = Celsius
                    (0 << 2) | // Euler = Degrees
                    (1 << 1) | // Gyro = Rads
                    (0 << 0);  // Accelerometer = m/s^2
  write8(BNO055_UNIT_SEL_ADDR, unitsel);
  */

  /* Configure axis mapping (see section 3.4) */
  /*
  write8(BNO055_AXIS_MAP_CONFIG_ADDR, REMAP_CONFIG_P2); // P0-P7, Default is P1
  delay(10);
  write8(BNO055_AXIS_MAP_SIGN_ADDR, REMAP_SIGN_P2); // P0-P7, Default is P1
  delay(10);
  */

  write8(BNO055_SYS_TRIGGER_ADDR, 0x0);
  delay(10);
  /* Set the requested operating mode (see section 3.3) */
  setMode(mode);
  delay(20);

  return true;
}

/*!
 *  @brief  Puts the chip in the specified operating mode
 *  @param  mode
 *          mode values
 *           [OPERATION_MODE_CONFIG,
 *            OPERATION_MODE_ACCONLY,
 *            OPERATION_MODE_MAGONLY,
 *            OPERATION_MODE_GYRONLY,
 *            OPERATION_MODE_ACCMAG,
 *            OPERATION_MODE_ACCGYRO,
 *            OPERATION_MODE_MAGGYRO,
 *            OPERATION_MODE_AMG,
 *            OPERATION_MODE_IMUPLUS,
 *            OPERATION_MODE_COMPASS,
 *            OPERATION_MODE_M4G,
 *            OPERATION_MODE_NDOF_FMC_OFF,
 *            OPERATION_MODE_NDOF]
 */
inline void Adafruit_BNO055::setMode(adafruit_bno055_opmode_t mode) {
  _mode = mode;
  write8(BNO055_OPR_MODE_ADDR, _mode);
  delay(30);
}

/*!
 *  @brief  Gets the current operating mode of the chip
 *  @return  operating_mode in integer which can be mapped in Section 3.3
 *           for example: a return of 12 (0X0C) => NDOF
 */
inline adafruit_bno055_opmode_t Adafruit_BNO055::getMode() {
  return (adafruit_bno055_opmode_t)read8(BNO055_OPR_MODE_ADDR);
}

/*!
 *  @brief  Changes the chip's axis remap
 *  @param  remapcode
 *          remap code possible values
 *          [REMAP_CONFIG_P0
 *           REMAP_CONFIG_P1 (default)
 *           REMAP_CONFIG_P2
 *           REMAP_CONFIG_P3
 *           REMAP_CONFIG_P4
 *           REMAP_CONFIG_P5
 *           REMAP_CONFIG_P6
 *           REMAP_CONFIG_P7]
 */
inline void Adafruit_BNO055::setAxisRemap(
    adafruit_bno055_axis_remap_config_t remapcode) {
  adafruit_bno055_opmode_t modeback = _mode;

  setMode(OPERATION_MODE_CONFIG);
  delay(25);
  write8(BNO055_AXIS_MAP_CONFIG_ADDR, remapcode);
  delay(10);
  /* Set the requested operating mode (see section 3.3) */
  setMode(modeback);
  delay(20);
}

/*!
 *  @brief  Changes the chip's axis signs
 *  @param  remapsign
 *          remap sign possible values
 *          [REMAP_SIGN_P0
 *           REMAP_SIGN_P1 (default)
 *           REMAP_SIGN_P2
 *           REMAP_SIGN_P3
 *           REMAP_SIGN_P4
 *           REMAP_SIGN_P5
 *           REMAP_SIGN_P6
 *           REMAP_SIGN_P7]
 */
inline void Adafruit_BNO055::setAxisSign(adafruit_bno055_axis_remap_sign_t remapsign) {
  adafruit_bno055_opmode_t modeback = _mode;

  setMode(OPERATION_MODE_CONFIG);
  delay(25);
  write8(BNO055_AXIS_MAP_SIGN_ADDR, remapsign);
  delay(10);
  /* Set the requested operating mode (see section 3.3) */
  setMode(modeback);
  delay(20);
}

/*!
 *  @brief  Use the external 32.768KHz crystal
 *  @param  usextal
 *          use external crystal boolean
 */
inline void Adafruit_BNO055::setExtCrystalUse(boolean usextal) {
  adafruit_bno055_opmode_t modeback = _mode;

  /* Switch to config mode (just in case since this is the default) */
  setMode(OPERATION_MODE_CONFIG);
  delay(25);
  write8(BNO055_PAGE_ID_ADDR, 0);
  if (usextal) {
    write8(BNO055_SYS_TRIGGER_ADDR, 0x80);
  } else {
    write8(BNO055_SYS_TRIGGER_ADDR, 0x00);
  }
  delay(10);
  /* Set the requested operating mode (see section 3.3) */
  setMode(modeback);
  delay(20);
}

/*!
 *   @brief  Gets the latest system status info
 *   @param  system_status
 *           system status info
 *   @param  self_test_result
 *           self test result
 *   @param  system_error
 *           system error info
 */
inline void Adafruit_BNO055::getSystemStatus(uint8_t *system_status,
                                      uint8_t *self_test_result,
                                      uint8_t *system_error) {
  write8(BNO055_PAGE_ID_ADDR, 0);

  /* System Status (see section 4.3.58)
     0 = Idle
     1 = System Error
     2 = Initializing Peripherals
     3 = System Iniitalization
     4 = Executing Self-Test
     5 = Sensor fusio algorithm running
     6 = System running without fusion algorithms
   */

  if (system_status != 0)
    *system_status = read8(BNO055_SYS_STAT_ADDR);

  /* Self Test Results
     1 = test passed, 0 = test failed

     Bit 0 = Accelerometer self test
     Bit 1 = Magnetometer self test
     Bit 2 = Gyroscope self test
     Bit 3 = MCU self test

     0x0F = all good!
   */

  if (self_test_result != 0)
    *self_test_result = read8(BNO055_SELFTEST_RESULT_ADDR);

  /* System Error (see section 4.3.59)
     0 = No error
     1 = Peripheral initialization error
     2 = System initialization error
     3 = Self test result failed
     4 = Register map value out of range
     5 = Register map address out of range
     6 = Register map write error
     7 = BNO low power mode not available for selected operat ion mode
     8 = Accelerometer power mode not available
     9 = Fusion algorithm configuration error
     A = Sensor configuration error
   */

  if (system_error != 0)
    *system_error = read8(BNO055_SYS_ERR_ADDR);

  delay(200);
}

/*!
 *  @brief  Gets the chip revision numbers
 *  @param  info
 *          revision info
 */
inline void Adafruit_BNO055::getRevInfo(adafruit_bno055_rev_info_t *info) {
  uint8_t a, b;

  memset(info, 0, sizeof(adafruit_bno055_rev_info_t));

  /* Check the accelerometer revision */
  info->accel_rev = read8(BNO055_ACCEL_REV_ID_ADDR);

  /* Check the magnetometer revision */
  info->mag_rev = read8(BNO055_MAG_REV_ID_ADDR);

  /* Check the gyroscope revision */
  info->gyro_rev = read8(BNO055_GYRO_REV_ID_ADDR);

  /* Check the SW revision */
  info->bl_rev = read8(BNO055_BL_REV_ID_ADDR);

  a = read8(BNO055_SW_REV_ID_LSB_ADDR);
  b = read8(BNO055_SW_REV_ID_MSB_ADDR);
  info->sw_rev = (((uint16_t)b) << 8) | ((uint16_t)a);
}

/*!
 *  @brief  Gets current calibration state.  Each value should be a uint8_t
 *          pointer and it will be set to 0 if not calibrated and 3 if
 *          fully calibrated.
 *          See section 34.3.54
 *  @param  sys
 *          Current system calibration status, depends on status of all sensors,
 * read-only
 *  @param  gyro
 *          Current calibration status of Gyroscope, read-only
 *  @param  accel
 *          Current calibration status of Accelerometer, read-only
 *  @param  mag
 *          Current calibration status of Magnetometer, read-only
 */
inline void Adafruit_BNO055::getCalibration(uint8_t *sys, uint8_t *gyro,
                                     uint8_t *accel, uint8_t *mag) {
  uint8_t calData = read8(BNO055_CALIB_STAT_ADDR);
  if (sys != NULL) {
    *sys = (calData >> 6) & 0x03;
  }
  if (gyro != NULL) {
    *gyro = (calData >> 4) & 0x03;
  }
  if (accel != NULL) {
    *accel = (calData >> 2) & 0x03;
  }
  if (mag != NULL) {
    *mag = calData & 0x03;
  }
}

/*!
 *  @brief  Gets the temperature in degrees celsius
 *  @return temperature in degrees celsius
 */
inline int8_t Adafruit_BNO055::getTemp() {
  int8_t temp = (int8_t)(read8(BNO055_TEMP_ADDR));
  return temp;
}

/*!
 *  @brief   Gets a vector reading from the specified source
 *  @param   vector_type
 *           possible vector type values
 *           [VECTOR_ACCELEROMETER
 *            VECTOR_MAGNETOMETER
 *            VECTOR_GYROSCOPE
 *            VECTOR_EULER
 *            VECTOR_LINEARACCEL
 *            VECTOR_GRAVITY]
 *  @return  vector from specified source
 */
inline imu::Vector<3> Adafruit_BNO055::getVector(adafruit_vector_type_t vector_type) {
  imu::Vector<3> xyz;
  uint8_t buffer[6];
  memset(buffer, 0, 6);

  int16_t x, y, z;
  x = y = z = 0;

  /* Read vector data (6 bytes) */
  readLen((adafruit_bno055_reg_t)vector_type, buffer, 6);

  x = ((int16_t)buffer[0]) | (((int16_t)buffer[1]) << 8);
  y = ((int16_t)buffer[2]) | (((int16_t)buffer[3]) << 8);
  z = ((int16_t)buffer[4]) | (((int16_t)buffer[5]) << 8);

  /*!
   * Convert the value to an appropriate range (section 3.6.4)
   * and assign the value to the Vector type
   */
  switch (vector_type) {
  case VECTOR_MAGNETOMETER:
    /* 1uT = 16 LSB */
    xyz[0] = ((double)x) / 16.0;
    xyz[1] = ((double)y) / 16.0;
    xyz[2] = ((double)z) / 16.0;
    break;
  case VECTOR_GYROSCOPE:
    /* 1dps = 16 LSB */
    xyz[0] = ((double)x) / 16.0;
    xyz[1] = ((double)y) / 16.0;
    xyz[2] = ((double)z) / 16.0;
    break;
  case VECTOR_EULER:
    /* 1 degree = 16 LSB */
    xyz[0] = ((double)x) / 16.0;
    xyz[1] = ((double)y) / 16.0;
    xyz[2] = ((double)z) / 16.0;
    break;
  case VECTOR_ACCELEROMETER:
    /* 1m/s^2 = 100 LSB */
    xyz[0] = ((double)x) / 100.0;
    xyz[1] = ((double)y) / 100.0;
    xyz[2] = ((double)z) / 100.0;
    break;
  case VECTOR_LINEARACCEL:
    /* 1m/s^2 = 100 LSB */
    xyz[0] = ((double)x) / 100.0;
    xyz[1] = ((double)y) / 100.0;
    xyz[2] = ((double)z) / 100.0;
    break;
  case VECTOR_GRAVITY:
    /* 1m/s^2 = 100 LSB */
    xyz[0] = ((double)x) / 100.0;
    xyz[1] = ((double)y) / 100.0;
    xyz[2] = ((double)z) / 100.0;
    break;
  }

  return xyz;
}

/*!
 *  @brief  Gets a quaternion reading from the specified source
 *  @return quaternion reading
 */
inline imu::Quaternion Adafruit_BNO055::getQuat() {
  uint8_t buffer[8];
  memset(buffer, 0, 8);

  int16_t x, y, z, w;
  x = y = z = w = 0;

  /* Read quat data (8 bytes) */
  readLen(BNO055_QUATERNION_DATA_W_LSB_ADDR, buffer, 8);
  w = (((uint16_t)buffer[1]) << 8) | ((uint16_t)buffer[0]);
  x = (((uint16_t)buffer[3]) << 8) | ((uint16_t)buffer[2]);
  y = (((uint16_t)buffer[5]) << 8) | ((uint16_t)buffer[4]);
  z = (((uint16_t)buffer[7]) << 8) | ((uint16_t)buffer[6]);

  /*!
   * Assign to Quaternion
   * See
   * https://cdn-shop.adafruit.com/datasheets/BST_BNO055_DS000_12.pdf
   * 3.6.5.5 Orientation (Quaternion)
   */
  const double scale = (1.0 / (1 << 14));
  imu::Quaternion quat(scale * w, scale * x, scale * y, scale * z);
  return quat;
}

/*!
 *  @brief  Provides the sensor_t data for this sensor
 *  @param  sensor
 *          Sensor description
 */
inline void Adafruit_BNO055::getSensor(sensor_t *sensor) {
  /* Clear the sensor_t object */
  memset(sensor, 0, sizeof(sensor_t));

  /* Insert the sensor name in the fixed length char array */
  strncpy(sensor->name, "BNO055", sizeof(sensor->name) - 1);
  sensor->name[sizeof(sensor->name) - 1] = 0;
  sensor->version = 1;
  sensor->sensor_id = _sensorID;
  sensor->type = SENSOR_TYPE_ORIENTATION;
  sensor->min_delay = 0;
  sensor->max_value = 0.0F;
  sensor->min_value = 0.0F;
  sensor->resolution = 0.01F;
}

/*!
 *  @brief  Reads the sensor and returns the data as a sensors_event_t
 *  @param  event
 *          Event description
 *  @return always returns true
 */
inline bool Adafruit_BNO055::getEvent(sensors_event_t *event) {
  /* Clear the event */
  memset(event, 0, sizeof(sensors_event_t));

  event->version = sizeof(sensors_event_t);
  event->sensor_id = _sensorID;
  event->type = SENSOR_TYPE_ORIENTATION;
  event->timestamp = millis();

  /* Get a Euler angle sample for orientation */
  imu::Vector<3> euler = getVector(Adafruit_BNO055::VECTOR_EULER);
  event->orientation.x = euler.x();
  event->orientation.y = euler.y();
  event->orientation.z = euler.z();

  return true;
}

/*!
 *  @brief  Reads the sensor and returns the data as a sensors_event_t
 *  @param  event
 *          Event description
 *  @param  vec_type
 *          specify the type of reading
 *  @return always returns true
 */
inline bool Adafruit_BNO055::getEvent(sensors_event_t *event,
                               adafruit_vector_type_t vec_type) {
  /* Clear the event */
  memset(event, 0, sizeof(sensors_event_t));

  event->version = sizeof(sensors_event_t);
  event->sensor_id = _sensorID;
  event->timestamp = millis();

  // read the data according to vec_type
  imu::Vector<3> vec;
  if (vec_type == Adafruit_BNO055::VECTOR_LINEARACCEL) {
    event->type = SENSOR_TYPE_LINEAR_ACCELERATION;
    vec = getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);

    event->acceleration.x = vec.x();
    event->acceleration.y = vec.y();
    event->acceleration.z = vec.z();
  } else if (vec_type == Adafruit_BNO055::VECTOR_ACCELEROMETER) {
    event->type = SENSOR_TYPE_ACCELEROMETER;
    vec = getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);

    event->acceleration.x = vec.x();
    event->acceleration.y = vec.y();
    event->acceleration.z = vec.z();
  } else if (vec_type == Adafruit_BNO055::VECTOR_GRAVITY) {
    event->type = SENSOR_TYPE_GRAVITY;
    vec = getVector(Adafruit_BNO055::VECTOR_GRAVITY);

    event->acceleration.x = vec.x();
    event->acceleration.y = vec.y();
    event->acceleration.z = vec.z();
  } else if (vec_type == Adafruit_BNO055::VECTOR_EULER) {
    event->type = SENSOR_TYPE_ORIENTATION;
    vec = getVector(Adafruit_BNO055::VECTOR_EULER);

    event->orientation.x = vec.x();
    event->orientation.y = vec.y();
    event->orientation.z = vec.z();
  } else if (vec_type == Adafruit_BNO055::VECTOR_GYROSCOPE) {
    event->type = SENSOR_TYPE_GYROSCOPE;
    vec = getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);

    event->gyro.x = vec.x() * SENSORS_DPS_TO_RADS;
    event->gyro.y = vec.y() * SENSORS_DPS_TO_RADS;
    event->gyro.z = vec.z() * SENSORS_DPS_TO_RADS;
  } else if (vec_type == Adafruit_BNO055::VECTOR_MAGNETOMETER) {
    event->type = SENSOR_TYPE_MAGNETIC_FIELD;
    vec = getVector(Adafruit_BNO055::VECTOR_MAGNETOMETER);

    event->magnetic.x = vec.x();
    event->magnetic.y = vec.y();
    event->magnetic.z = vec.z();
  }

  return true;
}

/*!
 *  @brief  Reads the sensor's offset registers into a byte array
 *  @param  calibData
 *          Calibration offset (buffer size should be 22)
 *  @return true if read is successful
 */
inline bool Adafruit_BNO055::getSensorOffsets(uint8_t *calibData) {
  if (isFullyCalibrated()) {
    adafruit_bno055_opmode_t lastMode = _mode;
    setMode(OPERATION_MODE_CONFIG);

    readLen(ACCEL_OFFSET_X_LSB_ADDR, calibData, NUM_BNO055_OFFSET_REGISTERS);

    setMode(lastMode);
    return true;
  }
  return false;
}

/*!
 *  @brief  Reads the sensor's offset registers into an offset struct
 *  @param  offsets_type
 *          type of offsets
 *  @return true if read is successful
 */
inline bool Adafruit_BNO055::getSensorOffsets(
    adafruit_bno055_offsets_t &offsets_type) {
  if (isFullyCalibrated()) {
    adafruit_bno055_opmode_t lastMode = _mode;
    setMode(OPERATION_MODE_CONFIG);
    delay(25);

    /* Accel offset range depends on the G-range:
       +/-2g  = +/- 2000 mg
       +/-4g  = +/- 4000 mg
       +/-8g  = +/- 8000 mg
       +/-1§g = +/- 16000 mg */
    offsets_type.accel_offset_x = (read8(ACCEL_OFFSET_X_MSB_ADDR) << 8) |
                                  (read8(ACCEL_OFFSET_X_LSB_ADDR));
    offsets_type.accel_offset_y = (read8(ACCEL_OFFSET_Y_MSB_ADDR) << 8) |
                                  (read8(ACCEL_OFFSET_Y_LSB_ADDR));
    offsets_type.accel_offset_z = (read8(ACCEL_OFFSET_Z_MSB_ADDR) << 8) |
                                  (read8(ACCEL_OFFSET_Z_LSB_ADDR));

    /* Magnetometer offset range = +/- 6400 LSB where 1uT = 16 LSB */
    offsets_type.mag_offset_x =
        (read8(MAG_OFFSET_X_MSB_ADDR) << 8) | (read8(MAG_OFFSET_X_LSB_ADDR));
    offsets_type.mag_offset_y =
        (read8(MAG_OFFSET_Y_MSB_ADDR) << 8) | (read8(MAG_OFFSET_Y_LSB_ADDR));
    offsets_type.mag_offset_z =
        (read8(MAG_OFFSET_Z_MSB_ADDR) << 8) | (read8(MAG_OFFSET_Z_LSB_ADDR));

    /* Gyro offset range depends on the DPS range:
      2000 dps = +/- 32000 LSB
      1000 dps = +/- 16000 LSB
       500 dps = +/- 8000 LSB
       250 dps = +/- 4000 LSB
       125 dps = +/- 2000 LSB
       ... where 1 DPS = 16 LSB */
    offsets_type.gyro_offset_x =
        (read8(GYRO_OFFSET_X_MSB_ADDR) << 8) | (read8(GYRO_OFFSET_X_LSB_ADDR));
    offsets_type.gyro_offset_y =
        (read8(GYRO_OFFSET_Y_MSB_ADDR) << 8) | (read8(GYRO_OFFSET_Y_LSB_ADDR));
    offsets_type.gyro_offset_z =
        (read8(GYRO_OFFSET_Z_MSB_ADDR) << 8) | (read8(GYRO_OFFSET_Z_LSB_ADDR));

    /* Accelerometer radius = +/- 1000 LSB */
    offsets_type.accel_radius =
        (read8(ACCEL_RADIUS_MSB_ADDR) << 8) | (read8(ACCEL_RADIUS_LSB_ADDR));

    /* Magnetometer radius = +/- 960 LSB */
    offsets_type.mag_radius =
        (read8(MAG_RADIUS_MSB_ADDR) << 8) | (read8(MAG_RADIUS_LSB_ADDR));

    setMode(lastMode);
    return true;
  }
  return false;
}

/*!
 *  @brief  Writes an array of calibration values to the sensor's offset
 *  @param  calibData
 *          calibration data
 */
inline void Adafruit_BNO055::setSensorOffsets(const uint8_t *calibData) {
  adafruit_bno055_opmode_t lastMode = _mode;
  setMode(OPERATION_MODE_CONFIG);
  delay(25);

  /* Note: Configuration will take place only when user writes to the last
     byte of each config data pair (ex. ACCEL_OFFSET_Z_MSB_ADDR, etc.).
     Therefore the last byte must be written whenever the user wants to
     changes the configuration. */

  /* A writeLen() would make this much cleaner */
  write8(ACCEL_OFFSET_X_LSB_ADDR, calibData[0]);
  write8(ACCEL_OFFSET_X_MSB_ADDR, calibData[1]);
  write8(ACCEL_OFFSET_Y_LSB_ADDR, calibData[2]);
  write8(ACCEL_OFFSET_Y_MSB_ADDR, calibData[3]);
  write8(ACCEL_OFFSET_Z_LSB_ADDR, calibData[4]);
  write8(ACCEL_OFFSET_Z_MSB_ADDR, calibData[5]);

  write8(MAG_OFFSET_X_LSB_ADDR, calibData[6]);
  write8(MAG_OFFSET_X_MSB_ADDR, calibData[7]);
  write8(MAG_OFFSET_Y_LSB_ADDR, calibData[8]);
  write8(MAG_OFFSET_Y_MSB_ADDR, calibData[9]);
  write8(MAG_OFFSET_Z_LSB_ADDR, calibData[10]);
  write8(MAG_OFFSET_Z_MSB_ADDR, calibData[11]);

  write8(GYRO_OFFSET_X_LSB_ADDR, calibData[12]);
  write8(GYRO_OFFSET_X_MSB_ADDR, calibData[13]);
  write8(GYRO_OFFSET_Y_LSB_ADDR, calibData[14]);
  write8(GYRO_OFFSET_Y_MSB_ADDR, calibData[15]);
  write8(GYRO_OFFSET_Z_LSB_ADDR, calibData[16]);
  write8(GYRO_OFFSET_Z_MSB_ADDR, calibData[17]);

  write8(ACCEL_RADIUS_LSB_ADDR, calibData[18]);
  write8(ACCEL_RADIUS_MSB_ADDR, calibData[19]);

  write8(MAG_RADIUS_LSB_ADDR, calibData[20]);
  write8(MAG_RADIUS_MSB_ADDR, calibData[21]);

  setMode(lastMode);
}

/*!
 *  @brief  Writes to the sensor's offset registers from an offset struct
 *  @param  offsets_type
 *          accel_offset_x = acceleration offset x
 *          accel_offset_y = acceleration offset y
 *          accel_offset_z = acceleration offset z
 *
 *          mag_offset_x   = magnetometer offset x
 *          mag_offset_y   = magnetometer offset y
 *          mag_offset_z   = magnetometer offset z
 *
 *          gyro_offset_x  = gyroscrope offset x
 *          gyro_offset_y  = gyroscrope offset y
 *          gyro_offset_z  = gyroscrope offset z
 */
inline void Adafruit_BNO055::setSensorOffsets(
    const adafruit_bno055_offsets_t &offsets_type) {
  adafruit_bno055_opmode_t lastMode = _mode;
  setMode(OPERATION_MODE_CONFIG);
  delay(25);

  /* Note: Configuration will take place only when user writes to the last
     byte of each config data pair (ex. ACCEL_OFFSET_Z_MSB_ADDR, etc.).
     Therefore the last byte must be written whenever the user wants to
     changes the configuration. */

  write8(ACCEL_OFFSET_X_LSB_ADDR, (offsets_type.accel_offset_x) & 0x0FF);
  write8(ACCEL_OFFSET_X_MSB_ADDR, (offsets_type.accel_offset_x >> 8) & 0x0FF);
  write8(ACCEL_OFFSET_Y_LSB_ADDR, (offsets_type.accel_offset_y) & 0x0FF);
  write8(ACCEL_OFFSET_Y_MSB_ADDR, (offsets_type.accel_offset_y >> 8) & 0x0FF);
  write8(ACCEL_OFFSET_Z_LSB_ADDR, (offsets_type.accel_offset_z) & 0x0FF);
  write8(ACCEL_OFFSET_Z_MSB_ADDR, (offsets_type.accel_offset_z >> 8) & 0x0FF);

  write8(MAG_OFFSET_X_LSB_ADDR, (offsets_type.mag_offset_x) & 0x0FF);
  write8(MAG_OFFSET_X_MSB_ADDR, (offsets_type.mag_offset_x >> 8) & 0x0FF);
  write8(MAG_OFFSET_Y_LSB_ADDR, (offsets_type.mag_offset_y) & 0x0FF);
  write8(MAG_OFFSET_Y_MSB_ADDR, (offsets_type.mag_offset_y >> 8) & 0x0FF);
  write8(MAG_OFFSET_Z_LSB_ADDR, (offsets_type.mag_offset_z) & 0x0FF);
  write8(MAG_OFFSET_Z_MSB_ADDR, (offsets_type.mag_offset_z >> 8) & 0x0FF);

  write8(GYRO_OFFSET_X_LSB_ADDR, (offsets_type.gyro_offset_x) & 0x0FF);
  write8(GYRO_OFFSET_X_MSB_ADDR, (offsets_type.gyro_offset_x >> 8) & 0x0FF);
  write8(GYRO_OFFSET_Y_LSB_ADDR, (offsets_type.gyro_offset_y) & 0x0FF);
  write8(GYRO_OFFSET_Y_MSB_ADDR, (offsets_type.gyro_offset_y >> 8) & 0x0FF);
  write8(GYRO_OFFSET_Z_LSB_ADDR, (offsets_type.gyro_offset_z) & 0x0FF);
  write8(GYRO_OFFSET_Z_MSB_ADDR, (offsets_type.gyro_offset_z >> 8) & 0x0FF);

  write8(ACCEL_RADIUS_LSB_ADDR, (offsets_type.accel_radius) & 0x0FF);
  write8(ACCEL_RADIUS_MSB_ADDR, (offsets_type.accel_radius >> 8) & 0x0FF);

  write8(MAG_RADIUS_LSB_ADDR, (offsets_type.mag_radius) & 0x0FF);
  write8(MAG_RADIUS_MSB_ADDR, (offsets_type.mag_radius >> 8) & 0x0FF);

  setMode(lastMode);
}

/*!
 *  @brief  Checks of all cal status values are set to 3 (fully calibrated)
 *  @return status of calibration
 */
inline bool Adafruit_BNO055::isFullyCalibrated() {
  uint8_t system, gyro, accel, mag;
  getCalibration(&system, &gyro, &accel, &mag);

  switch (_mode) {
  case OPERATION_MODE_ACCONLY:
    return (accel == 3);
  case OPERATION_MODE_MAGONLY:
    return (mag == 3);
  case OPERATION_MODE_GYRONLY:
  case OPERATION_MODE_M4G: /* No magnetometer calibration required. */
    return (gyro == 3);
  case OPERATION_MODE_ACCMAG:
  case OPERATION_MODE_COMPASS:
    return (accel == 3 && mag == 3);
  case OPERATION_MODE_ACCGYRO:
  case OPERATION_MODE_IMUPLUS:
    return (accel == 3 && gyro == 3);
  case OPERATION_MODE_MAGGYRO:
    return (mag == 3 && gyro == 3);
  default:
    return (system == 3 && gyro == 3 && accel == 3 && mag == 3);
  }
}

/*!
 *  @brief  Enter Suspend mode (i.e., sleep)
 */
inline void Adafruit_BNO055::enterSuspendMode() {
  adafruit_bno055_opmode_t modeback = _mode;

  /* Switch to config mode (just in case since this is the default) */
  setMode(OPERATION_MODE_CONFIG);
  delay(25);
  write8(BNO055_PWR_MODE_ADDR, 0x02);
  /* Set the requested operating mode (see section 3.3) */
  setMode(modeback);
  delay(20);
}

/*!
 *  @brief  Enter Normal mode (i.e., wake)
 */
inline void Adafruit_BNO055::enterNormalMode() {
  adafruit_bno055_opmode_t modeback = _mode;

  /* Switch to config mode (just in case since this is the default) */
  setMode(OPERATION_MODE_CONFIG);
  delay(25);
  write8(BNO055_PWR_MODE_ADDR, 0x00);
  /* Set the requested operating mode (see section 3.3) */
  setMode(modeback);
  delay(20);
}

/*!
 *  @brief  Writes an 8 bit value over I2C
 */
inline bool Adafruit_BNO055::write8(adafruit_bno055_reg_t reg, byte value) {
  uint8_t buffer[2] = {(uint8_t)reg, (uint8_t)value};
  return i2c_dev->write(buffer, 2);
}

/*!
 *  @brief  Reads an 8 bit value over I2C
 */
inline byte Adafruit_BNO055::read8(adafruit_bno055_reg_t reg) {
  uint8_t buffer[1] = {reg};
  i2c_dev->write_then_read(buffer, 1, buffer, 1);
  return (byte)buffer[0];
}

/*!
 *  @brief  Reads the specified number of bytes over I2C
 */
inline bool Adafruit_BNO055::readLen(adafruit_bno055_reg_t reg, byte *buffer,
                              uint8_t len) {
  uint8_t reg_buf[1] = {(uint8_t)reg};
  return i2c_dev->write_then_read(reg_buf, 1, buffer, len);
}


#endif

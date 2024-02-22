#ifndef _DRV_PIT_HPP_
#define _DRV_PIT_HPP_

#include <stdbool.h>
#include <stdint.h>
#include "stm32f0xx_hal.h"


/**
 * @brief  Timer units enumeration
 */
typedef enum
{
  MiliSec, /*!< Specifies the unit of milliseconds to the counter. */
} ValidUnitsOfPIT;

/**
 * @brief  Software timer control structure.
 */
typedef struct
{
  ValidUnitsOfPIT usedUnit; /*!< Specifies the unit of the counter to be
                            configured. This parameter can be any value
                            of @ref PIT_Units                         */
  uint32_t countLimit;      /*!< Specifies the time to count.
                            This parameter can be any uint32_t value  */
  uint32_t countAtRequest;  /*!< Specifies the actual time count.     */
  uint32_t status;          /*!< Specifies the time to count.
                            This parameter can be any uint32_t value  */
  bool waitOverflow;
} PIT_TimerParam;

class DrvPIT
{
public:
  DrvPIT(void);
  virtual ~DrvPIT();

  bool set(uint32_t CountsToPerform, ValidUnitsOfPIT Unit = MiliSec);
  bool get(void);
  void halt(void);
  void resume(void);
  uint32_t getCounter(void);

  static uint32_t getPITCount(void);

  inline void setAutoReload(bool state) {m_autoReload = state;}

private:

  void configurePeripheral(void);

  PIT_TimerParam m_timer;

  /**
   * PIT configuration flag.
   */
  bool m_isPeripheralConfigured;

  /**
   * PIT Enabled flag.
   */
  bool m_isPeripheralEnabled;
  uint32_t m_CountsToPerform;
  bool m_autoReload;
};

#endif /* _DRV_PIT_HPP_ */

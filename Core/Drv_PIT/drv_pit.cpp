/**
  ==============================================================================
                        ##### How to use this driver #####
  ==============================================================================

  ...
  // Create objects
  DrvPIT myTimer1;
  DrvPIT myTimer2;

  // Create a timer event of 100ms
  (void)myTimer1.set(100, MiliSec);
  // Create a timer event of 500ms with auto reload
  (void)myTimer2.set(500, MiliSec);
  (void)myTimer2.setAutoReload(true);


  while (1)
  {
    if(myTimer1.get() == true){ // Verif
      (void)myTimer1.set(100, MiliSec); // Restart the timer for the led1
      //Toggle GPIO pin where LED1 is connected here
    }

    if(myTimer2.get() == true){
      // No need to restart the timer for the led2 since it will autoreload
      //Toggle GPIO pin where LED2 is connected here
    }
  }

 */

#include "drv_pit.hpp"

/**
 * Maximum value in ms. When the tick is 0XFFFFFFFF - 1.
 */
#define PIT_MAX_COUNT_MS 0x7CFFFFF

/**
 * Return the actual tick counter.
 */
#define getPITCount() (uint32_t) HAL_GetTick() /* PIT Peripheral counts backwards */

/**
 * Determine the difference between the maximum count and actual counter value.
 */
#define GetCountToOverflow() (uint32_t)(0xffffffff - getPITCount()) /* PIT Peripheral counts backwards */

/**
 * @brief Initializes the PIT driver.
 *
 */
DrvPIT::DrvPIT(void)
{
  m_isPeripheralConfigured = false;
  m_isPeripheralEnabled = false;
  m_CountsToPerform = 0;
  m_autoReload = false;
  m_timer.waitOverflow = false;
}

DrvPIT::~DrvPIT() {}

/**
 * @brief  Start time counting routine.
 *
 * @param  CountsToPerform : How many ticks to count
 * @param  Unit : Tick's time base.
 *         This parameter can be one of the following values:
 *             @arg MiliSec: Counter is set to miliseconds base.
 * @retval Result : Result of Operation
 *         This parameter can be one of the following values:
 *              @arg true: All ok, time is being counted
 *              @arg Else: Some error happened.
 */
bool DrvPIT::set(uint32_t CountsToPerform, ValidUnitsOfPIT Unit)
{
  bool returnValue;
  uint32_t countsToOverflow;
  uint32_t currentPitCount;

  /* counts must be different than zero and Unit must be of Milissecond ( no other  */
  /*  is implemented).                                                        */
  if ((CountsToPerform == 0) || (Unit != MiliSec))
  { /* Wrong parameter(s) */
    returnValue = false;
  }
  /* Check if the device is disabled. If it is, it cannot perform the count.  */
  else if ((m_isPeripheralEnabled == false) && (m_isPeripheralConfigured != false))
  { /* Device is disabled, cannot proceed */
    returnValue = false;
  }

  else
  {
    m_CountsToPerform = CountsToPerform;
    /* If the peripheral is not yet configured, do so now. (first time)       */
    if (m_isPeripheralConfigured == false)
    {
      m_isPeripheralConfigured = true; /* Peripheral will now be configured.   */
      m_isPeripheralEnabled = true;    /* Peripheral will start enabled.       */

      configurePeripheral(); /* Call configuration routine.          */
    }
    countsToOverflow = GetCountToOverflow();
    currentPitCount = getPITCount();

    if (countsToOverflow >= CountsToPerform)
    { /* Count will finish before the overflow event */
      m_timer.countLimit = currentPitCount + CountsToPerform;
      m_timer.waitOverflow = false;
    }
    else
    { /* Count will finish after the overflow event */
      m_timer.countLimit = CountsToPerform - countsToOverflow;
      m_timer.waitOverflow = true;
    }

    /* Define status */
    m_timer.status = false;

    /* Saves the count value at request */
    m_timer.countAtRequest = currentPitCount;

    /* Job done */
    returnValue = true;
  }

  return returnValue;
}

/**
 * @brief  Routine that checks if the requested time has passed.
 *
 * @retval Result : Result of Operation.
 *         This parameter can be one of the following values:
 *          @arg true: All ok, time is being counted
 *          @arg false: Time has not passed yet, keep waiting.
 *          @arg Else: Some error happened.
 */
bool DrvPIT::get() {
	bool returnValue;
	uint32_t currentPitCount;

	/* Check if the device is disabled. If it is, it cannot perform the count.  */
	if ((m_isPeripheralEnabled == false) && (m_isPeripheralConfigured != false)) { /* Device is disabled, cannot proceed */
		returnValue = false;
	} else if (m_timer.status == true) { /* Timer has already expired. */
		returnValue = true;
	} else { /* Check current PIT value and calculate if it has expired.               */
		currentPitCount = getPITCount();

    if (m_timer.waitOverflow != false)
    {
      if (currentPitCount < m_timer.countAtRequest)
      {
        m_timer.waitOverflow = false;
      }
      returnValue = false;
    }
    else
    {
      if (currentPitCount >= m_timer.countLimit)
      {
        m_timer.status = true;
        returnValue = true;
      }
      else
      {
        returnValue = false;
      }
    }
	}

  if(returnValue && m_autoReload)
	  set(m_CountsToPerform);
	return returnValue;
}

/**
 * @brief  Stop driver's operation.
 * @retval Result : Result of Operation
 *         This parameter can be one of the following values:
 *             @arg true: All ok, time is being counted
 *             @arg Else: Some error happened.
 */
void DrvPIT::halt(void)
{
  /* Simply inform that process went well and mark that driver is disabled.   */
  m_isPeripheralEnabled = false;
}

/**
 * @brief  Resume driver's operation.
 * @retval Result : Result of Operation
 *         This parameter can be one of the following values:
 *            @arg true: All ok, time is being counted
 *            @arg Else: Some error happened.
 */
void DrvPIT::resume(void)
{
  /* Simply inform that process went well and mark that driver is running.  */
  m_isPeripheralEnabled = true;
}

/**
 * @brief Get the current timer value
 *
 * @return uint32_t Timer value
 */
uint32_t DrvPIT::getCounter(void)
{
  uint32_t currentPitCount = getPITCount();

  return (m_timer.countLimit - currentPitCount);
}

/**
 * @brief  Configure timer to generate periodic interrupt time.
 */
void DrvPIT::configurePeripheral() {}

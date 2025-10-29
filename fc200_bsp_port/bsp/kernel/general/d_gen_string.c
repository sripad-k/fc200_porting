/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : General string functions

  Abstract           : Driver providing basic string formatting and
                       manipulation functions which are safe

  Software Structure : SRS References: 136T-2200-131000-001-D20 SWREQ-754
                       SDD References: 136T-2200-131000-001-D22 SWDES-724
                                                                SWDES-725
                                                                SWDES-726
                                                                SWDES-727
                                                                SWDES-728
                                                                SWDES-729
                                                                SWDES-730
\note
  CSCID              : SWDES-719
*************************************************************************/

/***** Includes *********************************************************/

#include "d_gen_string.h"

#include "kernel/error_handler/d_error_handler.h"
#include "kernel/general/d_gen_memory.h"

/***** Constants ********************************************************/

/* Buffer size for float to string operation */
#define CHAR_BUFF_SIZE  (Int32_t)32

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

static Uint32_t WriteDigitsReverse(Uint32_t value, Char_t * const pBuff, const Uint32_t bufferSize, const Uint32_t base);
static Int32_t validateConvertIntegerToString(const Char_t * const pBuffer, const Uint32_t bufferSize, const Uint32_t digits, const Uint32_t base);
static Int32_t validateConvertFloatToString(const Float32_t value, const Char_t * const pBuffer, const Uint32_t bufferSize);
static void integerAddDigits(const Uint32_t digits, Char_t * const buff, Uint32_t * const pCount, const Bool_t isNegative);
static void floatAddDecimals(Uint32_t decimals, Char_t *tempBuffer, Int32_t * pBufferIndex, Int32_t * pCount);
static void floatAddUnits(Int32_t units, Char_t *tempBuffer, Int32_t * pBufferIndex, Int32_t * pCount);

/***** Function Definitions *********************************************/

/*********************************************************************//**
  <!-- d_GEN_SprintfInteger -->

  A Safe implementation of sprintf to add a single integer to a string
  Integer can be displayed as decimal or hex, depending on the formating string
  use %d for decimal and %x for hex

*************************************************************************/
Int32_t                             /** \return Error code upon failure, length of string on success */
d_GEN_SprintfInteger
(Char_t * const outputString,       /**< [out] Pointer to string where the output is written to */
const Uint32_t bufferSize,          /**< [in] Space available in the output buffer */
const Char_t * const format,        /**< [in] The formatting string to use */
const Int32_t value)                /**< [in] The Integer number to add to the string */
{
  if (outputString == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (bufferSize == 0u)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (format == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 3, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  Int32_t returnValue = 0;
  Uint32_t index = 0u;
  Uint32_t writeIndex = 0u;
  while ((format[index] != '\0') && (writeIndex < bufferSize))
  {
    if (format[index] == '%') 
    {
      index++;
      Int32_t length = 0;
      if (format[index]  == 'd') 
      {
        length = d_GEN_ConvertIntegerToString(value, &outputString[writeIndex], bufferSize - writeIndex, 0, 10);
      } 
      else if (format[index]  == 'x')
      {
        length = d_GEN_ConvertIntegerToString(value, &outputString[writeIndex], bufferSize - writeIndex, 0, 16);
      }
      else
      {
        /* Error, unsupported formatting */
        /* let's simply keep the unsupported formating string as is in the output string */
        outputString[writeIndex] = format[index - 1u];
        writeIndex++;
        outputString[writeIndex] = format[index] ;
        writeIndex++;
      }

      if (length < 0)
      {
        // gcov-jst 2 It is not possible to generate this error during bench testing.
        returnValue = length; /* return error from the integer to string conversion */
        break;
      }
      else
      {
        DO_NOTHING();
      }
      
      writeIndex += (Uint32_t)length;      
    }
    else
    {
      outputString[writeIndex] = format[index];
      writeIndex++;
    }

    returnValue = (Int32_t)writeIndex; /* the return value will be the length of the new string, so we update it here */
    index++;
  }

  /* Always make sure the the output string is null terminated */
  outputString[writeIndex] = '\0';

  return returnValue;
}

/*********************************************************************//**
  <!-- d_GEN_SprintfFloat -->

  A Safe implementation of sprintf to add a single float to a string
  Only two digits will be displayed after the comma

*************************************************************************/
Int32_t                           /** \return Error code upon failure, length of string on success */
d_GEN_SprintfFloat
(Char_t * const outputString,     /**< [out] Pointer to string where the output is written to */
const Uint32_t bufferSize,        /**< [in] Space available in the output buffer */
const Char_t * const format,      /**< [in] The formatting string to use */
const Float32_t value)            /**< [in] The float number to add to the string */
{
  if (outputString == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (bufferSize == 0u)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (format == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 3, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  Int32_t returnValue = 0;
  Uint32_t index = 0u;
  Uint32_t writeIndex = 0u;
  while ((format[index] != '\0') && (writeIndex < bufferSize))
  {
    if (format[index] == '%') 
    {
      index++;
      Int32_t length = 0;
      if (format[index]  == 'f') 
      {
        length = d_GEN_ConvertFloatToString(value, &outputString[writeIndex], bufferSize - writeIndex);
      } 
      else
      {
        /* Error, unsupported formatting */
        /* let's simply keep the unsupported formating string as is in the output string */
        outputString[writeIndex] = format[index - 1u];
        writeIndex++;
        outputString[writeIndex] = format[index] ;
        writeIndex++;
      }

      if (length < 0)
      {
        returnValue = length; /* return error from the integer to string conversion */
        break;
      }
      else
      {
        DO_NOTHING();
      }
      
      writeIndex += (Uint32_t)length;
    }
    else
    {
      outputString[writeIndex] = format[index];
      writeIndex++;
    }

    returnValue = (Int32_t)writeIndex; /* the return value will be the length of the new string, so we update it here */
    index++;
  }

  /* Always make sure the the output string is null terminated */
  outputString[writeIndex] = '\0';

  return returnValue;
}

/*********************************************************************//**
  <!-- d_GEN_ConvertIntegerToString -->

  Convert integer to string.

*************************************************************************/
Int32_t                         /** \return Error code upon failure, length of string on success */
d_GEN_ConvertIntegerToString
(
Int32_t value,                  /**< [in] Value to convert */
Char_t * const pBuffer,         /**< [out] Buffer for result */
const Uint32_t bufferSize,      /**< [in] The space available in the output buffer */
Uint32_t digits,                /**< [in] Number of digits, 0 for don't care */
const Uint32_t base             /**< [in] Number base */
)
{
  Int32_t returnValue = validateConvertIntegerToString(pBuffer, bufferSize, digits, base);
  if (returnValue != 0)
  {
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return returnValue;
  }

  Bool_t isNegative = d_FALSE;

  Uint32_t unsignedCopy = (Uint32_t)value;
  if ((base == 10u) && (value < 0))
  {
    isNegative = d_TRUE;
    // cppcheck-suppress misra-c2012-17.8; No risk implementation avoids violating other Misra rules or adding a temporary variable.
    value = -1 * value;
    unsignedCopy = (Uint32_t)value;
  }
  else
  {
    DO_NOTHING();
  }

  /* Buffer long enough to hold largest int32 including minus sign and terminating character*/
  Char_t buff[12];
  Uint32_t count = WriteDigitsReverse(unsignedCopy, buff, sizeof(buff), base);
  
  /* will the string fit in the provided buffer and leave enough space for terminating char */
  if (count > (bufferSize - 1u)) 
  {
    pBuffer[0] = '\0';
    returnValue = d_STATUS_OVERRUN;
  }
  else
  {
    DO_NOTHING();
  }

  if (returnValue == 0)
  {
    integerAddDigits(digits, &buff[0], &count, isNegative);
    Uint32_t index;
    /* Transfer to output buffer */
    for (index = 0; index < count; index++)
    {
      pBuffer[index] = buff[count - 1u - index];
    }
      
    /* add zero termination */
    pBuffer[index] = '\0';  
    returnValue = (Int32_t)count;
  }
  else
  {
    DO_NOTHING();
  }
  
  return returnValue;
}

/*********************************************************************//**
  <!-- d_GEN_ConvertFloatToString -->

  Convert a floating point number to string. This function only prints two
  digits after the decimal point. Note that this function does not support
  e-notation so this function only supports numbers within the following
  range: -2147483647.99 to 2147483647.99.

*************************************************************************/
Int32_t                             /** \return Error code upon failure, length of string on success */
d_GEN_ConvertFloatToString
(const Float32_t value,             /**< [in] Value to convert */
Char_t * const pBuffer,             /**< [out] Buffer for result */
const Uint32_t bufferSize)          /**< [in] The space available in the output buffer */
{
  Int32_t returnValue = validateConvertFloatToString(value, pBuffer, bufferSize);
  if (returnValue != 0)
  {
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return returnValue;
  }

  Char_t tempBuffer[CHAR_BUFF_SIZE];
  Int32_t bufferIndex;
  bufferIndex = CHAR_BUFF_SIZE - 1;     /* go to end of buffer */
  Uint32_t decimals;                              /* variable to store the decimals */
  Int32_t units;                                  /* variable to store the units (part to left of decimal place) */
  if (value < 0.0f)                               /* take care of negative numbers */
  { 
    decimals = (Int32_t)(value * -100.0f) % 100;     /* make 1000 for 3 decimals etc. for now we only support 2 */
    units = (Int32_t)(-1.0f * value);
  } 
  else // positive numbers
  { 
    decimals = (Int32_t)(value * 100.0f) % 100;
    units = (Int32_t)value;
  }

  floatAddDecimals(decimals, &tempBuffer[0], &bufferIndex, &returnValue);

  floatAddUnits(units, &tempBuffer[0], &bufferIndex, &returnValue);

  if (returnValue >= 0)
  {
    if (value < 0.0f) 
    {
      tempBuffer[bufferIndex] = '-';                                   /* unary minus sign for negative numbers */
      bufferIndex--;
      returnValue++;                                /* The total length of the string will be returned if we are successfull */
    }
    else
    {
      DO_NOTHING();
    }

    if ((Uint32_t)returnValue > (bufferSize - 1u))            /* will the string fit in the provided buffer and leave enough space for terminating char */
    {
      pBuffer[0] = '\0';
      returnValue = d_STATUS_OVERRUN;
    }
    else
    {
      /* Transfer to output buffer */
      for (Int32_t i = 0; i < returnValue; i++)
      {
        pBuffer[i] = tempBuffer[bufferIndex + i + 1];
      }

      pBuffer[returnValue] = '\0';
    }
  }
  else
  {
    // gcov-jst 3 It is not possible to generate this failure during bench testing.
    pBuffer[0] = '\0';
  }

  return returnValue;
}

/*********************************************************************//**
  <!-- d_GEN_StringLength -->

  Determine the length of a null terminated string.
  If the string is not null terminated, an error is returned.

*************************************************************************/
Int32_t                           /** \return Length of the string */
d_GEN_StringLength
(const Char_t * const pBuffer,    /**< [in] Null terminated string to be processed */
const Uint32_t bufferSize)        /**< [in] the total size of the buffer, to make sure we don't read past the boundaries of the buffer */
{
  Int32_t returnValue = 0;
  if (pBuffer == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  Uint32_t index = 0;
  while ((index < bufferSize) && (pBuffer[index] != '\0'))
  {
    index++;
  }

  returnValue = (Int32_t)index;

  return returnValue;
}

/*********************************************************************//**
  <!-- d_GEN_StringConcatenate -->

  Concatenate a string to an existing string

*************************************************************************/
Int32_t                             /** \return length of the new string upon success, error code on failure  */
d_GEN_StringConcatenate
(
  Char_t * const pBuffer,           /**< [in/out] Buffer containing original string that gets concatenated onto. */
  const Uint32_t bufferSize,        /**< [in] Length of the destination buffer, should be big enough for the resulting string */
  const Char_t * const pSource,     /**< [in] string to add to the buffer */
  const Uint32_t length             /**< [in] Maximum number of characters available in the source buffer */
)
{
  if (pBuffer == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (pSource == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 3, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  Uint32_t currentLength = d_GEN_StringLength(pBuffer, bufferSize);
  Uint32_t copyLength = d_GEN_StringLength(pSource, length);

  /* StringCopy will add terminating character */
  Int32_t returnValue = d_GEN_StringCopy(&pBuffer[currentLength], bufferSize - currentLength, pSource, copyLength); 
  if (returnValue >= 0 )
  {
    returnValue += (Int32_t)currentLength;
  }
  else
  {
    DO_NOTHING();
  }

  return returnValue;
}

/*********************************************************************//**
  <!-- d_GEN_StringCopy -->

  Copy a string

*************************************************************************/
Int32_t                           /** \return Number of characters copied, or the error code in case of failure */
d_GEN_StringCopy
(
Char_t * const pDestination,      /**< [out] destination buffer */
const Uint32_t destinationLength, /**< [in] available space in the destination buffer */ 
const Char_t * const pSource,     /**< [in] source buffer */
const Uint32_t sourceLength       /**< [in] size of the source buffer */
)
{
  if (pDestination == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (pSource == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 3, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (sourceLength > (destinationLength - 1u))
  {
    d_ERROR_Logger(d_STATUS_OVERRUN, d_ERROR_CRITICALITY_NON_CRITICAL, sourceLength, destinationLength, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_OVERRUN;
  }

  Uint32_t index = 0u;
  while ((index < sourceLength) && (index < (destinationLength - 1u)) && (pSource[index] != '\0'))
  {
    pDestination[index] = pSource[index];
    index++;
  }

  pDestination[index] = '\0';

  return (Int32_t)index;
}

/*********************************************************************//**
  <!-- WriteDigitsReverse -->

  Internal function to write the actual digits of the number to an interim string.
  Note that the digits are written in reverse. This function is simply an intermediate step
  in the conversion from numbers to strings. This Function should not be used on it's own.

*************************************************************************/
static Uint32_t             /** \return the length of the string */
WriteDigitsReverse
(Uint32_t value,            /**< [in] the value to write */
Char_t * const pBuff,       /**< [out] Pointer to string where the output is written to */
const Uint32_t bufferSize,  /**< [in] The size of the buffer we are writing to */
const Uint32_t base)        /**< [in] The base we are converting to */
{
  Uint32_t index = 0u;
  Uint32_t val = value;
  
  while ((val > 0u) && (index < bufferSize))
  {
    pBuff[index] = (Char_t)((val % base) + '0');
    /* Correct for hex */
    if (pBuff[index] > '9')
    {
      pBuff[index] = (Char_t)(pBuff[index] + 7);
    }
    else
    {
      DO_NOTHING();
    }
    val = val / base;
    index++;
  }

  pBuff[index] = '\0';
  
  return index;
}

/*********************************************************************//**
  <!-- validateConvertIntegerToString -->

  Validate parameters for d_GEN_ConvertIntegerToString function.
*************************************************************************/
static Int32_t                      /** \return Error code upon failure, zero on success */
validateConvertIntegerToString
(
const Char_t * const pBuffer,       /**< [out] Buffer for result */
const Uint32_t bufferSize,          /**< [in] The space available in the output buffer */
const Uint32_t digits,              /**< [in] Number of digits, 0 for don't care */
const Uint32_t base                 /**< [in] Number base */
)
{
  if (pBuffer == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (bufferSize == 0u)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 3, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;    
  }

  if (digits > bufferSize) /* we cannot overwrite the output buffer */
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 4, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if ((base != 10u) && (base != 16u)) // currently this library only officially supports conversion to either decimal or hex
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 5, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  return 0;
}

/*********************************************************************//**
  <!-- validateConvertFloatToString -->

  Validate parameters for d_GEN_ConvertFloatToString function.
*************************************************************************/
static Int32_t                      /** \return Error code upon failure, zero on success */
validateConvertFloatToString
(const Float32_t value,             /**< [in] Value to convert */
const Char_t * const pBuffer,       /**< [out] Buffer for result */
const Uint32_t bufferSize)          /**< [in] The space available in the output buffer */
{
  /* Current design limits supported size of float */
  if ((value < -2147483648.0f) || (value > 2147483647.0f))
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (pBuffer == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (bufferSize == 0u)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 3, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  return 0;
}

/*********************************************************************//**
  <!-- integerAddDigits -->

  Add digits to integer string.
*************************************************************************/
static void                      /** \return None */
integerAddDigits
(
  const Uint32_t digits,         /**< [in] Number of digits */
  Char_t * const buff,           /**< [out] Buffer for result */
  Uint32_t * const pCount,       /**< [out] String length */
  const Bool_t isNegative        /**< [in] Value is negative */
)
{
  Uint32_t index;
  Uint32_t digitsOuput = digits;
    
  /* Add leading zeros if required */
  if (digitsOuput == 0u)
  {
    digitsOuput = 1u; /* Must output at least 1 character */ 
  }
  else
  {
    DO_NOTHING();
  }

  for (index = *pCount; index < digitsOuput; index++)
  {
    buff[index] = '0';
  }
  *pCount = index;

  if (isNegative == d_TRUE)
  {
    buff[*pCount] = '-';
    (*pCount)++;
  }
  else
  {
    DO_NOTHING();
  }

  return;
}

/*********************************************************************//**
  <!-- floatAddDecimals -->

  Add decimal characters to floating point string.
*************************************************************************/
static void                      /** \return None */
floatAddDecimals
(
  const Uint32_t decimals,
  Char_t * const tempBuffer,
  Int32_t * const pBufferIndex,
  Int32_t * const pCount
)
{
  Uint32_t decimalCount = decimals;
  tempBuffer[*pBufferIndex] = (Char_t)((decimalCount % 10u) + '0');
  (*pBufferIndex)--;
  decimalCount /= 10;                                  /* repeat for as many decimal places as you need, for now we only support 2 */
  tempBuffer[*pBufferIndex] = (Char_t)((decimalCount % 10u) + '0');
  (*pBufferIndex)--;
  tempBuffer[*pBufferIndex] = '.';
  (*pBufferIndex)--;
  *pCount = 3;                                 /* The total length of the string will be returned if we are successfull */

  return;
}

/*********************************************************************//**
  <!-- floatAddUnits -->

  Add integer characters to floating point string.
*************************************************************************/
static void                      /** \return None */
floatAddUnits
(
  const Int32_t units,
  Char_t * const tempBuffer,
  Int32_t * const pBufferIndex,
  Int32_t * const pCount
)
{
  Int32_t unitsCount = units;
  if (unitsCount == 0)                                  /* in case the units is zero we need to write at least 1 zero otherwise we end up with .xx */
  {
    tempBuffer[*pBufferIndex] = '0';
    (*pBufferIndex)--;
    (*pCount)++;
  }
  else
  {
    DO_NOTHING();
  }

 
  while (unitsCount > 0)
  {
    tempBuffer[*pBufferIndex] = (Char_t)((unitsCount % 10) + '0');
    (*pBufferIndex)--;
    unitsCount /= 10;
    (*pCount)++;                                /* The total length of the string will be returned if we are successfull */
    if (*pCount >= CHAR_BUFF_SIZE)
    {
      /* number too large for internal buffer */
      // gcov-jst 3 It is not possible to generate this failure during bench testing.
      *pCount = d_STATUS_OVERRUN;
      break;
    }
    else
    {
      DO_NOTHING();
    }
  }
  
  return;
}


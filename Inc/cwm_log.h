/*
 * cwm_log.h
 *
 *  Created on: 2022年3月30日
 *      Author: user
 */

#ifndef INC_CWM_LOG_H_
#define INC_CWM_LOG_H_

/* Includes ------------------------------------------------------------------*/

int CWM_OS_dbgOutput(const char *);
int CWM_OS_dbgOutput_checkLog(const char *);
int CWM_OS_dbgPrintf(const char *,...);

void cwm_log_init(void);
void cwm_log_close(void);

#endif /* INC_CWM_LOG_H_ */

/* ==========================================
	Unity Project - A Test Framework for C
	Copyright (c) 2007-21 Mike Karlesky, Mark VanderVoord, Greg Williams, David Koch
	[Released under MIT License. Please refer to license.txt for details]
========================================== */

/* Unity Target
 * Put here your target specific configuration
 */

#ifndef UNITY_TARGET_H
#define UNITY_TARGET_H

#if defined(WIN32)

	#define UNITY_OUTPUT_CHAR(a)	sendchar(a)
	#define UNITY_PRINT_EOL()		UNITY_OUTPUT_CHAR('\r'); UNITY_OUTPUT_CHAR('\n')

#else	/* WIN32 */
#endif	/* WIN32 */

#endif	/* UNITY_TARGET_H */

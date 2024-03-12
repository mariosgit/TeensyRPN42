#pragma once

#include <Arduino.h>


// stuff from keyboard.h collides
#undef KEY_SIGMA 
#undef KEY_INV   
#undef KEY_SQRT  
#undef KEY_LOG   
#undef KEY_LN    
#undef KEY_XEQ   
#undef KEY_STO   
#undef KEY_RCL   
#undef KEY_RDN   
#undef KEY_SIN   
#undef KEY_COS   
#undef KEY_TAN   
#undef KEY_ENTER 
#undef KEY_SWAP  
#undef KEY_CHS   
#undef KEY_E     
#undef KEY_BSP   
#undef KEY_UP    
#undef KEY_7     
#undef KEY_8     
#undef KEY_9     
#undef KEY_DIV   
#undef KEY_DOWN  
#undef KEY_4     
#undef KEY_5     
#undef KEY_6     
#undef KEY_MUL   
#undef KEY_SHIFT 
#undef KEY_1     
#undef KEY_2     
#undef KEY_3     
#undef KEY_SUB   
#undef KEY_EXIT  
#undef KEY_0     
#undef KEY_DOT   
#undef KEY_RUN   
#undef KEY_ADD   

// other macro clashes
#undef PI


#include <common/core_main.h>
#include <common/core_display.h>
#include <common/core_helpers.h>

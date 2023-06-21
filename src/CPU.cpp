#include "../inc/CPU.hpp"
#include <string>
#include <iostream>

static __int32_t __getCacheId(int32_t type, int32_t level){
    for (int32_t id = 0; ; id++ ) {
        // Variables to hold the contents of the 4 i386 legacy registers
        uint32_t eax = 4; // get cache info
        uint32_t ebx;
        uint32_t ecx = id; // cache id
        uint32_t edx;
        
        // generates output in 4 registers eax, ebx, ecx and edx
        __asm__ (
            "cpuid" // call i386 cpuid instruction
            :   "+a" (eax), // contains the cpuid command code, 4 for cache query
                "=b" (ebx),
                "+c" (ecx), // contains the cache id
                "=d" (edx)
        );
        
        // See the page 3-191 of the manual.
        int32_t cache_type = eax & 0x1F;
        
        // end of valid cache identifiers
        if (cache_type == 0 ) {
            break;
        }

        int32_t cache_level = (eax >>= 5) & 0x7;

        //Return cache id if found
        if(cache_level == level && cache_type == type) return id;


    }

    return -1; //no L1 data cache
}

__int32_t CPU::getBlockSize(){
        //print_i386_cpuid_caches();
        int32_t l1d_id = __getCacheId(1, 1);
        
        uint32_t eax = 4; // get cache info
        uint32_t ebx;
        uint32_t ecx = l1d_id; // cache id
        uint32_t edx;
        
        // generates output in 4 registers eax, ebx, ecx and edx
        __asm__ (
                "cpuid" // call i386 cpuid instruction
                :   "+a" (eax), // contains the cpuid command code, 4 for cache query
                "=b" (ebx),
                "+c" (ecx), // contains the cache id
                "=d" (edx)
        );

        // See the page 3-191 of the manual.
        int32_t cache_type = eax & 0x1F;
        
        // end of valid cache identifiers
        if (cache_type == 0 ) {
                return -1;
        }
        
        return (ebx & 0xFFF) + 1;
}
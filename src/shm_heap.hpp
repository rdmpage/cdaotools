#ifndef SHM_HEAP_HPP_
#define SHM_HEAP_HPP_

#include <sys/types.h>

namespace CDAO {
  /*
   * Initialize the memory segment.
   */
  void shm_init( void* base_addr, size_t segment_size );
  /*
   * Release the segment.
   */
  void shm_destroy();
  /*
   * Allocate a block of of the specified size from the pool.
   */
  void* shm_malloc( size_t allocation_size );
  /*
   * Allocate and and initialze to 0. A block of memory of the specified size.
   */
  void* shm_calloc( size_t allocation_size, size_t num_items );
  /*
   * Return the specified block to the free pool.
   */
  void shm_free( void* ptr );

  
}
#endif

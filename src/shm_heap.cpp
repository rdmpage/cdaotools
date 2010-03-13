#include "shm_heap.hpp"
#include <strings.h>
#include <cstdlib>
#include <map>

namespace CDAO {

/*
 * Keep track of the start and extent of free blocks in the pool
 */
static std::map< void*, size_t > free_blocks_;

/*
 * Initialize the memory segment.
 */
void shm_init( void* base_addr, size_t segment_size ){
   free_blocks_ = std::map< void*, size_t >();
   free_blocks_[ base_addr ] = segment_size;
   return;
}
/*
 * Release the segment.
 */
void shm_destroy(){
   free_blocks_.clear();
}


void* shm_malloc(size_t size){
      //calculate the actual size that we need to allocate
      size_t allocation_size = size + sizeof(size_t);
      //look in the free pool for a block that's big enough. 
      //in the worst case the last key will point to the highest address (the remainder of the heap (virgin space)).
      std::map<void*, size_t>::iterator i;
      for ( i = free_blocks_.begin(); i != free_blocks_.end() && i->second < allocation_size; ++i );
      //if there's not enough space to satisify the request bail now.
      if ( i == free_blocks_.end() ){ return NULL; }
      //get the start address of the allocation
      void* ret = i->first;
      //get the extent of the free block
      size_t block_free = i->second;
      //copy the size of the allocation into the start of the block
      memcpy(ret, &allocation_size, sizeof(size_t));
      //remove this block from the free pool.
      free_blocks_.erase( i );
      //place what ever is left of the block back into the free pool.
      free_blocks_[ (void*)((size_t)ret + allocation_size) ] = block_free - allocation_size;
      return ret;
}


void* shm_calloc(size_t size, size_t num_items){
        void* ret = malloc( size * num_items );
        bzero( ret, size * num_items );
        return ret;
}

void shm_free( void* addr ){ 
   void* real_addr = (void*)((size_t)addr - sizeof(size_t));
   size_t allocation_size;
   memcpy( &allocation_size, real_addr, sizeof(size_t) );
   free_blocks_[ real_addr ] = allocation_size;

   return;
}

}


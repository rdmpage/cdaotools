#include "shm_heap.hpp"
#include <strings.h>
#include <cstdlib>
#include <map>
#include <errno.h>
#include <iostream>
#include <algorithm>

using namespace std;
namespace CDAO {

  static const size_t BLOCK_SIZE = 1024;
  
  struct block_utilization_t {
      block_utilization_t(size_t ss=0, size_t  bu=0):slack_space(ss), blocks_used(bu){}
      size_t slack_space;
      size_t blocks_used;
  }; 

/*
 * Keep track of the start and extent of free blocks in the pool
 */
static std::map< void*, size_t > free_blocks_;
static std::map< void*, block_utilization_t > allocated_blocks_;
//static std::vector< bool > block_pool_;

static void shm_defragment();
static std::map< void*, size_t >::iterator find_slot( size_t size_requested );

/*
 * Initialize the memory segment.
 */
void shm_init( void* base_addr, size_t segment_size ){
   free_blocks_ = std::map< void*, size_t >();
   allocated_blocks_ = std::map< void*, block_utilization_t  >();
  // block_bool_       = std::vector< bool >( segment_size / BLOCK_SIZE );
   free_blocks_[ base_addr ] = segment_size;
   return;
}
/*
 * Release the segment.
 */
void shm_destroy(){
   free_blocks_.clear();
}


void* shm_malloc( size_t size ){
      
       std::map< void*, size_t >::iterator block = find_slot( size );
       //Not enough free space to satisify the request.
       if ( free_blocks_.end() == block ){ 
         //defragment the space and try again.
         shm_defragment();
         block = find_slot(size);
         //we really are out of space.   
         if ( block == free_blocks_.end()) {errno = ENOMEM; return NULL;}
       }
       //We have enough space in the pool to satisify the request.
       void* block_base_addr        = block->first;
       size_t slack_space = size % BLOCK_SIZE;
       size_t num_blocks_needed ;
       if (slack_space){  num_blocks_needed   = size < BLOCK_SIZE ? 1 : (  size / BLOCK_SIZE ) + 1; }
       else { num_blocks_needed   = size < BLOCK_SIZE ? 1 : (  size / BLOCK_SIZE );  }
       //figure out how much slack space will be left in the last block.
       size_t remaining_blocks = block->second - num_blocks_needed * BLOCK_SIZE;
       free_blocks_.erase( block );
       free_blocks_[ (void*)( (size_t)block_base_addr + num_blocks_needed * BLOCK_SIZE) ] = remaining_blocks;
       allocated_blocks_[ block_base_addr ] = block_utilization_t( slack_space, num_blocks_needed );
      
       //std::cerr << "Allocated: " << num_blocks_needed << " blocks starting at: " << block_base_addr << std::endl;
       //std::cerr << "The pool now has: " << free_blocks_[ (void*)( (size_t)block_base_addr + num_blocks_needed * BLOCK_SIZE) ] << " bytes\n";

       return block_base_addr;
}


void* shm_calloc( size_t num_items, size_t size ){
        void* ret = shm_malloc( size * num_items );
        bzero( ret, size * num_items );
        return ret;
}

  /*
   * Realloate space in the segment. Has the same semantics as the stdlib realloc.
   */
  void* shm_realloc( void* addr, size_t new_size ){
     if (NULL == addr){ return shm_malloc( new_size ); }
     if (0 == new_size){ shm_free( addr ); return NULL; }
     void* ret = addr;
     std::map< void*, block_utilization_t >::iterator existing_allocation = allocated_blocks_.find( addr );
     size_t new_size_in_blocks = new_size < BLOCK_SIZE ? 1 : new_size / BLOCK_SIZE;
     size_t existing_size_in_bytes = existing_allocation->second.blocks_used * BLOCK_SIZE + BLOCK_SIZE - existing_allocation->second.slack_space; 
     if ( new_size_in_blocks <  existing_allocation->second.blocks_used ){
        //the allocation should shrink.
        size_t number_of_blocks_freed = ( existing_size_in_bytes - new_size )/BLOCK_SIZE;
        size_t current_blocks_used = existing_allocation->second.blocks_used;
        existing_allocation->second.blocks_used -= number_of_blocks_freed;
        existing_allocation->second.slack_space = 0;
        void* freed_region_start = (void*)((size_t)existing_allocation->first + existing_allocation->second.blocks_used * BLOCK_SIZE);
        free_blocks_[ freed_region_start ] = number_of_blocks_freed * BLOCK_SIZE;
        
     }
     else if ( new_size_in_blocks == existing_allocation->second.blocks_used){
        //the allocation should stay the same the slack space need only be updated.
        if (existing_size_in_bytes < new_size){
          //the allocation should grow.
          existing_allocation->second.slack_space += new_size % BLOCK_SIZE;
        }
        else if ( existing_size_in_bytes > new_size ){
          //the allocation should shrink.
          existing_allocation->second.slack_space -= new_size % BLOCK_SIZE;
        }  
     }
     else {
        //the allocation should grow.
       void* new_addr = shm_malloc( new_size );
       if (new_addr){
          memcpy( new_addr, addr, existing_allocation->second.blocks_used * BLOCK_SIZE );
          ret = new_addr;
          shm_free( addr );
       }
     }
     return ret;
  }


void shm_free( void* addr ){
    std::map< void*, block_utilization_t >::iterator block = allocated_blocks_.find( addr );

   // std::cerr << "Found the allocation for addr: " << addr << std::endl;

    free_blocks_[ addr ] = block->second.blocks_used * BLOCK_SIZE;

   // std::cerr << "Returing " << block->second.blocks_used << " blocks to the pool.\n";
   // std::cerr << "There are now: " << block->second.blocks_used * BLOCK_SIZE << " starting at: " << addr << " available in the pool\n";

    allocated_blocks_.erase( block );
    return;
}

std::map< void*, size_t >::iterator find_slot( size_t size ){
   std::map< void*, size_t>::iterator block;
   for ( block = free_blocks_.begin(); block != free_blocks_.end() && block->second < size; ++block );
   return block;
}



void shm_defragment(){
    size_t orig_block_count = free_blocks_.size();
    //size_t orig_max_block_size;
    //size_t current_max_block__size;
    do {
       orig_block_count = free_blocks_.size();
       for (std::map< void*, size_t>::iterator b = free_blocks_.begin(); b != free_blocks_.end(); ++b){
          std::map< void*, size_t >::iterator nb = free_blocks_.find( (void*)((size_t) b->first + b->second) );
          //std::cerr << "Looking at fragment: " << b->first << " with extent " << b->second << std::endl;
          if ( nb != free_blocks_.end() ){
             //std::cerr << "Found adjacent vacant blocks: " << b->first << " and " << nb->first << std::endl;
             //these fragments are adjacent merge them.
             b->second += nb->second;
             free_blocks_.erase( nb );
          }
       }
       //std::cerr << "Merged: " << orig_block_count - free_blocks_.size() << " fragments\n";
       //std::cerr << "Now have: " << free_blocks_.size() << " fragments\n";
    
    }while ( orig_block_count != free_blocks_.size() );
    return;
}


}


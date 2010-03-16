#include "shm_heap.hpp"
#include <strings.h>
#include <string.h>
#include <cstdlib>
#include <map>
#include <errno.h>
#include <iostream>
#include <cstdio>
#include <algorithm>

#include <string.h>

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


//#include <malloc.h>

using namespace std;
//namespace CDAO {
  /*
   * Set the managed heap block size to the system page size.
   */
  static const size_t BLOCK_SIZE = sysconf(_SC_PAGESIZE );
  
  //static bool initialized_ = false;
  //static bool bootstrap_mode_ = true;
  /*
   * True if the managed heap has been activated.
   */
  bool HeapManagerManager::isActivated_ = false;
  //default manager for client requests.
  HeapManager* HeapManagerManager::default_manager_ = NULL;
  //default manager for system requests.
  DefaultManager* HeapManagerManager::system_manager_ = NULL;
  //manager manager singleton
  HeapManagerManager* HeapManagerManager::instance_ = NULL;
  /*
   * Convert a number of blocks into their size in bytes
   */
  inline size_t blocks_to_bytes( unsigned long blocks ){ return blocks * BLOCK_SIZE; }
  /*
   * Convert a number of bytes into the number of blocks needed to store them.
   */
  inline size_t bytes_to_blocks( unsigned long bytes ){ return bytes % BLOCK_SIZE ? bytes/BLOCK_SIZE + 1 : bytes/BLOCK_SIZE ; }
  
  HeapManager::~HeapManager(){}

  /*
   * Allocate blocks directly from the system.
   */
  void* DefaultManager::malloc( size_t size){ return std::malloc( size ); }
  /*
   * Allocate blocks directly from the system. 
   */
  void* DefaultManager::calloc( size_t number, size_t size ){ return std::calloc(number, size); }
  /*
   * Return blocks directly to the system.
   */
  void  DefaultManager::free( void* trash ){ std::free(trash); return; }
  /*
   * Trivial implementation to satisify the heap manager interface.
   */
  region_t DefaultManager::getregion()const{ return region_t(); }
  DefaultManager::~DefaultManager(){}


/*
 * Keep track of the start and extent of free blocks in the pool
 */
//std::map< void*, size_t > PoolManager::free_blocks_;
//std::map< void*, size_t > PoolManager::allocated_blocks_;
//static std::vector< bool > block_pool_;

 // PoolManager::PoolManager(){}
//static void* find_slot( size_t size_requested );
/*
 * Initialize an expandable pool manager. 
 */
PoolManager::PoolManager( unsigned long size){

  //fprintf(stderr, "PoolManager( size:%d )\n", size);
     /*
      * Allocate pool space from the system.
      */
     void* pool = std::malloc( size );
     /*
      * Mark this as pool as self-allocated.
      */
     self_allocated_mem_ = true;
     //Initialize accounting structures.
     this->free_blocks_ = map< void*, unsigned long >();
     this->free_blocks_[ pool ] = DEFAULT_POOL_SIZE;

     this->allocated_blocks_ = map< void*, unsigned long >();
     this->instance_region_ = region_t( pool, DEFAULT_POOL_SIZE );
     
}
/*
 * Initialize an already allocated pool
 */
PoolManager::PoolManager( void* base_addr, unsigned long segment_size, bool self_allocated ){
    //Manage an already allocated pool.
    self_allocated_mem_ = self_allocated;
    this->free_blocks_ = map< void*, unsigned long >();
    this->free_blocks_[ base_addr ] = segment_size;
    this->allocated_blocks_ = map< void*, unsigned long >();
    this->instance_region_ = region_t(base_addr, segment_size);
    //this->next_segment_ = NULL;

}
/*
 * Release pool to the system.
 */
PoolManager::~PoolManager(){
   //fprintf(stderr, "PoolManager::~PoolManager\n");
    this->free_blocks_.clear();
    this->allocated_blocks_.clear();
    if (self_allocated_mem_ ){
        std::free( this->instance_region_.getbase() );
    }
}

/*
 * Attempt to allocate some space in the pool.
 * If the pool is expandable, then the pool may be expanded to satisify the request.
 */

void* PoolManager::malloc( unsigned long size ){
   
   //fprintf(stderr, "%p->malloc( size: %lu )\n",this, size);
       
       void* block_base_addr;
       std::map< void*, size_t >::iterator block = find_slot( size );
       //Not enough free space to satisify the request.
       if ( this->free_blocks_.end() == block ){ 
         //defragment the space and try again.
         this->shm_defragment();
         block = this->find_slot(size);
         //we really are out of space in this segment.   
         if ( block == this->free_blocks_.end()) {
           //if this is self-allocated on the real heap then allocate a new region.
           if ( this->self_allocated_mem_ ){
             block_base_addr = std::malloc( std::max(size, DEFAULT_POOL_SIZE ) );
             if (block_base_addr ){
                  this->free_blocks_[ block_base_addr ] = std::max( size, DEFAULT_POOL_SIZE );
                  block = find_slot( size );
                  //return this->malloc( size );
             }
             else { errno = ENOMEM; return NULL; }
           }
         }
       } 
       //We have enough space in the pool to satisify the request.
       block_base_addr        = block->first;
       //size_t slack_space = size % BLOCK_SIZE;
       unsigned long num_blocks_needed = bytes_to_blocks( size );
       unsigned long remaining_bytes = block->second - blocks_to_bytes( num_blocks_needed );
       this->free_blocks_.erase( block );
       this->free_blocks_[ (void*)( (size_t)block_base_addr + blocks_to_bytes( num_blocks_needed ) ) ] = remaining_bytes;
       this->allocated_blocks_[ block_base_addr ] =  num_blocks_needed ;
       return block_base_addr;
}
/*
 * Allocate some space from the pool and initialize it to 0.
 */
void* PoolManager::calloc( unsigned long num_items, unsigned long size ){
        void* ret = PoolManager::malloc( size * num_items );
        bzero( ret, size * num_items );
        return ret;
}

  /*
   * Realloate space in the segment. Has the same semantics as the stdlib realloc.
   */
/*
  void* CDAO::shm_realloc( void* addr, size_t new_size ){
     if (NULL == addr){ return shm_malloc( new_size ); }
     if (0 == new_size){ shm_free( addr ); return NULL; }
     void* ret = addr;
     std::map< void*, block_utilization_t >::iterator existing_allocation = allocated_blocks_.find( addr );
     size_t new_size_in_blocks = bytes_to_blocks( new_size );
     size_t existing_size_in_bytes = blocks_to_bytes( existing_allocation->second ); 
     if ( new_size_in_blocks <  existing_allocation->second.blocks_used ){
        //the allocation should shrink.
        size_t number_of_blocks_freed = bytes_to_blocks( existing_size_in_bytes - new_size );
        size_t current_blocks_used = existing_allocation->second;
        existing_allocation->second -= number_of_blocks_freed;
        //existing_allocation->second.slack_space = 0;
        void* freed_region_start = (void*)((size_t)existing_allocation->first + blocks_to_bytes( existing_allocation->second ) );
        free_blocks_[ freed_region_start ] = blocks_to_bytes( number_of_blocks_freed );
        
     }
     else if ( new_size_in_blocks == existing_allocation->second.blocks_used){
       //do nothing the object already has enough space in it's block.
     }
     else {
        //the allocation should grow.
       void* new_addr = shm_malloc( new_size );
       if (new_addr){
          memcpy( new_addr, addr, blocks_to_bytes( existing_allocation->second ) );
          ret = new_addr;
          shm_free( addr );
       }
     }
     return ret;
  }
*/

/*
 * Return an allocation to the pool.
 */
void PoolManager::free( void* addr ){
    std::map< void*, unsigned long >::iterator block = allocated_blocks_.find( addr );
    if ( block != allocated_blocks_.end() ){
       free_blocks_[ addr ] = blocks_to_bytes( block->second );
       allocated_blocks_.erase( block );
    }
    return;
}
/*
 * Find a free block large enough to accomidate the specified size.
 */
std::map< void*, unsigned long >::iterator PoolManager::find_slot( size_t size ){
   std::map< void*, unsigned long>::iterator block;
   //fprintf(stderr, "Looking for a %lu byte block\n", size);
   for ( block = free_blocks_.begin(); block != free_blocks_.end() && block->second <  size ; ++block );
   return block;
}
/*
 * Defragment adjacent blocks in the pool.
 */
void PoolManager::shm_defragment(){
    size_t orig_block_count = free_blocks_.size();
    //size_t orig_max_block_size;
    //size_t current_max_block__size;
    do {
       orig_block_count = free_blocks_.size();
       for (std::map< void*, unsigned long>::iterator b = free_blocks_.begin(); b != free_blocks_.end(); ++b){
          std::map< void*, unsigned long >::iterator nb = free_blocks_.find( (void*)((size_t) b->first + b->second) );
          if ( nb != free_blocks_.end() ){
             //these fragments are adjacent merge them.
             b->second += nb->second;
             free_blocks_.erase( nb );
          }
       }
    
    }while ( orig_block_count != free_blocks_.size() );
    return;
}
/*
 * True if the heap-manager manager is active.
 */
bool HeapManagerManager::isActivated(){ return isActivated_; }
//activate the manager.
void HeapManagerManager::activate(){ 
 //trigger initialization while still managed by std::malloc. 
  getInstance();
  //set flag for new to start using the shm manager.
  isActivated_ = true; 
}
//use system allocation and free for the manager.
void* PoolManager::operator new( unsigned long size ){ return std::malloc(size); }
void PoolManager::operator delete( void*  trash ){ return std::free(trash); }

HeapManagerManager::HeapManagerManager(HeapManager* dflt  ){
       unsigned INIT_SIZE = 512;
       manager_vector_ = (HeapManager**)std::calloc( INIT_SIZE, sizeof(HeapManager*) );
       manager_index_  = (region_t*)std::calloc( INIT_SIZE, sizeof(region_t) );
       manager_vector_[0] = dflt;
       manager_index_[0] = dflt->getregion();
       managers_end_ = 1;
       managers_max_available_space_ = INIT_SIZE;
       //isActivated_ = false;
       default_manager_ = dflt;
       
}

HeapManagerManager* HeapManagerManager::getInstance( HeapManager* dflt ){
      if (NULL == instance_){
         instance_ = new HeapManagerManager( dflt );
         default_manager_ = dflt;
         system_manager_ = new DefaultManager();
         //initialized_ = true;
      }
      return instance_;
}

HeapManagerManager* HeapManagerManager::getInstance(){
     if (NULL == instance_ ){
          instance_ = getInstance( new PoolManager() );
          system_manager_ = new DefaultManager();
          //initialized_ = true;
     }
     return instance_;
}

HeapManager* HeapManagerManager::addManager( HeapManager* manager ){
      if ( managers_end_ >= managers_max_available_space_ ){
          manager_vector_ = (HeapManager**)std::realloc( manager_vector_, managers_max_available_space_ * 2 );
          manager_index_  = (region_t*)std::realloc( manager_index_, managers_max_available_space_ * 2 );
          managers_max_available_space_ *= 2;
      }
      manager_vector_[ managers_end_ ] = manager;
      manager_index_[ managers_end_ ] = manager->getregion();
      managers_end_++;
      return manager;

}

HeapManager* HeapManagerManager::addManager( void* base, unsigned long extent ){
     PoolManager* nheap = new PoolManager( base, extent );
     return addManager( nheap );
}

HeapManager* HeapManagerManager::setDefault( HeapManager* manager ){
     HeapManager* old_default = default_manager_;
     default_manager_ = manager;
     return old_default;
}

HeapManager* HeapManagerManager::getManager( region_t key ){
     unsigned ret_ind = find_manager( key );
     if ( ret_ind < managers_end_ ){ return manager_vector_[ ret_ind ]; }
     return NULL;
}

void* HeapManagerManager::malloc( unsigned long size ){
    //isActivated_ = false;
    HeapManager* current = default_manager_;
    setDefault( system_manager_ );
    void* ret = default_manager_->malloc( size );
    setDefault( current );
    //isActivated_ = true;
    return ret;
}

void* HeapManagerManager::malloc( unsigned long size, region_t key ){
   unsigned ind = find_manager( key );
   void* ret = NULL;
   HeapManager* current = default_manager_;
   setDefault( system_manager_ );
   if ( ind < managers_end_ ){
      ret=  manager_vector_[ ind ]->malloc( size );
   }
   setDefault( current );
   return ret;
}

void* HeapManagerManager::calloc( unsigned long number, unsigned long size ){
    void* ret = NULL;
    HeapManager* current = default_manager_;
    setDefault( system_manager_ );
    ret= default_manager_->calloc(number, size);
    setDefault( current );
    return ret;
}

void* HeapManagerManager::calloc( unsigned long number, unsigned long size, region_t key ){
    unsigned ind = find_manager( key );
    void* ret = NULL;
    HeapManager* current = default_manager_;
    setDefault( system_manager_ );

    if ( ind < managers_end_ ){
        ret= manager_vector_[ ind ]->calloc( number, size );
    }
    setDefault( current );
    return ret;
}

void HeapManagerManager::free( void* trash ){
  unsigned ind = find_manager( trash );
 // isActivated_ = false;
  if ( ind < managers_end_ ){
    HeapManager* current = default_manager_;
    setDefault( system_manager_ );

     manager_vector_[ ind ]->free( trash );
     setDefault( current );

  }
 // else {
    // std::free( trash );
  //}
 // isActivated_ = true;
  return;
}

unsigned HeapManagerManager::find_manager( region_t key ){
     unsigned ret;
     for ( ret = 0; ret < managers_end_ && (manager_index_[ret]) != key.getbase(); ++ret);
     return ret;
}

unsigned HeapManagerManager::find_manager( void* key ){
     unsigned ret;
     for ( ret = 0; ret < managers_end_ && (manager_index_[ret]) != key; ++ret);
     return ret;
}


void* HeapManagerManager::operator new( unsigned long size ){
    return std::malloc( size );
}

void HeapManagerManager::operator delete( void* trash ){
    HeapManagerManager* t = static_cast< HeapManagerManager* >( trash );
    if ( t ){
      std::free( t->manager_index_ );
       for (unsigned i = 0; i < t->managers_end_; ++i) delete t->manager_vector_[i];
       std::free( t->manager_vector_ );
       std::free( trash );
    }
    return;
}



MmappedHeapManager::MmappedHeapManager( unsigned long size ):PoolManager(0){
  void* segment_;
  char* ttemplate = "sharedshmXXXXXX";
  size_t len = strlen( ttemplate );
  char* rtempl = (char*)std::calloc( len, sizeof(char)+1 );
  strncpy( rtempl, ttemplate, len );
  if (( fd = mkstemp( rtempl ) ) < 0 ){ perror("mkstemp failed"); exit( 1 ); }
  if (lseek(fd, size, SEEK_END) < 0){ perror("Unable to seek in mmapped file"); exit(1); }
  write(fd, "", 1);
  if ( (long)(segment_ = mmap(NULL, size,  PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0)) < 0 ){
      perror("Unable to map shared area");
      exit(1);
  }
  //initialize members.
  this->self_allocated_mem_ = false;
 // this->free_blocks_ = map< void*, unsigned long >();
  this->free_blocks_[ segment_ ] = size;
  //this->allocated_blocks_ = map< void*, unsigned long >();
  this->instance_region_ = region_t(segment_, size);

  //(*PoolManager::manager_)[ region_t( segment_, size ) ] = this;
  unlink( rtempl );
  std::free( rtempl );
  close( fd );

}
MmappedHeapManager::~MmappedHeapManager(){
  munmap( this->instance_region_.getbase() , this->instance_region_.getlimit() );
}



void* operator new(unsigned long size ){
   void* ret;
   //fprintf(stderr, "Request to allocate %lu\n", size);
   if ( HeapManagerManager::isActivated() ){
     HeapManagerManager* shm = HeapManagerManager::getInstance();
       ret = shm->malloc( size );
   }
   else {
      ret = std::malloc( size );
   }
   return ret;
}


void* operator new(unsigned long size, unsigned long num ){
   void* ret;
   //if ( HeapManagerManager::isActivated() ){
       ret = HeapManagerManager::getInstance()->malloc( size );
   //}
   //else {
   //   ret = std::malloc( size );
   //}
   return ret;
} 

void operator delete( void* trash ){
  //if ( HeapManagerManager::isActivated() ){
     static HeapManagerManager* shm = HeapManagerManager::getInstance();
     shm->free( trash );
 // }
  //else { std::free( trash ); }
  return;
}

void operator delete[]( void* trash ){
 // if ( HeapManagerManager::isActivated() ){
     static HeapManagerManager* shm = HeapManagerManager::getInstance();
     shm->free( trash );
  //}
  //else { std::free( trash ); }
  return;
}


//}


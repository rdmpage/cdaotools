#ifndef SHM_HEAP_HPP_
#define SHM_HEAP_HPP_

#include <sys/types.h>
#include <map>

//namespace CDAO {
  const size_t DEFAULT_POOL_SIZE = 1024 * 1024 * 128;
  /*
   * Defines a region of memory.
   */
  class region_t {
    public:
     /*
      * Create a region with the specified base and extent.
      */
     region_t(void* base=NULL, size_t limit=0):base_(base), limit_(limit){  }
     /* Region copy */
     region_t(const region_t& rhs){ base_ = rhs.base_; limit_ = rhs.limit_; }
     /* Region copy */
     region_t& operator=( const region_t& rhs ){ base_ = rhs.base_; limit_ = rhs.limit_; return *this; }
     /* operator less for storage in STL containers. */
     inline bool operator<( const region_t& rhs )const{ return rhs.base_ < this->base_;  }
     /* check regions for base pointer equality */
     inline bool operator==( const region_t& rhs )const{ return rhs.base_ == this->base_; }
     /* true if the pointer is in the range defined by the region. */
     inline bool operator==( const void* rhs )const{ return rhs >= this->base_ && (size_t)rhs <= ((size_t)this->base_ + this->limit_ ); }
     /* true if the pointer is not in teh range defined by the region. */
     inline bool operator!=( const void* rhs )const{ return !( *this == rhs ); }
     /* get the base pointer */
     inline void* getbase()const{ return base_; }
     /* get the limit */
     inline size_t getlimit()const{ return limit_; }
    private:
     void* base_;
     size_t limit_;
  };


  class HeapManager {
    public:
       virtual ~HeapManager();
       virtual void* malloc( size_t )=0;
       virtual void* calloc( size_t, size_t )=0;
       virtual void free( void* )=0;
       virtual region_t getregion()const=0;
  };

  class DefaultManager : public HeapManager {
     public:
      virtual ~DefaultManager();
      virtual void* malloc( size_t size);
      virtual void* calloc( size_t number, size_t size );
      virtual void free( void* );
      virtual region_t getregion()const;
  };

  /*
   * Define a heap manager.
   */
  class PoolManager : public HeapManager {
    public:
       /*
        * Define the heap manager with the specified size heap.
        */
       PoolManager(unsigned long size = DEFAULT_POOL_SIZE);
       /*
        * Define the manager with an existing pool of memory.
        */
       PoolManager( void* base_addr, size_t segment_size, bool self_allocated = false );
       /*
        * Return the pool to the system.
        */
       virtual ~PoolManager();
     protected:
       //PoolManager();
       /*
        * Defragment free blocks in the pool
        */
       void shm_defragment();
       std::map< void*, size_t >::iterator find_slot( size_t size_requested );
    public:
       /*
        * Allocate a block of of the specified size from the pool.
        */
        void* malloc( size_t allocation_size );
       /*
        * Allocate and and initialze to 0. A block of memory of the specified size.
        */
        void* calloc( size_t allocation_size, size_t num_items );
       /*
        * Realloate space in the segment. Has the same semantics as the stdlib realloc.
        */
        //static void* shm_realloc( void* addr, size_t new_size );
       /*
        * Return the specified block to the free pool.
        */
        void free( void* ptr );
        /*
         * Get the region managed by this heap.
         */
        region_t getregion()const{ return instance_region_; }
        /*
         * Allocate the manager itself using the system allocator
         */
        static void* operator new( unsigned long size );
        /*
         * Free the manager itself using the system free.
         */
        static void operator delete(void*);
     protected:
        bool self_allocated_mem_;
        region_t instance_region_;
        /*
         * Keep track of the start and extent of free blocks in the pool.
         */
        std::map< void*, unsigned long > free_blocks_;
        /*
         * Keep track of the start and extent of allocations in the pool.
         */
        std::map< void*, unsigned long > allocated_blocks_;

  };
  /*
   * Manages a collection of heap segment managers.
   * Allows client specification of which manager to use for particular allocations.
   */
  class HeapManagerManager {
    protected:
      /*
       * Initialize and set the default manager to the specified manager.
       */
      HeapManagerManager( HeapManager* dflt );
    public:
      /*
       * Get the instance replacing the default with the specified manager.
       */
      static HeapManagerManager* getInstance( HeapManager* dflt );
      /*
       * Get the instance initializing with a default manager of the private heap.
       */
      static HeapManagerManager* getInstance();
      /*
       * Add a new manager to the set of managers.
       */
      HeapManager* addManager( HeapManager* manager );
      /*
       * Create and add a manager based on the specified memory regoin.
       */
      HeapManager* addManager( void* base, size_t extent );
      /*
       * Specify a new manager as the default.
       * Returns the old default manager.
       */
      HeapManager* setDefault( HeapManager* manager );
      /*
       * Get the manager associated with the specified region.
       */
      HeapManager* getManager( region_t key );
      /*
       * True if the manager is operating
       * The manager is started in an inactive state until 
       * system initialization has been completed.
       */
      static bool isActivated();
      /*
       * Activate management.
       */
      static void activate();
      /*
       * Allocate a block of memory with the default manager.
       */
      void* malloc( unsigned long );
      /*
       * Allocate a block of memeory with the specified manager.
       */
      void* malloc( unsigned long, region_t key );
      /*
       * Allocate and initilize to 0 a block of memory with the default manager.
       */
      void* calloc( unsigned long, unsigned long );
      /*
       * Allocate and initialize a block of memory with the specified manager.
       */
      void* calloc( unsigned long, unsigned long, region_t key );
      /*
       * Free a block of memory (even if it was alloated with the system malloc).
       */
      void free( void* );
      /*
       * Allocate the manager itself with the system allocator
       */
      static void* operator new( size_t );
      /*
       * Free the manager itself using the system free.
       */
      static void operator delete( void* trash );
    private:
      /*
       * Pointer to the shm heap manager instance.
       */
      static HeapManagerManager* instance_;
      /*
       * Find the manager associated with the specified region.
       */
      unsigned find_manager( region_t key );
      /*
       * Find the manager whose region contains the key.
       */
      unsigned find_manager( void* key );
      /*
       * Track the number of managers in the system
       */
      unsigned managers_end_;
      /*
       * Track the space allocated for managers.
       */
      unsigned managers_max_available_space_;
      /*
       * Default manager.
       */
      static HeapManager* default_manager_;
      static DefaultManager* system_manager_;
      /*
       * Managers
       */
       HeapManager** manager_vector_;
      /*
       * Index of managers.
       */
      region_t* manager_index_;
      /*
       * Activation flag
       */
      static bool isActivated_;

  };

  class MmappedHeapManager : public PoolManager {
     public: 
       MmappedHeapManager( unsigned long size );
       virtual ~MmappedHeapManager();
     private:
       int fd;

  };

//}
#endif

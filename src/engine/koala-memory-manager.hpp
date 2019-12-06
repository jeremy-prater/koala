#include <mutex>
#include <string>
#include <unordered_map>

// clang-format off
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Koala Memory Manager Header
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//					//			<----------------------------------		<---------------------------
//	MEMORY			//	MEMORY		//	BIN		//             //	MEMORY	//		APP		//
//					//				//			//      EMPTY  //			//				//
//	SEGMENT			//	CHUNK		//	ENTRY	//             //	CHUNK	//		DATA	//
//					//				//		-------> next      //			//				//
//				------->			//			//   bin       //			//				//
//					//			----------------------------------->	--------------------------->
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//										   |
//										m_Bins
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// clang-format on

using namespace std;

class MemorySegment;
class MemoryChunk;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Memory Manager
class KoalaMemoryManager {
public:
  unordered_map<string, MemorySegment> memorySegments;

public:
  KoalaMemoryManager();
  ~KoalaMemoryManager();

  [[nodiscard]] MemorySegment &CreateMemorySegment(string name,
                                                   size_t size) noexcept;
  void ReleaseMemorySegment(string name) noexcept;

  [[nodiscard]] MemorySegment &GetSegmentByResourceType(string name) const
      noexcept;
  void Shutdown() noexcept;

  //////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Memory Manager Constants

public:
  static const unsigned int numberOfExactBins = 0x00000080;
  static const unsigned int fixedBinRoR =
      0x00000004; // Number of bits to shift right for binStride (MUST BE POWER
                  // OF 2!!)
  static const unsigned int fixedBinStride = 1 << fixedBinRoR;
  static const size_t AllocAlign = fixedBinStride;

private:
  static size_t AdjustAllocSize(size_t size);
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Memory Chunk

enum class MemoryChunkStatus : uint32_t {
  Free = 0x7FFFFFFF,
  Used = 0x80000000
};

class MemoryChunk {
public:
  MemoryChunk();
  ~MemoryChunk();
  [[nodiscard]] unsigned int GetLength() const noexcept;
  [[nodiscard]] bool IsUsed() const noexcept;

  void SetLength(unsigned int size) noexcept;
  void SetState(const unsigned int State) noexcept;
  void Resize(size_t size) noexcept;
  void Release() noexcept;

private:
  unsigned int m_Length;    // 4 Bytes (1- Bit for usage flag / 31 - Bits for
                            // Length : Length = 0 - 2GB)
  MemoryChunk *p_Next;      // 4 Bytes
  MemoryChunk *p_Previous;  // 4 Bytes
  MemorySegment *p_Segment; // 4 Bytes / Future use / Alignment
  // unsigned int m_LastOperation; // 4 bytes Creation Method
  // unsigned int Padding[11]; // 44 Bytes of Padding
  // 64 Bytes Total - Cache Line
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Memory Bins
struct BinEntry {
  BinEntry *p_NextBin; // 4 Bytes
  BinEntry *p_PrevBin; // 4 Bytes
                       // 4 Bytes Total
};

class MemoryBin {
public:
  MemoryBin();
  ~MemoryBin();

  BinEntry *p_FirstBin; // 4 Bytes
  BinEntry *p_LastBin;  // 4 Bytes
  // 8 Bytes Total

  void AddEntry(MemoryChunk *chunk);
  void RemoveEntry(MemoryChunk *chunk);
  void RemoveEntry(BinEntry *chunk);
};

class MemoryBins {
public:
  MemoryBin m_Bins[KoalaMemoryManager::numberOfExactBins];
  MemoryBin &operator[](unsigned int size) {
    return m_Bins[((size) >> KoalaMemoryManager::fixedBinRoR) - 1];
  }
  void Reset();
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Memory Segment
class MemorySegment {
public:
  MemorySegment(string segmentName);
  ~MemorySegment();

  void CoalesceSegment();
  void ResetSegment();
  void ResizeSegment(size_t newSize);
  bool Validate();
  void Destroy();
  [[nodiscard]] unsigned int GetSegUsed() const noexcept;

  [[nodiscard]] MemoryChunk *AttemptAllocateChunk(size_t datasize) noexcept;
  [[nodiscard]] MemoryChunk *AllocateChunk(size_t size) noexcept;
  [[nodiscard]] MemoryChunk *ResizeChunk(MemoryChunk *chunk,
                                         size_t newdatasize) noexcept;
  void ReleaseChunk(MemoryChunk *chunk);

  void *Allocate(size_t size);
  void *Resize(void *chunk, size_t newdatasize);
  void Release(void *Memory);

  void FindBestDV();
  [[nodiscard]] float GetFragmentationPercent() const noexcept;
  [[nodiscard]] size_t GetTotalFreeSpace() const noexcept;
  [[nodiscard]] size_t GetEndFreeSpace() const noexcept;
  [[nodiscard]] const string GetName() const noexcept;

private:
  MemoryBins m_Bins;
  mutex m_Lock;

  size_t m_ReserveSize;
  size_t m_CommitSize;
  size_t m_Pagesize;
  const string name;
  MemoryChunk *p_FirstChunk;
  MemoryChunk *p_DV;
  MemoryChunk *p_WildernessChunk;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// User Memory
// class UserMemory {
// public:
//   MemorySegment *p_Segment;
//   void *operator new(size_t size, MemorySegment *p_Seg);
//   void operator delete(void *p, MemorySegment *p_Seg);
//   void operator delete(void *p);
//   LPSTR CopyString(LPSTR source);

//   UserMemory(void *address);
// };
//
// AV Memory Manager
//
// 2011 - P-Dev
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#include "AVMain.h"

// Static Definitions
SYSTEM_INFO AVMemoryManager::m_Info;
MemorySegment * AVMemoryManager::p_MemorySegments [AVMemoryManager_Default_Max_Segments];


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Memory Manager
AVMemoryManager::AVMemoryManager ()
{
	GetSystemInfo (&m_Info);
	ZeroMemory (p_MemorySegments,sizeof (MemorySegment *) * AVMemoryManager_Default_Max_Segments);
}
MemorySegment * AVMemoryManager::CreateMemorySegment (unsigned int segnum, unsigned int commitSize,unsigned int reserveSize)
{
	commitSize = (commitSize + (m_Info.dwPageSize -  1 )) & ~(m_Info.dwPageSize - 1); // Round sizes to nearest Largest Page size
	reserveSize = (reserveSize + (m_Info.dwPageSize -  1 )) & ~(m_Info.dwPageSize - 1);
	if (segnum >= AVMemoryManager_Default_Max_Segments)
		return 0;
	if (p_MemorySegments[segnum]!=0)
		return 0;
	MemorySegment * newseg = (MemorySegment *)VirtualAlloc (NULL,reserveSize,MEM_RESERVE,PAGE_READWRITE);
	if (!newseg)
		return 0;
	if (VirtualAlloc (newseg,commitSize,MEM_COMMIT,PAGE_READWRITE)!=0)
		newseg->m_CommitSize = commitSize;
	else
		return 0;
	InitializeCriticalSection (&newseg->m_Lock);
	EnterCriticalSection (&newseg->m_Lock);
	newseg->m_Bins.Reset();
	newseg->m_ReserveSize = reserveSize;
	newseg->m_SegID = segnum;
	newseg->m_Pagesize = m_Info.dwPageSize;
	p_MemorySegments [segnum] = newseg;
	MemoryChunk * chunk = newseg->p_WildernessChunk = newseg->p_DV = newseg->p_FirstChunk = (MemoryChunk*)((DWORD)newseg + sizeof (MemorySegment));
	chunk->SetLength (commitSize - sizeof (MemorySegment) - sizeof (MemoryChunk));
	chunk->p_Segment = newseg;
	chunk->p_Next = 0;
	chunk->p_Previous = 0;
	chunk->SetState (AVMemoryManager_MemoryChunk_Status::Free);
	LeaveCriticalSection (&newseg->m_Lock);
#ifdef _AV_PROFILE
	AVProfiler.SendNetworkPacket (newseg);
	AVProfiler.SendNetworkPacket (chunk);
	AVProfiler.SendNetworkCommand (AVProfiler_Command::Memory::Response::Calc_Update,newseg->m_SegID);
	AVProfiler.SendNetworkLog (AVProfiler_Command::General::Response::Memory_Log,1,"Segment : %u Created",newseg->m_SegID);
#endif
	return newseg;
}
void AVMemoryManager::ReleaseMemorySegment (unsigned int segnum)
{
	if (segnum >= AVMemoryManager_Default_Max_Segments)
		return;
	if (p_MemorySegments[segnum])
	{
#ifdef _AV_PROFILE
		bool used = false;
		MemoryChunk * fc = p_MemorySegments[segnum]->p_FirstChunk;
		while (fc)
		{
			used |= fc->IsUsed();
			fc = fc->p_Next;
		}
		if (used)
			AVProfiler.SendNetworkLog (AVProfiler_Command::General::Response::Memory_Log,1,"Attempted to Release Segment %d with Used Memory in it.",segnum);
#endif
		DeleteCriticalSection (&p_MemorySegments[segnum]->m_Lock);
		VirtualFree (p_MemorySegments[segnum],0,MEM_RELEASE);
		p_MemorySegments [segnum] = 0;
#ifdef _AV_PROFILE
		AVProfiler.SendNetworkCommand (AVProfiler_Command::Memory::Response::Seg_Free,segnum);
		AVProfiler.SendNetworkLog (AVProfiler_Command::General::Response::Memory_Log,1,"Segment : %u Released",segnum);
#endif
	}
}
MemorySegment * AVMemoryManager::GetSegmentByResourceType (unsigned int Type)
{
	switch (Type)
	{
		case AVResource_Index_Type::Render_Material:
		case AVResource_Index_Type::Render_Model:
		case AVResource_Index_Type::Render_Texture:
		case AVResource_Index_Type::Render_Shader:
			return p_MemorySegments[AVMemoryManager_Segment::Render];
			
		case AVResource_Index_Type::PhysX_Actor:
		case AVResource_Index_Type::PhysX_Material:
		case AVResource_Index_Type::PhysX_TriangleMesh:
		case AVResource_Index_Type::PhysX_ConvexMesh:
		//case AVResource_Index_Type::PhysX_DeformMesh:
		case AVResource_Index_Type::PhysX_ClothMesh:
		case AVResource_Index_Type::PhysX_Object:
			return p_MemorySegments[AVMemoryManager_Segment::PhysXData];
		
		case AVResource_Index_Type::Game_Part:
		case AVResource_Index_Type::Game_Object:
			return p_MemorySegments[AVMemoryManager_Segment::GameData];
		
		case AVResource_Index_Type::Resource_File:
		case AVResource_Index_Type::Resource_Index:
			return p_MemorySegments[AVMemoryManager_Segment::Resource_Headers];

		default:
			return p_MemorySegments[AVMemoryManager_Segment::General];
	}
}
void AVMemoryManager::Shutdown()
{
	for (unsigned int d=0;d<AVMemoryManager_Default_Max_Segments;d++)
	{
		if (p_MemorySegments[d])
			p_MemorySegments[d]->Destroy();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Memory Segment
void MemorySegment::CoalesceSegment ()
{
#ifdef _AV_PROFILE
		//AVProfiler.SendNetworkCommand (AVProfiler_Command::General::Response::Memory_Seg_Coal,m_SegID);
		AVProfiler.SendNetworkLog (AVProfiler_Command::General::Response::Memory_Log,1,"Segment : %u Coalesced",m_SegID);
#endif
	EnterCriticalSection (&m_Lock);
	MemoryChunk * chunk = p_FirstChunk;
	MemoryChunk * nextchunk = 0;
	while (chunk->p_Next != 0) // Is Last Chunk, end loop
	{
		if (chunk->IsUsed ()) // If used, goto next and continue
		{
			chunk = chunk->p_Next;
			continue;
		}
		else // Is unused
		{
			nextchunk = chunk->p_Next; // Check if next chunk is unused
			if (!nextchunk->IsUsed()) // Make sure nextChunk is unused
			{
				// Combine
				if (chunk->GetLength() <= AVMemoryManager_Default_Number_of_Exact_Bins << AVMemoryManager_Default_Fixed_Bin_RoR)
				{
					// Remove chunk from bin
					m_Bins[chunk->GetLength()].RemoveEntry (chunk);
				}
				if (nextchunk->GetLength() <= AVMemoryManager_Default_Number_of_Exact_Bins << AVMemoryManager_Default_Fixed_Bin_RoR)
				{
					// Remove nextchunk from bin
					m_Bins[nextchunk->GetLength()].RemoveEntry (nextchunk);
				}
				if (nextchunk != p_WildernessChunk) // Make sure nextchunk is not the wilderness chunk.
					nextchunk->p_Next->p_Previous = chunk;
				else
					p_WildernessChunk = chunk;
				chunk->p_Next = nextchunk->p_Next;
				chunk->SetLength (chunk->GetLength () + nextchunk->GetLength() + sizeof (MemoryChunk));
				if (chunk->GetLength() <= AVMemoryManager_Default_Number_of_Exact_Bins << AVMemoryManager_Default_Fixed_Bin_RoR)
				{
					m_Bins[chunk->GetLength()].AddEntry (chunk);
				}
				//chunk->m_LastOperation = 5;
			}
			else // Nextchunk is used, skip to nextchunk->next and continue
			{
				if (nextchunk->p_Next)
					chunk = nextchunk->p_Next;
				else
					chunk = nextchunk;
			}
		}
	}
	FindBestDV();
	LeaveCriticalSection (&m_Lock);
#ifdef _AV_PROFILE
	AVProfilerNetworkPacketAssembler packet (AVProfiler_Command::Memory::Request::Segment_Dump,sizeof (unsigned int),&m_SegID);
	AVProfiler.DoNetworkCommand (packet.p_Packet);
	AVProfiler.SendNetworkCommand (AVProfiler_Command::Memory::Response::Calc_Update,m_SegID);
#endif
}
unsigned int MemorySegment::GetSegUsed()
{
	unsigned int used = 0;
	MemoryChunk * chunk = p_FirstChunk;
	while (chunk)
	{
		if (chunk->IsUsed ())
			used++;
		chunk = chunk->p_Next;
	}
	return used;
}
void MemorySegment::FindBestDV()
{
	// Linked list search through non-linear memory, OMG...

	MemoryChunk * chunk = p_FirstChunk;
	unsigned int largest = 0;
	unsigned int testsize = 0;
	while (chunk)
	{
		if (!chunk->IsUsed ())
		{
			testsize = chunk->GetLength();
			if ( testsize > largest)
			{
				largest = testsize;
				p_DV = chunk;
			}
		}
		chunk = chunk->p_Next;
	}
}
unsigned int MemorySegment::GetTotalFreeSpace()
{
	MemoryChunk * chk = p_FirstChunk;
	unsigned int free = 0;
	while (chk)
	{
		if (!chk->IsUsed())
			free += chk->GetLength();
		chk = chk->p_Next;
	}
	return free;
}
float MemorySegment::GetFragmentationPercent ()
{
	MemoryChunk * chk = p_FirstChunk;
	unsigned int free = 0;
	unsigned int used = 0;
	while (chk)
	{
		if (!chk->IsUsed())
			free += chk->GetLength();
		else
			used += chk->GetLength();
		chk = chk->p_Next;
	}
	free -= GetEndFreeSpace();
	return (float)free / (free+used);
}
unsigned int MemorySegment::GetEndFreeSpace()
{
	if (p_WildernessChunk->IsUsed ())
		return 0;
	else
		return p_WildernessChunk->GetLength ();
}
void MemorySegment::ResetSegment()
{
	EnterCriticalSection (&m_Lock);
	p_FirstChunk->p_Next = p_FirstChunk->p_Previous = 0;
	p_FirstChunk->SetLength (m_CommitSize - sizeof (MemoryChunk) - sizeof (MemorySegment));
	p_FirstChunk->SetState (AVMemoryManager_MemoryChunk_Status::Free);
	p_WildernessChunk = p_DV = p_FirstChunk;
	m_Bins.Reset ();
#ifdef _AV_PROFILE
		AVProfiler.SendNetworkCommand (AVProfiler_Command::Memory::Response::Seg_Res,&m_SegID,sizeof (unsigned int));
#endif
	LeaveCriticalSection (&m_Lock);
}
bool MemorySegment::Validate()
{
	EnterCriticalSection (&m_Lock);
	MemoryChunk * cur = p_FirstChunk;
	unsigned int size = 0;
	while (cur)
	{
		if (cur->p_Next)
			if ((DWORD)cur + cur->GetLength() + sizeof (MemoryChunk) != (DWORD)cur->p_Next)
			{
				LeaveCriticalSection (&m_Lock);
				return false;
			}
		size += cur->GetLength() + sizeof (MemoryChunk);
		cur = cur->p_Next;
	}
	size += sizeof (MemorySegment);
	if (size != m_CommitSize)
	{
		LeaveCriticalSection (&m_Lock);
		return false;
	}
	for (unsigned int d=0;d<AVMemoryManager_Default_Number_of_Exact_Bins;d++)
	{
		MemoryBin * bin = &m_Bins.m_Bins[d];
		if (bin->p_FirstBin == 0)
		{
			if (bin->p_LastBin != 0)
			{
				LeaveCriticalSection (&m_Lock);
				return false;
			}
		}
		else
		{
			BinEntry * entry = bin->p_FirstBin;
			while (entry)
			{
				if ((DWORD)entry->p_NextBin)
				{
					if ((DWORD)entry->p_NextBin < (DWORD)this)
					{
						LeaveCriticalSection (&m_Lock);
						return false;
					}
					if (entry->p_NextBin->p_PrevBin != entry)
					{
						LeaveCriticalSection (&m_Lock);
						return false;
					}
				}
				if ((DWORD)entry->p_PrevBin)
				{
					if ((DWORD)entry->p_PrevBin < (DWORD)this)
					{
						LeaveCriticalSection (&m_Lock);
						return false;
					}
					if (entry->p_PrevBin->p_NextBin != entry)
					{
						LeaveCriticalSection (&m_Lock);
						return false;
					}
				}
				entry = entry->p_NextBin;
			}
		}
	}
	LeaveCriticalSection (&m_Lock);
	return true;
}
void * MemorySegment::Allocate (unsigned int datasize)
{
	MemoryChunk * mem = AllocateChunk (datasize);
	if (mem)
		return mem+1;
	else
		return 0;
}
void MemorySegment::Release (void * Memory)
{
	ReleaseChunk ( ((MemoryChunk*)Memory)-1);
}
void * MemorySegment::Resize (void * chunk,unsigned int newdatasize)
{
	if (!chunk)
		return Allocate (newdatasize);
	return (ResizeChunk ( ((MemoryChunk*)chunk)-1,newdatasize))+1;
}
MemoryChunk * MemorySegment::AttemptAllocateChunk (unsigned int datasize)
{
	EnterCriticalSection (&m_Lock);
	if (datasize < AVMemoryManager_Alloc_Align)
		datasize = AVMemoryManager_Alloc_Align;
	datasize = AdjustAllocSize (datasize);
	//if (size < sizeof (MemoryChunk) + AVMemoryManager_Default_Fixed_Bin_Stride) size = sizeof (MemoryChunk) + AVMemoryManager_Default_Fixed_Bin_Stride;
	unsigned int chunksize = datasize + sizeof (MemoryChunk);
	if (datasize <= AVMemoryManager_Default_Number_of_Exact_Bins << AVMemoryManager_Default_Fixed_Bin_RoR) // Check if size is Bin-able
	{
		BinEntry * bin = m_Bins[datasize].p_FirstBin;
		if (bin != 0) // Search Exact bin for chunk
		{
			// Found exact bin
			MemoryChunk * chk = (MemoryChunk*) ((DWORD)bin - sizeof (MemoryChunk)); // Move back to MemoryChunk Header from BinEntry
			if (chk != p_WildernessChunk)
			{
				chk->SetState (AVMemoryManager_MemoryChunk_Status::Used); // Reassign chunk to Used
				m_Bins[datasize].RemoveEntry (bin); // Set m_Bin to next bin (or 0 if empty)
				if (chk == p_DV)
					FindBestDV();

				//chk->m_LastOperation = 1;

				LeaveCriticalSection (&m_Lock);
#ifdef _AV_PROFILE
				AVProfiler.SendNetworkPacket (chk);
				AVProfiler.SendNetworkPacket (p_DV);
				AVProfiler.SendNetworkPacket (p_WildernessChunk);
				AVProfiler.SendNetworkCommand (AVProfiler_Command::Memory::Response::Calc_Update,m_SegID);
				AVProfiler.SendNetworkLog (AVProfiler_Command::General::Response::Memory_Log,3,"Chunk : Allocate (0x%08X) (size=%u)",chk,datasize);
#endif
				return chk;
			}
		}	

		// Search larger bins for split
		unsigned int nextDatasize = datasize + AVMemoryManager_Default_Fixed_Bin_Stride + sizeof (MemoryChunk);
		while (nextDatasize <= AVMemoryManager_Default_Number_of_Exact_Bins << AVMemoryManager_Default_Fixed_Bin_RoR)
		{
			BinEntry * bin = m_Bins[nextDatasize].p_FirstBin;
			if (bin != 0)
			{
				// Bin not empty, split
				MemoryChunk * chk = (MemoryChunk *) ((DWORD) bin - sizeof (MemoryChunk));
				m_Bins[nextDatasize].RemoveEntry (bin);
				MemoryChunk * splitchk = (MemoryChunk *) (((DWORD)chk) + chunksize);
				MemoryChunk * nextchk = chk->p_Next;
				chk->p_Next = splitchk;
				splitchk->p_Previous = chk;
				splitchk->p_Next = nextchk;				
				if (nextchk) // Wilderness chunk has no next!
					nextchk->p_Previous = splitchk;
				splitchk->p_Segment = chk->p_Segment;
				chk->SetLength (datasize);
				splitchk->SetLength (nextDatasize - datasize - sizeof (MemoryChunk));
				if ((splitchk->GetLength() <= AVMemoryManager_Default_Number_of_Exact_Bins << AVMemoryManager_Default_Fixed_Bin_RoR) && // Add splitchk to a bin
					(splitchk != p_WildernessChunk))																					// and not the wilderness chunk
					m_Bins[splitchk->GetLength()].AddEntry (splitchk);
				chk->SetState (AVMemoryManager_MemoryChunk_Status::Used);
				splitchk->SetState (AVMemoryManager_MemoryChunk_Status::Free);
				if (chk == p_WildernessChunk)
					p_WildernessChunk = splitchk; // Re-assign wilderness chunk if split
				if (chk == p_DV)
					FindBestDV();

				//chk->m_LastOperation = 2;
				//splitchk->m_LastOperation = 2;
				LeaveCriticalSection (&m_Lock);


#ifdef _AV_PROFILE
				AVProfiler.SendNetworkPacket (p_WildernessChunk);
				AVProfiler.SendNetworkPacket (p_DV);
				AVProfiler.SendNetworkPacket (splitchk);
				AVProfiler.SendNetworkPacket (chk);
				AVProfiler.SendNetworkCommand (AVProfiler_Command::Memory::Response::Calc_Update,m_SegID);
				AVProfiler.SendNetworkLog (AVProfiler_Command::General::Response::Memory_Log,3,"Chunk : Allocate (0x%08X) (size=%u)",chk,datasize);
#endif
				return chk;
			}
			nextDatasize += AVMemoryManager_Default_Fixed_Bin_Stride;
		}
	}

	// Split from non-binnable memory

	if (p_DV->GetLength() >= chunksize + AVMemoryManager_Default_Fixed_Bin_Stride) // Split the DV chunk
	{
		if (p_DV->GetLength () <= AVMemoryManager_Default_Number_of_Exact_Bins << AVMemoryManager_Default_Fixed_Bin_RoR)
			m_Bins[p_DV->GetLength ()].RemoveEntry (p_DV);
		MemoryChunk * newchunk = p_DV;
		p_DV = (MemoryChunk*) ((DWORD)newchunk + chunksize);

		p_DV->SetLength(newchunk->GetLength() - chunksize);
		p_DV->p_Segment = newchunk->p_Segment;

		p_DV->p_Previous = newchunk;
		p_DV->p_Next = newchunk->p_Next;
		newchunk->p_Next = p_DV;

		if (p_DV->p_Next)
			p_DV->p_Next->p_Previous = p_DV;


		newchunk->SetLength(datasize);

		p_DV->SetState (AVMemoryManager_MemoryChunk_Status::Free);
		newchunk->SetState (AVMemoryManager_MemoryChunk_Status::Used);
		if (newchunk == p_WildernessChunk)
			p_WildernessChunk = p_DV;
		if (p_DV->GetLength () <= AVMemoryManager_Default_Number_of_Exact_Bins << AVMemoryManager_Default_Fixed_Bin_RoR)
			m_Bins[p_DV->GetLength ()].AddEntry (p_DV);

		FindBestDV();

		//newchunk->m_LastOperation = 3;
		//p_DV->m_LastOperation = 3;

		LeaveCriticalSection (&m_Lock);

#ifdef _AV_PROFILE
		AVProfiler.SendNetworkPacket (p_WildernessChunk);
		AVProfiler.SendNetworkPacket (p_DV);
		AVProfiler.SendNetworkPacket (newchunk);
		AVProfiler.SendNetworkCommand (AVProfiler_Command::Memory::Response::Calc_Update,m_SegID);
		AVProfiler.SendNetworkLog (AVProfiler_Command::General::Response::Memory_Log,3,"Chunk : Allocate (0x%08X) (size=%u)",newchunk,datasize);
#endif

		return newchunk;
	}

	if (p_WildernessChunk->GetLength() >= chunksize + AVMemoryManager_Default_Fixed_Bin_Stride) // Split the wilderness chunk
	{
		MemoryChunk * newchunk = p_WildernessChunk;
		p_WildernessChunk = (MemoryChunk*) ((DWORD)newchunk + chunksize);

		p_WildernessChunk->SetLength(newchunk->GetLength() - chunksize);
		p_WildernessChunk->p_Segment = newchunk->p_Segment;
		p_WildernessChunk->p_Previous = newchunk;
		p_WildernessChunk->p_Next = 0;

		newchunk->p_Next = p_WildernessChunk;
		newchunk->SetLength(datasize);

		p_WildernessChunk->SetState (AVMemoryManager_MemoryChunk_Status::Free);
		newchunk->SetState (AVMemoryManager_MemoryChunk_Status::Used);

		
		if (p_WildernessChunk->GetLength() <= AVMemoryManager_Default_Number_of_Exact_Bins << AVMemoryManager_Default_Fixed_Bin_RoR)
			m_Bins[p_WildernessChunk->GetLength ()].AddEntry (p_WildernessChunk);
		
		//newchunk->m_LastOperation = 4;
		//p_WildernessChunk->m_LastOperation = 4;

		LeaveCriticalSection (&m_Lock);

#ifdef _AV_PROFILE
		AVProfiler.SendNetworkPacket (p_WildernessChunk);
		AVProfiler.SendNetworkPacket (p_DV);
		AVProfiler.SendNetworkPacket (newchunk);
		AVProfiler.SendNetworkCommand (AVProfiler_Command::Memory::Response::Calc_Update,m_SegID);
		AVProfiler.SendNetworkLog (AVProfiler_Command::General::Response::Memory_Log,3,"Chunk : Allocate (0x%08X) (size=%u)",newchunk,datasize);
#endif
		
		return newchunk;
	}
	LeaveCriticalSection (&m_Lock);
	AVProfiler.SendNetworkLog (AVProfiler_Command::General::Response::Memory_Log,3,"Chunk : Failed to Allocate (size=%u)",datasize);
	return 0;
}
MemoryChunk * MemorySegment::AllocateChunk (unsigned int datasize)
{
	MemoryChunk * test = AttemptAllocateChunk(datasize);
	if (test)
		return test;
	else
		CoalesceSegment(); // Try Again
	test = AttemptAllocateChunk(datasize);
	if (test)
		return test;
	else
		ResizeSegment(m_ReserveSize); // Try Again
	test = AttemptAllocateChunk(datasize); // Final Try
	if (test)
		return test;
#ifdef _AV_PROFILE	
		AVProfiler.SendNetworkLog (AVProfiler_Command::General::Response::Memory_Log,3,"Segment %u at 0x%08X out of memory. %u free space",this->m_SegID,this,GetTotalFreeSpace());
#endif
	return 0;
}
void MemorySegment::ReleaseChunk (MemoryChunk * chunk)
{
	if (chunk->IsUsed())
	{
		EnterCriticalSection (&m_Lock);
		chunk->SetState (AVMemoryManager_MemoryChunk_Status::Free);
		//chunk->m_LastOperation = 4;
		if (chunk->GetLength () <= AVMemoryManager_Default_Number_of_Exact_Bins << AVMemoryManager_Default_Fixed_Bin_RoR)
			m_Bins [chunk->GetLength()].AddEntry (chunk);
		LeaveCriticalSection (&m_Lock);
#ifdef _AV_PROFILE
		AVProfiler.SendNetworkPacket (chunk);
		AVProfiler.SendNetworkCommand (AVProfiler_Command::Memory::Response::Calc_Update,m_SegID);
		AVProfiler.SendNetworkLog (AVProfiler_Command::General::Response::Memory_Log,3,"Chunk : Released (0x%08X)",chunk);
#endif
	}
}
MemoryChunk * MemorySegment::ResizeChunk (MemoryChunk * chunk,unsigned int newdatasize)
{
	/*if (chunk->p_Next)
		if (!chunk->p_Next->IsUsed ())
			if (chunk->p_Next->GetLength () >= newdatasize - (chunk->GetLength () + sizeof (MemoryChunk)) )
			{
				bool used = chunk->IsUsed ();
				unsigned int nsize = chunk->p_Next->GetLength();
				chunk->p_Next = chunk->p_Next->p_Next;
				if (chun k->p_Next)
					chunk->p_Next->p_Previous = chunk;
				chunk->SetLength (nsize + sizeof (MemoryChunk) + chunk->GetLength ());
				chunk->SetState (used ? AVMemoryManager_MemoryChunk_Status::Used : AVMemoryManager_MemoryChunk_Status::Free);
				if (chunk->GetLength () >= 
			}*/ // KISS
#ifdef _AV_PROFILE
	AVProfiler.SendNetworkLog (AVProfiler_Command::General::Response::Memory_Log,3,"Chunk : Resized (0x%08X) %u -> %u",chunk,chunk->GetLength (),newdatasize);
#endif
	EnterCriticalSection (&m_Lock);
	if (newdatasize == chunk->GetLength ())
		return chunk;
	MemoryChunk * newchunk = AllocateChunk (newdatasize);
	if (newchunk)
	{
		memcpy (newchunk + 1, chunk + 1, chunk->GetLength());
		ReleaseChunk (chunk);
	}
	LeaveCriticalSection(&m_Lock);
	return newchunk;
}
void MemorySegment::ResizeSegment (unsigned int newCommitSize)
{
	// Untested!

	newCommitSize = (newCommitSize + (m_Pagesize -  1 )) & ~(m_Pagesize - 1); // Round sizes to nearest Largest Page size
	if (newCommitSize <= m_CommitSize)
		return;
	if (newCommitSize > m_ReserveSize)
		return;
	if (VirtualAlloc (this,newCommitSize,MEM_COMMIT,PAGE_READWRITE)!=0)
		m_CommitSize = newCommitSize;
}
void MemorySegment::Destroy()
{
	DeleteCriticalSection(&m_Lock);
	VirtualFree (this,0,MEM_RELEASE);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Memory Chunk
/*inline*/ unsigned int MemoryChunk::GetLength ()
{
	return m_Length & AVMemoryManager_MemoryChunk_Status::Free;
}
/*inline*/ void MemoryChunk::SetLength (unsigned int size)
{
	m_Length = size;
}
bool MemoryChunk::IsUsed ()
{
	if (m_Length & AVMemoryManager_MemoryChunk_Status::Used)
		return true;
	else
		return false;
}
void MemoryChunk::SetState (const unsigned int State)
{
	if (State == AVMemoryManager_MemoryChunk_Status::Used)
		m_Length |= AVMemoryManager_MemoryChunk_Status::Used;
	else
		m_Length &= AVMemoryManager_MemoryChunk_Status::Free;
}
void MemoryChunk::Release()
{
	p_Segment->ReleaseChunk (this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Memory Bins
void MemoryBins::Reset ()
{
	ZeroMemory (m_Bins,sizeof (m_Bins));
}
void MemoryBin::AddEntry (MemoryChunk * entry)
{
		if (!p_LastBin )
		{
			// Bin is empty, add new
			BinEntry * newbin = (BinEntry*) ((DWORD)entry + sizeof (MemoryChunk));
			p_FirstBin = p_LastBin = newbin;
			newbin->p_NextBin = newbin->p_PrevBin = 0;
		}
		else
		{
			// Bin has contents, append to end
			BinEntry * newbin = (BinEntry*) ((DWORD)entry + sizeof (MemoryChunk));
			p_LastBin->p_NextBin = newbin;
			newbin->p_PrevBin = p_LastBin;
			p_LastBin = newbin;
			newbin->p_NextBin = 0;
		}
}
void MemoryBin::RemoveEntry (BinEntry * entry)
{
	if (entry == p_FirstBin)
	{
		p_FirstBin = p_FirstBin->p_NextBin;
		if (p_FirstBin) 
				p_FirstBin->p_PrevBin = 0; // Make sure not 0x0!
		else
			p_LastBin = p_FirstBin;
		return;
	}
	if (entry == p_LastBin)
	{
		p_LastBin = p_LastBin->p_PrevBin;
		p_LastBin->p_NextBin = 0;
		if (p_LastBin == p_FirstBin)
			p_FirstBin->p_PrevBin = 0;
		return;
	}
	BinEntry * next = entry->p_NextBin;
	BinEntry * prev = entry->p_PrevBin;
	prev->p_NextBin = next;
	next->p_PrevBin = prev;
}
void MemoryBin::RemoveEntry (MemoryChunk * chunk)
{
	RemoveEntry ((BinEntry *) ((DWORD)chunk + sizeof (MemoryChunk)));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// User Memory

void * UserMemory::operator new (size_t size,MemorySegment * p_Seg)
{
	UserMemory * memory = (UserMemory*)p_Seg->Allocate (size);
	if (!memory)
	{
		AVProfiler.SendNetworkLog (AVProfiler_Command::General::Response::Memory_Log,1,"Segment %d out of memory",p_Seg->m_SegID);
		std::bad_alloc ba;
		throw (ba);
	}
	return memory;
}

UserMemory::UserMemory(void * address)
{
	MemoryChunk	* chunk = (MemoryChunk*)address;
	chunk--;
	this->p_Segment = chunk->p_Segment;
}

void UserMemory::operator delete(void * p,MemorySegment * p_Seg)
{
	if (p)
		((MemoryChunk *)p-1)->Release ();
}
void UserMemory::operator delete(void * p)
{
	((MemoryChunk *)p-1)->Release ();
}

LPSTR UserMemory::CopyString (LPSTR source)
{
	LPSTR output = (LPSTR)p_Segment->Allocate (strlen (source)+1);
	strcpy (output,source);
	return output;
}
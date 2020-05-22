#endif
}
  return p->pBt->nRef;
  testcase( p->sharable );
int sqlite3BtreeConnectionCount(Btree *p){
*/
** this is always 1. For shared caches it may be 1 or greater.
** the Btree handle passed as the only argument. For private caches 
** Return the number of connections to the BtShared object accessed by
/*

}
  return p->sharable;
int sqlite3BtreeSharable(Btree *p){
*/
** Return true if the Btree passed as the only argument is sharable.
/*
#if !defined(SQLITE_OMIT_SHARED_CACHE)

int sqlite3HeaderSizeBtree(void){ return ROUND8(sizeof(MemPage)); }
*/
** Return the size of the header added to each page by this module.
/*

}
  return (p->pBt->btsFlags & BTS_READ_ONLY)!=0;
int sqlite3BtreeIsReadonly(Btree *p){
*/
** Return true if the given Btree is read-only.
/*

}
  return (pCsr->hints & mask)!=0;
int sqlite3BtreeCursorHasHint(BtCursor *pCsr, unsigned int mask){
*/
** only used from within assert() statements
** Return true if the cursor has a hint specified.  This routine is
/*

}
  return rc;
  pBt->btsFlags &= ~BTS_NO_WAL;

  }
    }
      }
        }
          aData[19] = (u8)iVersion;
          aData[18] = (u8)iVersion;
        if( rc==SQLITE_OK ){
        rc = sqlite3PagerWrite(pBt->pPage1->pDbPage);
      if( rc==SQLITE_OK ){
      rc = sqlite3BtreeBeginTrans(pBtree, 2, 0);
    if( aData[18]!=(u8)iVersion || aData[19]!=(u8)iVersion ){
    u8 *aData = pBt->pPage1->aData;
  if( rc==SQLITE_OK ){
  rc = sqlite3BtreeBeginTrans(pBtree, 0, 0);

  if( iVersion==1 ) pBt->btsFlags |= BTS_NO_WAL;
  pBt->btsFlags &= ~BTS_NO_WAL;
  */
  ** WAL connection, even if the version fields are currently set to 2.
  /* If setting the version fields to 1, do not automatically open the

  assert( iVersion==1 || iVersion==2 );
 
  int rc;                         /* Return code */
  BtShared *pBt = pBtree->pBt;
int sqlite3BtreeSetVersion(Btree *pBtree, int iVersion){
*/
** header to iVersion.
** "write version" (single byte at byte offset 19) fields in the database
** Set both the "read version" (single byte at byte offset 18) and 
/*

#endif
}
  pCur->pBtree->hasIncrblobCur = 1;
  pCur->curFlags |= BTCF_Incrblob;
void sqlite3BtreeIncrblobCursor(BtCursor *pCur){
*/
** Mark this cursor as an incremental blob cursor.
/* 

}
  return accessPayload(pCsr, offset, amt, (unsigned char *)z, 1);

  assert( pCsr->pPage->intKey );
  assert( !hasReadConflicts(pCsr->pBtree, pCsr->pgnoRoot) );
  assert( hasSharedCacheTableLock(pCsr->pBtree, pCsr->pgnoRoot, 0, 2) );
              && pCsr->pBt->inTransaction==TRANS_WRITE );
  assert( (pCsr->pBt->btsFlags & BTS_READ_ONLY)==0
  }
    return SQLITE_READONLY;
  if( (pCsr->curFlags & BTCF_WriteFlag)==0 ){
  */
  **   (e) the cursor points at a valid row of an intKey table.
  **   (d) there are no conflicting read-locks, and
  **   (c) the connection holds a write-lock on the table (if required),
  **   (b) there is a read/write transaction open,
  **   (a) the cursor is open for writing,
  /* Check some assumptions: 

  assert( rc==SQLITE_OK );
  VVA_ONLY(rc =) saveAllCursors(pCsr->pBt, pCsr->pgnoRoot, pCsr);
  */
  ** saveAllCursors can only return SQLITE_OK.
  ** and hence saveAllCursors() cannot fail on a BTREE_INTKEY table, hence
  ** Note that pCsr must be open on a INTKEY table and saveCursorPosition()
  **
  ** version of the b-tree page modified by the accessPayload call below.
  ** required in case any of them are holding references to an xFetch
  /* Save the positions of all other cursors open on this table. This is

  }
    return SQLITE_ABORT;
  if( pCsr->eState!=CURSOR_VALID ){
  assert( pCsr->eState!=CURSOR_REQUIRESEEK );
  }
    return rc;
  if( rc!=SQLITE_OK ){
  rc = restoreCursorPosition(pCsr);

  assert( pCsr->curFlags & BTCF_Incrblob );
  assert( sqlite3_mutex_held(pCsr->pBtree->db->mutex) );
  assert( cursorOwnsBtShared(pCsr) );
  int rc;
int sqlite3BtreePutData(BtCursor *pCsr, u32 offset, u32 amt, void *z){
*/
** no modifications are made and SQLITE_CORRUPT is returned.
** parameters that attempt to write past the end of the existing data,
** change the length of the data stored. If this function is called with
** Only the data content may only be modified, it is not possible to 
**
** This function modifies the data stored as part of that entry.
** INTKEY table currently pointing at a valid table entry. 
** Argument pCsr must be a cursor opened for writing on an 
/*
#ifndef SQLITE_OMIT_INCRBLOB

#endif
}
  return rc;
  }
    sqlite3BtreeLeave(p);
    }
      rc = setSharedCacheTableLock(p, iTab, lockType);
    if( rc==SQLITE_OK ){
    rc = querySharedCacheTableLock(p, iTab, lockType);
    sqlite3BtreeEnter(p);

    assert( isWriteLock==0 || isWriteLock==1 );
    assert( READ_LOCK+1==WRITE_LOCK );
    u8 lockType = READ_LOCK + isWriteLock;
  if( p->sharable ){
  assert( p->inTrans!=TRANS_NONE );
  int rc = SQLITE_OK;
int sqlite3BtreeLockTable(Btree *p, int iTab, u8 isWriteLock){
*/
** if it is false.
** lock is a write lock if isWritelock is true or a read lock
** Obtain a lock on the table whose root page is iTab.  The
/*
#ifndef SQLITE_OMIT_SHARED_CACHE


}
  return rc;
  sqlite3BtreeLeave(p);
  assert( rc==SQLITE_OK || rc==SQLITE_LOCKED_SHAREDCACHE );
  rc = querySharedCacheTableLock(p, MASTER_ROOT, READ_LOCK);
  sqlite3BtreeEnter(p);
  assert( sqlite3_mutex_held(p->db->mutex) );
  int rc;
int sqlite3BtreeSchemaLocked(Btree *p){
*/
** sqlite_master table. Otherwise SQLITE_OK.
** btree as the argument handle holds an exclusive lock on the 
** Return SQLITE_LOCKED_SHAREDCACHE if another user of the same shared 
/*

}
  return pBt->pSchema;
  sqlite3BtreeLeave(p);
  }
    pBt->xFreeSchema = xFree;
    pBt->pSchema = sqlite3DbMallocZero(0, nBytes);
  if( !pBt->pSchema && nBytes ){
  sqlite3BtreeEnter(p);
  BtShared *pBt = p->pBt;
void *sqlite3BtreeSchema(Btree *p, int nBytes, void(*xFree)(void *)){
*/
** on the memory, the btree layer does that.
** blob of allocated memory. The xFree function should not call sqlite3_free()
** xFree argument when the memory allocation was made is invoked on the 
** Just before the shared-btree is closed, the function passed as the 
**
** allocated, it is returned as normal.
** allocated, a null pointer is returned. If the blob has already been
** If the nBytes parameter is 0 and the blob of memory has not yet been
**
** of memory returned. 
** call the nBytes parameter is ignored and a pointer to the same blob
** are allocated, zeroed, and returned to the caller. For each subsequent 
** The first time this is called on a shared-btree, nBytes bytes of memory
**
** the shared-btree). The btree layer manages reference counting issues.
** purposes (for example, to store a high-level schema associated with 
** a single shared-btree. The memory is used by client code for its own
** This function returns a pointer to a blob of memory associated with
/*

}
  return p->nBackup!=0;
  assert( sqlite3_mutex_held(p->db->mutex) );
  assert( p );
int sqlite3BtreeIsInBackup(Btree *p){

}
  return p->inTrans!=TRANS_NONE;
  assert( sqlite3_mutex_held(p->db->mutex) );
  assert( p );
int sqlite3BtreeIsInReadTrans(Btree *p){
*/
** Return non-zero if a read (or write) transaction is active.
/*

#endif
}
  return rc;
  }
    sqlite3BtreeLeave(p);
    }
      rc = sqlite3PagerCheckpoint(pBt->pPager, p->db, eMode, pnLog, pnCkpt);
    }else{
      rc = SQLITE_LOCKED;
    if( pBt->inTransaction!=TRANS_NONE ){
    sqlite3BtreeEnter(p);
    BtShared *pBt = p->pBt;
  if( p ){
  int rc = SQLITE_OK;
int sqlite3BtreeCheckpoint(Btree *p, int eMode, int *pnLog, int *pnCkpt){
*/
** Parameter eMode is one of SQLITE_CHECKPOINT_PASSIVE, FULL or RESTART.
**
** transaction on the shared-cache the argument Btree is connected to.
** Return SQLITE_LOCKED if this or any other connection has an open 
**
** Run a checkpoint on the Btree passed as the first argument.
/*
#ifndef SQLITE_OMIT_WAL

}
  return (p && (p->inTrans==TRANS_WRITE));
  assert( p==0 || sqlite3_mutex_held(p->db->mutex) );
int sqlite3BtreeIsInTrans(Btree *p){
*/
** Return non-zero if a transaction is active.
/*

}
  return sqlite3PagerJournalname(p->pBt->pPager);
  assert( p->pBt->pPager!=0 );
const char *sqlite3BtreeGetJournalname(Btree *p){
*/
** open so it is safe to access without the BtShared mutex.
** The pager journal filename is invariant as long as the pager is
**
** has been created or not.
** value of this routine is the same regardless of whether the journal file
** Return the pathname of the journal file for this database. The return
/*

}
  return sqlite3PagerFilename(p->pBt->pPager, 1);
  assert( p->pBt->pPager!=0 );
const char *sqlite3BtreeGetFilename(Btree *p){
*/
** open so it is safe to access without the BtShared mutex.
** The pager filename is invariant as long as the pager is
**
** an empty string if the database is in-memory or a TEMP database.
** Return the full pathname of the underlying database file.  Return
/*

#endif /* SQLITE_OMIT_INTEGRITY_CHECK */
}
  return sqlite3StrAccumFinish(&sCheck.errMsg);
  sqlite3BtreeLeave(p);
  assert( nRef==sqlite3PagerRefcount(pBt->pPager) );
  /* Make sure this analysis did not leave any unref() pages. */
  if( sCheck.nErr==0 ) sqlite3_str_reset(&sCheck.errMsg);
  *pnErr = sCheck.nErr;
  }
    sCheck.nErr++;
    sqlite3_str_reset(&sCheck.errMsg);
  if( sCheck.mallocFailed ){
  sqlite3_free(sCheck.aPgRef);
  sqlite3PageFree(sCheck.heap);
integrity_ck_cleanup:
  */
  /* Clean  up and report errors.

  }
#endif
    }
      checkAppendMsg(&sCheck, "Pointer map page %d is referenced", i);
       (PTRMAP_PAGENO(pBt, i)==i && pBt->autoVacuum) ){
    if( getPageReferenced(&sCheck, i)!=0 && 
    }
      checkAppendMsg(&sCheck, "Page %d is never used", i);
       (PTRMAP_PAGENO(pBt, i)!=i || !pBt->autoVacuum) ){
    if( getPageReferenced(&sCheck, i)==0 && 
    */
    ** references to pointer-map pages.
    /* If the database supports auto-vacuum, make sure no tables contain
#else
    }
      checkAppendMsg(&sCheck, "Page %d is never used", i);
    if( getPageReferenced(&sCheck, i)==0 ){
#ifdef SQLITE_OMIT_AUTOVACUUM
  for(i=1; i<=sCheck.nPage && sCheck.mxErr; i++){
  */
  /* Make sure every page in the file is referenced

  pBt->db->flags = savedDbFlags;
  }
    checkTreePage(&sCheck, aRoot[i], &notUsed, LARGEST_INT64);
#endif
    }
      checkPtrmap(&sCheck, aRoot[i], PTRMAP_ROOTPAGE, 0);
    if( pBt->autoVacuum && aRoot[i]>1 ){
#ifndef SQLITE_OMIT_AUTOVACUUM
    if( aRoot[i]==0 ) continue;
    i64 notUsed;
  for(i=0; (int)i<nRoot && sCheck.mxErr; i++){
  pBt->db->flags &= ~(u64)SQLITE_CellSizeCk;
  testcase( pBt->db->flags & SQLITE_CellSizeCk );
#endif
  }
    );
      "incremental_vacuum enabled with a max rootpage of zero"
    checkAppendMsg(&sCheck,
  }else if( get4byte(&pBt->pPage1->aData[64])!=0 ){
    }
      );
        mx, mxInHdr
        "max rootpage (%d) disagrees with header (%d)",
      checkAppendMsg(&sCheck,
    if( mx!=mxInHdr ){
    mxInHdr = get4byte(&pBt->pPage1->aData[52]);
    for(i=0; (int)i<nRoot; i++) if( mx<aRoot[i] ) mx = aRoot[i];
    int mxInHdr;
    int mx = 0;
  if( pBt->autoVacuum ){
#ifndef SQLITE_OMIT_AUTOVACUUM
  */
  /* Check all the tables.

  sCheck.zPfx = 0;
            get4byte(&pBt->pPage1->aData[36]));
  checkList(&sCheck, 1, get4byte(&pBt->pPage1->aData[32]),
  sCheck.zPfx = "Main freelist: ";
  */
  /* Check the integrity of the freelist

  if( i<=sCheck.nPage ) setPageReferenced(&sCheck, i);
  i = PENDING_BYTE_PAGE(pBt);

  }
    goto integrity_ck_cleanup;
    sCheck.mallocFailed = 1;
  if( sCheck.heap==0 ){
  sCheck.heap = (u32*)sqlite3PageMalloc( pBt->pageSize );
  }
    goto integrity_ck_cleanup;
    sCheck.mallocFailed = 1;
  if( !sCheck.aPgRef ){
  sCheck.aPgRef = sqlite3MallocZero((sCheck.nPage / 8)+ 1);

  }
    goto integrity_ck_cleanup;
  if( sCheck.nPage==0 ){
  sCheck.errMsg.printfFlags = SQLITE_PRINTF_INTERNAL;
  sqlite3StrAccumInit(&sCheck.errMsg, 0, zErr, sizeof(zErr), SQLITE_MAX_LENGTH);
  sCheck.heap = 0;
  sCheck.aPgRef = 0;
  sCheck.v2 = 0;
  sCheck.v1 = 0;
  sCheck.zPfx = 0;
  sCheck.mallocFailed = 0;
  sCheck.nErr = 0;
  sCheck.mxErr = mxErr;
  sCheck.nPage = btreePagecount(sCheck.pBt);
  sCheck.pPager = pBt->pPager;
  sCheck.pBt = pBt;
  sCheck.db = db;
  assert( nRef>=0 );
  VVA_ONLY( nRef = sqlite3PagerRefcount(pBt->pPager) );
  assert( p->inTrans>TRANS_NONE && pBt->inTransaction>TRANS_NONE );
  sqlite3BtreeEnter(p);

  VVA_ONLY( int nRef );
  char zErr[100];
  u64 savedDbFlags = pBt->db->flags;
  BtShared *pBt = p->pBt;
  IntegrityCk sCheck;
  Pgno i;
){
  int *pnErr    /* Write number of errors seen to this variable */
  int mxErr,    /* Stop reporting errors after this many */
  int nRoot,    /* Number of entries in aRoot[] */
  int *aRoot,   /* An array of root pages numbers for individual trees */
  Btree *p,     /* The btree to be checked */
  sqlite3 *db,  /* Database connection that is running the check */
char *sqlite3BtreeIntegrityCheck(
*/
** returned.  If a memory allocation error occurs, NULL is returned.
** malloc is returned if *pnErr is non-zero.  If *pnErr==0 then NULL is
** allocation errors,  an error message held in memory obtained from
** Write the number of error seen in *pnErr.  Except for some memory
**
** this function.
** A read-only or read-write transaction must be opened before calling
**
** a table.  nRoot is the number of entries in aRoot.
** an array of pages numbers were each page number is the root page of
** This routine does a complete check of the given BTree file.  aRoot[] is
/*
#ifndef SQLITE_OMIT_INTEGRITY_CHECK

#endif /* SQLITE_OMIT_INTEGRITY_CHECK */
}
  return depth+1;
  pCheck->v2 = saved_v2;
  pCheck->v1 = saved_v1;
  pCheck->zPfx = saved_zPfx;
  releasePage(pPage);
  if( !doCoverageCheck ) pPage->isInit = savedIsInit;
end_of_check:

  }
    }
          nFrag, data[hdr+7], iPage);
          "Fragmentation of %d bytes reported as %d on page %d",
      checkAppendMsg(pCheck,
    if( heap[0]==0 && nFrag!=data[hdr+7] ){
    */
    ** number of fragmented free bytes within the cell content area.
    ** EVIDENCE-OF: R-07161-27322 The one-byte integer at offset 7 gives the
    ** is stored in the fifth field of the b-tree page header.
    /* EVIDENCE-OF: R-43263-13491 The total number of bytes in all fragments
    nFrag += usableSize - (prev&0xffff) - 1;
    }
      }
        prev = x;
        nFrag += (x>>16) - (prev&0xffff) - 1;
      }else{
        break;
          "Multiple uses for byte %u of page %d", x>>16, iPage);
        checkAppendMsg(pCheck,
      if( (prev&0xffff)>=(x>>16) ){
    while( btreeHeapPull(heap,&x) ){
    prev = contentOffset - 1;   /* Implied first min-heap entry */
    nFrag = 0;
    */
    ** that gap is added to the fragmentation count.
    ** overlap, that means bytes are used multiple times.  If there is a gap,
    ** the start_address against the previous end_address.  If there is an
    ** The loop below pulls entries from the min-heap in order and compares
    **
    ** of cell content.  
    ** pointer index, and the gap between the cell pointer index and the start
    ** There is an implied first entry the covers the page header, the cell
    ** Each min-heap entry is of the form:    (start_address<<16)|end_address.
    ** 
    ** freeblocks, and counting the number of untracked bytes in nFrag.
    /* Analyze the min-heap looking for overlap between cells and/or 
    }
      i = j;
      assert( (u32)j<=usableSize-4 ); /* Enforced by btreeComputeFreeSpace() */
      assert( j==0 || j>i+size );     /* Enforced by btreeComputeFreeSpace() */
      ** increasing offset. */
      /* EVIDENCE-OF: R-06866-39125 Freeblocks are always connected in order of
      j = get2byte(&data[i]);
      ** chain. */
      ** freeblock in the chain, or zero if the freeblock is the last on the
      ** big-endian integer which is the offset in the b-tree page of the next
      /* EVIDENCE-OF: R-58208-19414 The first 2 bytes of a freeblock are a
      btreeHeapInsert(heap, (((u32)i)<<16)|(i+size-1));
      assert( (u32)(i+size)<=usableSize ); /* due to btreeComputeFreeSpace() */
      size = get2byte(&data[i+2]);
      assert( (u32)i<=usableSize-4 ); /* Enforced by btreeComputeFreeSpace() */
      int size, j;
    while( i>0 ){
    i = get2byte(&data[hdr+1]);
    */
    ** freeblocks on the page. 
    ** is the offset of the first freeblock, or zero if there are no
    ** EVIDENCE-OF: R-20690-50594 The second field of the b-tree page header
    **
    /* Add the freeblocks to the min-heap
    }
      }
        btreeHeapInsert(heap, (pc<<16)|(pc+size-1));
        size = pPage->xCellSize(pPage, &data[pc]);
        pc = get2byteAligned(&data[cellStart+i*2]);
        u32 size;
      for(i=nCell-1; i>=0; i--){
      heap[0] = 0;
      heap = pCheck->heap;
    if( !pPage->leaf ){
    ** not yet been done, so do it now */
    ** cells have already been inserted.  But for internal pages, that has
    /* For leaf pages, the min-heap has already been initialized and the
  if( doCoverageCheck && pCheck->mxErr>0 ){
  pCheck->zPfx = 0;
  */
  /* Check for complete coverage of the page

  *piMinKey = maxKey;
  }
    }
      btreeHeapInsert(heap, (pc<<16)|(pc+info.nSize-1));
      /* Populate the coverage-checking heap for leaf pages */
    }else{
      }
        depth = d2;
        checkAppendMsg(pCheck, "Child page depth differs");
      if( d2!=depth ){
      keyCanBeEqual = 0;
      d2 = checkTreePage(pCheck, pgno, &maxKey, maxKey);
#endif
      }
        checkPtrmap(pCheck, pgno, PTRMAP_BTREE, iPage);
      if( pBt->autoVacuum ){
#ifndef SQLITE_OMIT_AUTOVACUUM
      pgno = get4byte(pCell);
      /* Check sanity of left child page for internal pages */
    if( !pPage->leaf ){

    }
      checkList(pCheck, 0, pgnoOvfl, nPage);
#endif
      }
        checkPtrmap(pCheck, pgnoOvfl, PTRMAP_OVERFLOW1, iPage);
      if( pBt->autoVacuum ){
#ifndef SQLITE_OMIT_AUTOVACUUM
      pgnoOvfl = get4byte(&pCell[info.nSize - 4]);
      nPage = (info.nPayload - info.nLocal + usableSize - 5)/(usableSize - 4);
      assert( pc + info.nSize - 4 <= usableSize );
      Pgno pgnoOvfl;   /* First page of the overflow chain */
      u32 nPage;       /* Number of pages on the overflow chain */
    if( info.nPayload>info.nLocal ){
    /* Check the content overflow list */

    }
      keyCanBeEqual = 0;     /* Only the first key on the page may ==maxKey */
      maxKey = info.nKey;
      }
        checkAppendMsg(pCheck, "Rowid %lld out of order", info.nKey);
      if( keyCanBeEqual ? (info.nKey > maxKey) : (info.nKey >= maxKey) ){
    if( pPage->intKey ){
    /* Check for integer primary key out of range */

    }
      continue;
      doCoverageCheck = 0;
      checkAppendMsg(pCheck, "Extends off end of page");
    if( pc+info.nSize>usableSize ){
    pPage->xParseCell(pPage, pCell, &info);
    pCell = &data[pc];
    }
      continue;
      doCoverageCheck = 0;
                             pc, contentOffset, usableSize-4);
      checkAppendMsg(pCheck, "Offset %d out of range %d..%d",
    if( pc<contentOffset || pc>usableSize-4 ){
    pCellIdx -= 2;
    pc = get2byteAligned(pCellIdx);
    assert( pCellIdx==&data[cellStart + i*2] );
    pCheck->v2 = i;
    /* Check cell size */

    CellInfo info;
  for(i=nCell-1; i>=0 && pCheck->mxErr; i--){
  ** integer offsets to the cell contents. */
  /* EVIDENCE-OF: R-02776-14802 The cell pointer array consists of K 2-byte

  }
    heap[0] = 0;
    heap = pCheck->heap;
    ** as the other cell checks, so initialize the heap.  */
    /* For leaf pages, the coverage check will occur in the same loop
  }else{
    keyCanBeEqual = 0;
    depth = checkTreePage(pCheck, pgno, &maxKey, maxKey);
#endif
    }
      checkPtrmap(pCheck, pgno, PTRMAP_BTREE, iPage);
      pCheck->zPfx = "On page %d at right child: ";
    if( pBt->autoVacuum ){
#ifndef SQLITE_OMIT_AUTOVACUUM
    pgno = get4byte(&data[hdr+8]);
    /* Analyze the right-child page of internal pages */
  if( !pPage->leaf ){

  pCellIdx = &data[cellStart + 2*(nCell-1)];
  assert( pPage->aCellIdx==&data[cellStart] );
  cellStart = hdr + 12 - 4*pPage->leaf;
  ** immediately follows the b-tree page header. */
  /* EVIDENCE-OF: R-23882-45353 The cell pointer array of a b-tree page

  assert( pPage->nCell==nCell );
  nCell = get2byte(&data[hdr+3]);
  ** number of cells on the page. */
  /* EVIDENCE-OF: R-37002-32774 The two-byte integer at offset 3 gives the

  assert( contentOffset<=usableSize );  /* Enforced by btreeInitPage() */
  contentOffset = get2byteNotZero(&data[hdr+5]);
  pCheck->zPfx = "On tree page %d cell %d: ";
  /* Set up for cell analysis */

  hdr = pPage->hdrOffset;
  data = pPage->aData;
  }
    goto end_of_check;
    checkAppendMsg(pCheck, "free space corruption", rc);
    assert( rc==SQLITE_CORRUPT );
  if( (rc = btreeComputeFreeSpace(pPage))!=0 ){
  }
    goto end_of_check;
                   "btreeInitPage() returns error code %d", rc);
    checkAppendMsg(pCheck,
    assert( rc==SQLITE_CORRUPT );  /* The only possible error from InitPage */
  if( (rc = btreeInitPage(pPage))!=0 ){
  pPage->isInit = 0;
  savedIsInit = pPage->isInit;
  ** btreeInitPage() is executed.  */
  /* Clear MemPage.isInit to make sure the corruption detection code in

  }
    goto end_of_check;
       "unable to get the page. error code=%d", rc);
    checkAppendMsg(pCheck,
  if( (rc = btreeGetPage(pBt, (Pgno)iPage, &pPage, 0))!=0 ){
  pCheck->v1 = iPage;
  pCheck->zPfx = "Page %d: ";
  if( checkRef(pCheck, iPage) ) return 0;
  if( iPage==0 ) return 0;
  usableSize = pBt->usableSize;
  pBt = pCheck->pBt;
  */
  /* Check that the page exists

  u8 savedIsInit = 0;
  int saved_v2 = pCheck->v2;
  int saved_v1 = pCheck->v1;
  const char *saved_zPfx = pCheck->zPfx;
  u32 x, prev = 0;         /* Next and previous entry on the min-heap */
  u32 *heap = 0;           /* Min-heap used for checking cell coverage */
  u32 contentOffset;       /* Offset to the start of the cell content area */
  u32 usableSize;          /* Usable size of the page */
  u32 pc;                  /* Address of a cell */
  BtShared *pBt;           /* The BtShared object that owns pPage */
  u8 *pCellIdx;            /* Next element of the cell pointer array */
  u8 *pCell;               /* Cell content */
  u8 *data;                /* Page content */
                           ** False if IPK must be strictly less than maxKey */
  int keyCanBeEqual = 1;   /* True if IPK can be equal to maxKey
  int doCoverageCheck = 1; /* True if cell coverage checking should be done */
  int nCell;               /* Number of cells */
  int cellStart;           /* Offset to the start of the cell pointer array */
  int hdr;                 /* Offset to the page header */
  int nFrag;               /* Number of fragmented bytes on the page */
  int pgno;                /* Page number */
  int depth = -1, d2;      /* Depth of a subtree */
  int rc;                  /* Result code from subroutine call */
  int i;                   /* Loop counter */
  MemPage *pPage = 0;      /* The page being analyzed */
){
  i64 maxKey            /* Error if integer primary key greater than this */
  i64 *piMinKey,        /* Write minimum integer primary key here */
  int iPage,            /* Page number of the page to check */
  IntegrityCk *pCheck,  /* Context for the sanity check */
static int checkTreePage(
*/
**      5.  Verify that the depth of all children is the same.
**      4.  Recursively call checkTreePage on all children.
**      3.  Check the integrity of overflow pages.
**      2.  Make sure integer cell keys are in order.
**          but combine to completely cover the page.
**      1.  Make sure that cells and freeblocks do not overlap
**
** These checks are done:
** 
** return 1, and so forth.
** the tree depth.  Root pages return 0.  Parents of root pages
** Do various sanity checks on a single page of a tree.  Return
/*
#ifndef SQLITE_OMIT_INTEGRITY_CHECK

}
  return 1;  
  }
    i = j;
    aHeap[j] = x;
    aHeap[i] = aHeap[j];
    x = aHeap[i];
    if( aHeap[i]<aHeap[j] ) break;
    if( aHeap[j]>aHeap[j+1] ) j++;
  while( (j = i*2)<=aHeap[0] ){
  i = 1;
  aHeap[0]--;
  aHeap[x] = 0xffffffff;
  aHeap[1] = aHeap[x];
  *pOut = aHeap[1];
  if( (x = aHeap[0])==0 ) return 0;
  u32 j, i, x;
static int btreeHeapPull(u32 *aHeap, u32 *pOut){
}
  }
    i = j;
    aHeap[i] = x;
    aHeap[j] = aHeap[i];
    x = aHeap[j];
  while( (j = i/2)>0 && aHeap[j]>aHeap[i] ){
  aHeap[i] = x;
  u32 j, i = ++aHeap[0];
static void btreeHeapInsert(u32 *aHeap, u32 x){
*/
** lower 16 bits are the index of the last byte of that range.
** The upper 16 bits are the index of the first byte of a range and the
** entry represents the span of a cell or freeblock on a btree page.  
** This heap is used for cell overlap and coverage testing.  Each u32
**
** property.
** and then moves other nodes around as necessary to preserve the heap
** removes the root element from the heap (the minimum value in the heap)
** the heap, preserving the heap property.  The btreeHeapPull() routine
** The btreeHeapInsert() routine inserts an unsigned 32-bit number onto
**
** root node aHeap[1] is always the minimum value currently in the heap.
** of its daughter nodes.  A consequence of the heap property is that the
** The heap property is this:  Every node is less than or equal to both
**
** and aHeap[N*2+1].
** root element.  The daughter nodes of aHeap[N] are aHeap[N*2]
** aHeap[0] is the number of elements on the heap.  aHeap[1] is the
**
** An implementation of a min-heap.
/*

#endif /* SQLITE_OMIT_INTEGRITY_CHECK */
}
  }
      expected-N, expected);
      isFreeList ? "size" : "overflow list length",
      "%s is %d but should be %d",
    checkAppendMsg(pCheck,
  if( N && nErrAtStart==pCheck->nErr ){
  }
    sqlite3PagerUnref(pOvflPage);
    iPage = get4byte(pOvflData);
#endif
    }
      }
        checkPtrmap(pCheck, i, PTRMAP_OVERFLOW2, iPage);
        i = get4byte(pOvflData);
      if( pCheck->pBt->autoVacuum && N>0 ){
      */
      ** the following page matches iPage.
      ** page in this overflow list, check that the pointer-map entry for
      /* If this database supports auto-vacuum and iPage is not the last
    else{
#ifndef SQLITE_OMIT_AUTOVACUUM
    }
      }
        N -= n;
        }
          checkRef(pCheck, iFreePage);
#endif
          }
            checkPtrmap(pCheck, iFreePage, PTRMAP_FREEPAGE, 0);
          if( pCheck->pBt->autoVacuum ){
#ifndef SQLITE_OMIT_AUTOVACUUM
          Pgno iFreePage = get4byte(&pOvflData[8+i*4]);
        for(i=0; i<(int)n; i++){
      }else{
        N--;
           "freelist leaf count too big on page %d", iPage);
        checkAppendMsg(pCheck,
      if( n>pCheck->pBt->usableSize/4-2 ){
#endif
      }
        checkPtrmap(pCheck, iPage, PTRMAP_FREEPAGE, 0);
      if( pCheck->pBt->autoVacuum ){
#ifndef SQLITE_OMIT_AUTOVACUUM
      u32 n = (u32)get4byte(&pOvflData[4]);
    if( isFreeList ){
    pOvflData = (unsigned char *)sqlite3PagerGetData(pOvflPage);
    }
      break;
      checkAppendMsg(pCheck, "failed to get page %d", iPage);
    if( sqlite3PagerGet(pCheck->pPager, (Pgno)iPage, &pOvflPage, 0) ){
    N--;
    if( checkRef(pCheck, iPage) ) break;
    unsigned char *pOvflData;
    DbPage *pOvflPage;
  while( iPage!=0 && pCheck->mxErr ){
  int nErrAtStart = pCheck->nErr;
  u32 expected = N;
  int i;
){
  u32 N                 /* Expected number of pages in the list */
  int iPage,            /* Page number for first page in the list */
  int isFreeList,       /* True for a freelist.  False for overflow page list */
  IntegrityCk *pCheck,  /* Integrity checking context */
static void checkList(
*/
** Verify that the number of pages on the list is N.
** Check the integrity of the freelist or of an overflow page list.
/*

#endif
}
  }
      iChild, eType, iParent, ePtrmapType, iPtrmapParent);
      "Bad ptr map entry key=%d expected=(%d,%d) got=(%d,%d)", 
    checkAppendMsg(pCheck,
  if( ePtrmapType!=eType || iPtrmapParent!=iParent ){

  }
    return;
    checkAppendMsg(pCheck, "Failed to read ptrmap key=%d", iChild);
    if( rc==SQLITE_NOMEM || rc==SQLITE_IOERR_NOMEM ) pCheck->mallocFailed = 1;
  if( rc!=SQLITE_OK ){
  rc = ptrmapGet(pCheck->pBt, iChild, &ePtrmapType, &iPtrmapParent);

  Pgno iPtrmapParent;
  u8 ePtrmapType;
  int rc;
){
  Pgno iParent           /* Expected pointer map parent page number */
  u8 eType,              /* Expected pointer map type */
  Pgno iChild,           /* Child page number */
  IntegrityCk *pCheck,   /* Integrity check context */
static void checkPtrmap(
*/
** to pCheck.
** page iParent, pointer type ptrType. If not, append an error message
** Check that the entry in the pointer-map for page iChild maps to 
/*
#ifndef SQLITE_OMIT_AUTOVACUUM

}
  return 0;
  setPageReferenced(pCheck, iPage);
  if( AtomicLoad(&pCheck->db->u1.isInterrupted) ) return 1;
  }
    return 1;
    checkAppendMsg(pCheck, "2nd reference to page %d", iPage);
  if( getPageReferenced(pCheck, iPage) ){
  }
    return 1;
    checkAppendMsg(pCheck, "invalid page number %d", iPage);
  if( iPage>pCheck->nPage || iPage==0 ){
static int checkRef(IntegrityCk *pCheck, Pgno iPage){
*/
** Also check that the page number is in bounds.
**
** if this is the first reference to the page.
** Return 1 if there are 2 or more references to the page and 0 if
** reference to the page, add an error message to pCheck->zErrMsg.
** Add 1 to the reference count for page iPage.  If this is the second
/*


}
  pCheck->aPgRef[iPg/8] |= (1 << (iPg & 0x07));
  assert( iPg<=pCheck->nPage && sizeof(pCheck->aPgRef[0])==1 );
static void setPageReferenced(IntegrityCk *pCheck, Pgno iPg){
*/
** Set the bit in the IntegrityCk.aPgRef[] array that corresponds to page iPg.
/*

}
  return (pCheck->aPgRef[iPg/8] & (1 << (iPg & 0x07)));
  assert( iPg<=pCheck->nPage && sizeof(pCheck->aPgRef[0])==1 );
static int getPageReferenced(IntegrityCk *pCheck, Pgno iPg){
*/
** corresponds to page iPg is already set.
** Return non-zero if the bit in the IntegrityCk.aPgRef[] array that
/*

#ifndef SQLITE_OMIT_INTEGRITY_CHECK

#endif /* SQLITE_OMIT_INTEGRITY_CHECK */
}
  }
    pCheck->mallocFailed = 1;
  if( pCheck->errMsg.accError==SQLITE_NOMEM ){
  va_end(ap);
  sqlite3_str_vappendf(&pCheck->errMsg, zFormat, ap);
  }
    sqlite3_str_appendf(&pCheck->errMsg, pCheck->zPfx, pCheck->v1, pCheck->v2);
  if( pCheck->zPfx ){
  }
    sqlite3_str_append(&pCheck->errMsg, "\n", 1);
  if( pCheck->errMsg.nChar ){
  va_start(ap, zFormat);
  pCheck->nErr++;
  pCheck->mxErr--;
  if( !pCheck->mxErr ) return;
  va_list ap;
){
  ...
  const char *zFormat,
  IntegrityCk *pCheck,
static void checkAppendMsg(
*/
** Append a message to the error message string.
/*
#ifndef SQLITE_OMIT_INTEGRITY_CHECK

}
  return p->pBt->pPager;
Pager *sqlite3BtreePager(Btree *p){
*/
** testing and debugging only.
** Return the pager associated with a BTree.  This routine is used for
/*

#endif
}
  return rc;
  /* An error has occurred. Return an error code. */

  }
    }
      rc = moveToChild(pCur, get4byte(findCell(pPage, iIdx)));
    }else{
      rc = moveToChild(pCur, get4byte(&pPage->aData[pPage->hdrOffset+8]));
    if( iIdx==pPage->nCell ){
    iIdx = pCur->ix;
    */
    ** points at. This is the right-child if (iIdx==pPage->nCell).
    /* Descend to the child node of the cell that the cursor currently 

    }
      pPage = pCur->pPage;
      pCur->ix++;

      }while ( pCur->ix>=pCur->pPage->nCell );
        moveToParent(pCur);
        }
          return moveToRoot(pCur);
          *pnEntry = nEntry;
          /* All pages of the b-tree have been visited. Return successfully. */
        if( pCur->iPage==0 ){
      do {
    if( pPage->leaf ){
    */
    ** caller.
    ** If all pages in the tree have been visited, return SQLITE_OK to the
    **
    ** to visit is the right-child of its parent.
    ** of the page, or to the number of cells in the page if the next page
    ** pCur->aiIdx[pCur->iPage] value is set to the index of the parent cell
    ** the next page in the tree that has not yet been visited. The
    ** points to the first interior cell that it points to the parent of
    /* pPage is a leaf node. This loop navigates the cursor so that it 

    }
      nEntry += pPage->nCell;
    if( pPage->leaf || !pPage->intKey ){
    pPage = pCur->pPage;
    */
    ** accordingly.
    ** this page contains countable entries. Increment the entry counter
    /* If this is a leaf page or the tree is not an int-key tree, then 

    MemPage *pPage;                    /* Current page of the b-tree */
    int iIdx;                          /* Index of child node in parent */
  while( rc==SQLITE_OK && !AtomicLoad(&db->u1.isInterrupted) ){
  */
  ** page in the B-Tree structure (not including overflow pages). 
  /* Unless an error occurs, the following loop runs one iteration for each

  }
    return SQLITE_OK;
    *pnEntry = 0;
  if( rc==SQLITE_EMPTY ){
  rc = moveToRoot(pCur);

  int rc;                              /* Return code */
  i64 nEntry = 0;                      /* Value to return in *pnEntry */
int sqlite3BtreeCount(sqlite3 *db, BtCursor *pCur, i64 *pnEntry){
*/
** corruption) an SQLite error code is returned.
** Otherwise, if an error is encountered (i.e. an IO error or database
** SQLITE_OK is returned if the operation is successfully executed. 
**
** number of entries in the b-tree and write the result to *pnEntry.
** The first argument, pCur, is a cursor opened on some b-tree. Count the
/*
#ifndef SQLITE_OMIT_BTREECOUNT

}
  return rc;
  sqlite3BtreeLeave(p);
  }
#endif
    }
      pBt->incrVacuum = (u8)iMeta;
      assert( iMeta==0 || iMeta==1 );
      assert( pBt->autoVacuum || iMeta==0 );
    if( idx==BTREE_INCR_VACUUM ){
#ifndef SQLITE_OMIT_AUTOVACUUM
    put4byte(&pP1[36 + idx*4], iMeta);
  if( rc==SQLITE_OK ){
  rc = sqlite3PagerWrite(pBt->pPage1->pDbPage);
  pP1 = pBt->pPage1->aData;
  assert( pBt->pPage1!=0 );
  assert( p->inTrans==TRANS_WRITE );
  sqlite3BtreeEnter(p);
  assert( idx>=1 && idx<=15 );
  int rc;
  unsigned char *pP1;
  BtShared *pBt = p->pBt;
int sqlite3BtreeUpdateMeta(Btree *p, int idx, u32 iMeta){
*/
** read-only and may not be written.
** Write meta-information back into the database.  Meta[0] is
/*

}
  sqlite3BtreeLeave(p);

#endif
  }
    pBt->btsFlags |= BTS_READ_ONLY;
  if( idx==BTREE_LARGEST_ROOT_PAGE && *pMeta>0 ){
#ifdef SQLITE_OMIT_AUTOVACUUM
  ** database, mark the database as read-only.  */
  /* If auto-vacuum is disabled in this build and this is an auto-vacuum

  }
    *pMeta = get4byte(&pBt->pPage1->aData[36 + idx*4]);
  }else{
    *pMeta = sqlite3PagerDataVersion(pBt->pPager) + p->iDataVersion;
  if( idx==BTREE_DATA_VERSION ){

  assert( idx>=0 && idx<=15 );
  assert( pBt->pPage1 );
  assert( SQLITE_OK==querySharedCacheTableLock(p, MASTER_ROOT, READ_LOCK) );
  assert( p->inTrans>TRANS_NONE );
  sqlite3BtreeEnter(p);

  BtShared *pBt = p->pBt;
void sqlite3BtreeGetMeta(Btree *p, int idx, u32 *pMeta){
*/
** read it from this routine.
** pattern is the same as header meta values, and so it is convenient to
** database file.  It is a number computed by the pager.  But its access
** from the pager.  The BTREE_DATA_VERSION value is not actually stored in the
** of reading the value out of the header, it instead loads the "DataVersion"
** This routine treats Meta[BTREE_DATA_VERSION] as a special case.  Instead
**
** free pages is not visible.  So Cookie[0] is the same as Meta[1].
** layer (and the SetCookie and ReadCookie opcodes) the number of
** The schema layer numbers meta values differently.  At the schema
** 
** is read-only, the others are read/write.
** through meta[15] are available for use by higher layers.  Meta[0]
** is the number of free pages currently in the database.  Meta[1]
** Read the meta-information out of a database file.  Meta[0]
**
** has a read or write transaction open on the database.
** This function may only be called if the b-tree connection already
/*


}
  return rc;
  sqlite3BtreeLeave(p);
  rc = btreeDropTable(p, iTable, piMoved);
  sqlite3BtreeEnter(p);
  int rc;
int sqlite3BtreeDropTable(Btree *p, int iTable, int *piMoved){
}
  return rc;  
#endif
  }
    releasePage(pPage);
    freePage(pPage, &rc);
  }else{
    rc = sqlite3BtreeUpdateMeta(p, 4, maxRootPgno);

    assert( maxRootPgno!=PENDING_BYTE_PAGE(pBt) );
    }
      maxRootPgno--;
           || PTRMAP_ISPAGE(pBt, maxRootPgno) ){
    while( maxRootPgno==PENDING_BYTE_PAGE(pBt)
    maxRootPgno--;
    */
    ** PENDING_BYTE_PAGE.
    ** be a root-page number, less one again if that is the
    ** is the old value less one, less one more if that happens to
    /* Set the new 'max-root-page' value in the database header. This

    }
      *piMoved = maxRootPgno;
      }
        return rc;
      if( rc!=SQLITE_OK ){
      releasePage(pMove);
      freePage(pMove, &rc);
      rc = btreeGetPage(pBt, maxRootPgno, &pMove, 0);
      pMove = 0;
      }
        return rc;
      if( rc!=SQLITE_OK ){
      releasePage(pMove);
      rc = relocatePage(pBt, pMove, PTRMAP_ROOTPAGE, 0, iTable, 0);
      }
        return rc;
      if( rc!=SQLITE_OK ){
      rc = btreeGetPage(pBt, maxRootPgno, &pMove, 0);
      releasePage(pPage);
      MemPage *pMove;
      */
      ** gap left by the deleted root-page.
      ** number in the database. So move the page that does into the 
      /* The table being dropped does not have the largest root-page
    }else{
      }
        return rc;
      if( rc!=SQLITE_OK ){
      releasePage(pPage);
      freePage(pPage, &rc);
      */
      ** number in the database, put the root page on the free list. 
      /* If the table being dropped is the table with the largest root-page
    if( iTable==maxRootPgno ){

    sqlite3BtreeGetMeta(p, BTREE_LARGEST_ROOT_PAGE, &maxRootPgno);
    Pgno maxRootPgno;
  if( pBt->autoVacuum ){
#else
  releasePage(pPage);
  freePage(pPage, &rc);
#ifdef SQLITE_OMIT_AUTOVACUUM

  *piMoved = 0;

  }
    return rc;
    releasePage(pPage);
  if( rc ){
  rc = sqlite3BtreeClearTable(p, iTable, 0);
  if( rc ) return rc;
  rc = btreeGetPage(pBt, (Pgno)iTable, &pPage, 0);

  }
    return SQLITE_CORRUPT_BKPT;
  if( iTable>btreePagecount(pBt) ){
  assert( iTable>=2 );
  assert( p->inTrans==TRANS_WRITE );
  assert( sqlite3BtreeHoldsMutex(p) );

  BtShared *pBt = p->pBt;
  MemPage *pPage = 0;
  int rc;
static int btreeDropTable(Btree *p, Pgno iTable, int *piMoved){
*/
** meta[3] is updated by this procedure.
** The last root page is recorded in meta[3] and the value of
** the move.  If no page gets moved, *piMoved is set to 0.
** page number that used to be the last root page in the file before
** is necessary for AUTOVACUUM to work right.  *piMoved is set to the 
** root pages are kept at the beginning of the database file, which
** is added to the freelist instead of iTable.  In this say, all
** iTable and that last slot formerly occupied by the last root page
** in the database file is moved into the slot formerly occupied by
** root page in the database file, then the last root page 
** If AUTOVACUUM is enabled and the page at iTable is not the last
**
** cursors on the table.
** This routine will fail with SQLITE_LOCKED if there are any open
**
** page 1) is never added to the freelist.
** the freelist.  Except, the root of the principle table (the one on
** Erase all information in a table and add the root of the table to
/*

}
  return sqlite3BtreeClearTable(pCur->pBtree, pCur->pgnoRoot, 0);
int sqlite3BtreeClearTableOfCursor(BtCursor *pCur){
*/
** This routine only work for pCur on an ephemeral table.
**
** Delete all information from the single table that pCur is open on.
/*

}
  return rc;
  sqlite3BtreeLeave(p);
  }
    rc = clearDatabasePage(pBt, (Pgno)iTable, 0, pnChange);
    invalidateIncrblobCursors(p, (Pgno)iTable, 0, 1);
    ** a no-op).  */
    ** is the root of a table b-tree - if it is not, the following call is
    /* Invalidate all incrblob cursors open on table iTable (assuming iTable
  if( SQLITE_OK==rc ){

  rc = saveAllCursors(pBt, (Pgno)iTable, 0);

  assert( p->inTrans==TRANS_WRITE );
  sqlite3BtreeEnter(p);
  BtShared *pBt = p->pBt;
  int rc;
int sqlite3BtreeClearTable(Btree *p, int iTable, int *pnChange){
*/
** entries in the table.
** integer value pointed to by pnChange is incremented by the number of
** If pnChange is not NULL, then table iTable must be an intkey table. The
**
** root of the table.
** read cursors on the table.  Open write cursors are moved to the
** This routine will fail with SQLITE_LOCKED if there are any open
**
** the root page is empty, but still exists.
** the page number of the root of the table.  After this routine returns,
** Delete all information from a single table in the database.  iTable is
/*

}
  return rc;
  releasePage(pPage);
  pPage->bBusy = 0;
cleardatabasepage_out:

  }
    zeroPage(pPage, pPage->aData[hdr] | PTF_LEAF);
  }else if( (rc = sqlite3PagerWrite(pPage->pDbPage))==0 ){
    freePage(pPage, &rc);
  if( freePageFlag ){
  }
    *pnChange += pPage->nCell;
    testcase( !pPage->intKey );
    assert( pPage->intKey || CORRUPT_DB );
  }else if( pnChange ){
    if( rc ) goto cleardatabasepage_out;
    rc = clearDatabasePage(pBt, get4byte(&pPage->aData[hdr+8]), 1, pnChange);
  if( !pPage->leaf ){
  }
    if( rc ) goto cleardatabasepage_out;
    rc = clearCell(pPage, pCell, &info);
    }
      if( rc ) goto cleardatabasepage_out;
      rc = clearDatabasePage(pBt, get4byte(pCell), 1, pnChange);
    if( !pPage->leaf ){
    pCell = findCell(pPage, i);
  for(i=0; i<pPage->nCell; i++){
  hdr = pPage->hdrOffset;
  pPage->bBusy = 1;
  }
    goto cleardatabasepage_out;
    rc = SQLITE_CORRUPT_BKPT;
  if( pPage->bBusy ){
  if( rc ) return rc;
  rc = getAndInitPage(pBt, pgno, &pPage, 0, 0);
  }
    return SQLITE_CORRUPT_BKPT;
  if( pgno>btreePagecount(pBt) ){
  assert( sqlite3_mutex_held(pBt->mutex) );

  CellInfo info;
  int hdr;
  int i;
  unsigned char *pCell;
  int rc;
  MemPage *pPage;
){
  int *pnChange            /* Add number of Cells freed to this counter */
  int freePageFlag,        /* Deallocate page if true */
  Pgno pgno,               /* Page number to clear */
  BtShared *pBt,           /* The BTree that contains the table */
static int clearDatabasePage(
*/
** the page to the freelist.
** Erase the given database page and all its children.  Return
/*

}
  return rc;
  sqlite3BtreeLeave(p);
  rc = btreeCreateTable(p, piTable, flags);
  sqlite3BtreeEnter(p);
  int rc;
int sqlite3BtreeCreateTable(Btree *p, int *piTable, int flags){
}
  return SQLITE_OK;
  *piTable = (int)pgnoRoot;
  assert( (pBt->openFlags & BTREE_SINGLE)==0 || pgnoRoot==2 );
  sqlite3PagerUnref(pRoot->pDbPage);
  zeroPage(pRoot, ptfFlags);
  }
    ptfFlags = PTF_ZERODATA | PTF_LEAF;
  }else{
    ptfFlags = PTF_INTKEY | PTF_LEAFDATA | PTF_LEAF;
  if( createTabFlags & BTREE_INTKEY ){
  assert( sqlite3PagerIswriteable(pRoot->pDbPage) );
#endif
  }
    if( rc ) return rc;
    rc = allocateBtreePage(pBt, &pRoot, &pgnoRoot, 1, 0);
  }else{

    }
      return rc;
      releasePage(pRoot);
    if( NEVER(rc) ){
    rc = sqlite3BtreeUpdateMeta(p, 4, pgnoRoot);
    assert( sqlite3PagerIswriteable(pBt->pPage1->pDbPage) );
    */
    ** freelist count.  Hence, the sqlite3BtreeUpdateMeta() call cannot fail.
    ** order either to increase the database filesize, or to decrement the
    /* When the new root page was allocated, page 1 was made writable in

    }
      return rc;
      releasePage(pRoot);
    if( rc ){
    ptrmapPut(pBt, pgnoRoot, PTRMAP_ROOTPAGE, 0, &rc);
    /* Update the pointer-map and meta-data with the new root-page number. */

    } 
      pRoot = pPageMove;
    }else{
      }
        return rc;
        releasePage(pRoot);
      if( rc!=SQLITE_OK ){
      rc = sqlite3PagerWrite(pRoot->pDbPage);
      }
        return rc;
      if( rc!=SQLITE_OK ){
      rc = btreeGetPage(pBt, pgnoRoot, &pRoot, 0);
      }
        return rc;
      if( rc!=SQLITE_OK ){
      /* Obtain the page at pgnoRoot */

      releasePage(pRoot);
      rc = relocatePage(pBt, pRoot, eType, iPtrPage, pgnoMove, 0);
      assert( eType!=PTRMAP_FREEPAGE );
      assert( eType!=PTRMAP_ROOTPAGE );
      }
        return rc;
        releasePage(pRoot);
      if( rc!=SQLITE_OK ){
      }
        rc = SQLITE_CORRUPT_BKPT;
      if( eType==PTRMAP_ROOTPAGE || eType==PTRMAP_FREEPAGE ){
      rc = ptrmapGet(pBt, pgnoRoot, &eType, &iPtrPage);
      }
        return rc;
      if( rc!=SQLITE_OK ){
      rc = btreeGetPage(pBt, pgnoRoot, &pRoot, 0);
      /* Move the page currently at pgnoRoot to pgnoMove. */

      }
        return rc;
      if( rc!=SQLITE_OK ){
      releasePage(pPageMove);
      rc = saveAllCursors(pBt, 0, 0);
      ** corresponding to page pgnoRoot.  */
      ** case they are holding a reference to an xFetch reference
      /* Save the positions of any open cursors. This is required in

      Pgno iPtrPage = 0;
      u8 eType = 0;
      */
      ** is already journaled.
      ** by extending the file), the current page at position pgnoMove
      ** allocated pgnoMove. If required (i.e. if it was not allocated
      ** the new table (assuming an error did not occur). But we were
      /* pgnoRoot is the page that will be used for the root-page of
    if( pgnoMove!=pgnoRoot ){

    }
      return rc;
    if( rc!=SQLITE_OK ){
    rc = allocateBtreePage(pBt, &pPageMove, &pgnoMove, pgnoRoot, BTALLOC_EXACT);
    */
    ** to reside at pgnoRoot).
    ** be moved to the allocated page (unless the allocated page happens
    /* Allocate a page. The page that currently resides at pgnoRoot will

    testcase( pgnoRoot<3 );
    assert( pgnoRoot>=3 || CORRUPT_DB );
    }
      pgnoRoot++;
        pgnoRoot==PENDING_BYTE_PAGE(pBt) ){
    while( pgnoRoot==PTRMAP_PAGENO(pBt, pgnoRoot) ||
    */
    ** PENDING_BYTE page.
    /* The new root-page may not be allocated on a pointer-map page, or the

    pgnoRoot++;
    sqlite3BtreeGetMeta(p, BTREE_LARGEST_ROOT_PAGE, &pgnoRoot);
    */
    ** created so far, so the new root-page is (meta[3]+1).
    ** root page of the new table should go. meta[3] is the largest root-page
    /* Read the value of meta[3] from the database to determine where the

    invalidateAllOverflowCache(pBt);
    */
    ** held by open cursors.
    ** out to be an overflow page, delete all overflow page-map caches
    ** to make room for the new tables root page. In case this page turns
    /* Creating a new table may probably require moving an existing database

    MemPage *pPageMove; /* The page to move to. */
    Pgno pgnoMove;      /* Move a page here to make room for the root-page */
  if( pBt->autoVacuum ){
#else
  }
    return rc;
  if( rc ){
  rc = allocateBtreePage(pBt, &pRoot, &pgnoRoot, 1, 0);
#ifdef SQLITE_OMIT_AUTOVACUUM

  assert( (pBt->btsFlags & BTS_READ_ONLY)==0 );
  assert( pBt->inTransaction==TRANS_WRITE );
  assert( sqlite3BtreeHoldsMutex(p) );

  int ptfFlags;          /* Page-type flage for the root page of new table */
  int rc;
  Pgno pgnoRoot;
  MemPage *pRoot;
  BtShared *pBt = p->pBt;
static int btreeCreateTable(Btree *p, int *piTable, int createTabFlags){
*/
**     BTREE_ZERODATA                  Used for SQL indices
**     BTREE_INTKEY|BTREE_LEAFDATA     Used for SQL tables with rowid keys
**
** flags might not work:
** following values of flags are currently in use.  Other values for
** The type of type is determined by the flags parameter.  Only the
**
** number for the root page of the new table.
** Create a new BTree table.  Write into *piTable the page
/*

}
  return rc;
  }
    }
      if( rc==SQLITE_EMPTY ) rc = SQLITE_OK;
      }
        pCur->eState = CURSOR_REQUIRESEEK;
        btreeReleaseAllCursorPages(pCur);
      if( bPreserve ){
      rc = moveToRoot(pCur);
    }else{
      }
        pCur->skipNext = 1;
      }else{
        pCur->ix = pPage->nCell-1;
        pCur->skipNext = -1;
      if( iCellIdx>=pPage->nCell ){
      pCur->eState = CURSOR_SKIPNEXT;
      assert( (pPage->nCell>0 || CORRUPT_DB) && iCellIdx<=pPage->nCell );
      assert( pPage==pCur->pPage || CORRUPT_DB );
      assert( bPreserve && (pCur->iPage==iCellDepth || CORRUPT_DB) );
    if( bSkipnext ){
  if( rc==SQLITE_OK ){

  }
    rc = balance(pCur);
    pCur->pPage = pCur->apPage[pCur->iPage];
    }
      releasePage(pCur->apPage[pCur->iPage--]);
    while( pCur->iPage>iCellDepth ){
    pCur->iPage--;
    releasePageNotNull(pCur->pPage);
  if( rc==SQLITE_OK && pCur->iPage>iCellDepth ){
  rc = balance(pCur);
  ** well.  */
  ** walk the cursor up the tree to the internal node and balance it as 
  ** been corrected, so be it. Otherwise, after balancing the leaf node,
  ** tree that we can be sure that any problem in the internal node has
  ** on the leaf node first. If the balance proceeds far enough up the
  ** be either under or overfull. In this case run the balancing algorithm
  ** tricky as the leaf node may be underfull, and the internal node may
  ** replace the cell deleted from the internal node. This is slightly
  ** pCur is pointing to the leaf page from which a cell was removed to
  ** Otherwise, if the entry deleted was on an internal node page, then
  **
  ** never true.
  ** call to balance() repairs the tree, and the if(...) condition is
  ** then the cursor still points to that page. In this case the first
  /* Balance the tree. If the entry deleted was located on a leaf page,

  }
    if( rc ) return rc;
    dropCell(pLeaf, pLeaf->nCell-1, nCell, &rc);
    }
      insertCell(pPage, iCellIdx, pCell-4, nCell+4, pTmp, n, &rc);
    if( rc==SQLITE_OK ){
    rc = sqlite3PagerWrite(pLeaf->pDbPage);
    assert( pTmp!=0 );
    pTmp = pBt->pTmpSpace;
    assert( MX_CELL_SIZE(pBt) >= nCell );
    nCell = pLeaf->xCellSize(pLeaf, pCell);
    if( pCell<&pLeaf->aData[4] ) return SQLITE_CORRUPT_BKPT;
    pCell = findCell(pLeaf, pLeaf->nCell-1);
    }
      n = pCur->pPage->pgno;
    }else{
      n = pCur->apPage[iCellDepth+1]->pgno;
    if( iCellDepth<pCur->iPage-1 ){
    }
      if( rc ) return rc;
      rc = btreeComputeFreeSpace(pLeaf);
    if( pLeaf->nFree<0 ){

    unsigned char *pTmp;
    Pgno n;
    int nCell;
    MemPage *pLeaf = pCur->pPage;
  if( !pPage->leaf ){
  ** node to replace the deleted cell.  */
  ** node. The cell from the leaf node needs to be moved to the internal
  ** by the child-page of the cell that was just deleted from an internal
  ** is currently pointing to the largest entry in the sub-tree headed
  /* If the cell deleted was not located on a leaf page, then the cursor

  if( rc ) return rc;
  dropCell(pPage, iCellIdx, info.nSize, &rc);
  rc = clearCell(pPage, pCell, &info);
  if( rc ) return rc;
  rc = sqlite3PagerWrite(pPage->pDbPage);
  ** itself from within the page.  */
  ** overflow pages associated with the entry and finally remove the cell
  /* Make the page containing the entry to be deleted writable. Then free any

  }
    invalidateIncrblobCursors(p, pCur->pgnoRoot, pCur->info.nKey, 0);
  if( pCur->pKeyInfo==0 ){
  ** invalidate any incrblob cursors open on the row being deleted.  */
  /* If this is a delete operation to remove a row from a table b-tree,

  }
    if( rc ) return rc;
    rc = saveAllCursors(pBt, pCur->pgnoRoot, pCur);
  if( pCur->curFlags & BTCF_Multiple ){
  ** making any modifications.  */
  /* Save the positions of any other cursors open on this table before

  }
    if( rc ) return rc;
    assert( rc!=SQLITE_DONE );
    rc = sqlite3BtreePrevious(pCur, 0);
  if( !pPage->leaf ){
  ** balancing the tree following the delete operation easier.  */
  ** sub-tree headed by the child page of the cell being deleted. This makes
  ** of the 'next' entry, as the previous entry is always a part of the
  ** from the internal node. The 'previous' entry is used for this instead
  ** the entry being deleted. This cell will replace the cell being deleted
  ** the cursor to the largest entry in the tree that is smaller than
  /* If the page containing the entry to delete is not a leaf page, move

  }
    }
      bSkipnext = 1;
    }else{
      if( rc ) return rc;
      rc = saveCursorKey(pCur);
      ** Save the cursor key.  */
      /* A b-tree rebalance will be required after deleting this entry.
    ){
     || pPage->nCell==1  /* See dbfuzz001.test for a test case */
     || (pPage->nFree+cellSizePtr(pPage,pCell)+2)>(int)(pBt->usableSize*2/3)
    if( !pPage->leaf 
  if( bPreserve ){
  ** before or after the deleted entry. In this case set bSkipnext to true.  */
  ** will be left in CURSOR_SKIPNEXT state pointing to the entry immediately
  ** Or, if the current delete will not cause a rebalance, then the cursor
  **
  ** returning. 
  ** key and leaving the cursor in CURSOR_REQUIRESEEK state before 
  ** will cause a b-tree rebalance, then this is done by saving the cursor
  ** be preserved following this delete operation. If the current delete
  /* If the bPreserve flag is set to true, then the cursor position must

  if( pPage->nFree<0 && btreeComputeFreeSpace(pPage) ) return SQLITE_CORRUPT;
  pCell = findCell(pPage, iCellIdx);
  pPage = pCur->pPage;
  iCellIdx = pCur->ix;
  iCellDepth = pCur->iPage;

  assert( pCur->eState==CURSOR_VALID );
  }
    if( rc ) return rc;
    rc = btreeRestoreCursorPosition(pCur);
  if( pCur->eState==CURSOR_REQUIRESEEK ){
  assert( (flags & ~(BTREE_SAVEPOSITION | BTREE_AUXDELETE))==0 );
  assert( !hasReadConflicts(p, pCur->pgnoRoot) );
  assert( hasSharedCacheTableLock(p, pCur->pgnoRoot, pCur->pKeyInfo!=0, 2) );
  assert( pCur->curFlags & BTCF_WriteFlag );
  assert( (pBt->btsFlags & BTS_READ_ONLY)==0 );
  assert( pBt->inTransaction==TRANS_WRITE );
  assert( cursorOwnsBtShared(pCur) );

  u8 bPreserve = flags & BTREE_SAVEPOSITION;  /* Keep cursor valid */
  int bSkipnext = 0;                   /* Leaf cursor in SKIPNEXT state */
  CellInfo info;                       /* Size of the cell being deleted */
  int iCellDepth;                      /* Depth of node containing pCell */ 
  int iCellIdx;                        /* Index of cell to delete */
  unsigned char *pCell;                /* Pointer to cell to delete */
  MemPage *pPage;                      /* Page to delete cell from */
  int rc;                              /* Return code */
  BtShared *pBt = p->pBt;              
  Btree *p = pCur->pBtree;
int sqlite3BtreeDelete(BtCursor *pCur, u8 flags){
*/
** but which might be used by alternative storage engines.
** The BTREE_AUXDELETE bit is a hint that is not used by this implementation,
** operation on non-FORDELETE cursors is tagged with the AUXDELETE flag.
** on a cursor that is not a BTREE_FORDELETE cursor.  All but one delete
** deletes is considered the "primary" delete.  The primary delete occurs
** associated with a single table entry and its indexes.  Only one of those
** The BTREE_AUXDELETE bit of flags indicates that is one of several deletes
**
** as it would have been on if the call to BtreeDelete() had been omitted.
** the next call to BtreeNext() or BtreePrev() moves it to the same row
** But if that bit is set, then the cursor is left in a state such that
** the cursor is left pointing at an arbitrary location after the delete.
** If the BTREE_SAVEPOSITION bit of the flags parameter is zero, then
**
** Delete the entry that the cursor is pointing to. 
/*

}
  return rc;
end_insert:

  assert( pCur->iPage<0 || pCur->pPage->nOverflow==0 );
  }
    }
      pCur->nKey = pX->nKey;
      pCur->eState = CURSOR_REQUIRESEEK;
      }
        }
          memcpy(pCur->pKey, pX->pKey, pX->nKey);
        }else{
          rc = SQLITE_NOMEM;
        if( pCur->pKey==0 ){
        pCur->pKey = sqlite3Malloc( pX->nKey );
        assert( pCur->pKey==0 );
      if( pCur->pKeyInfo ){
      btreeReleaseAllCursorPages(pCur);
    if( (flags & BTREE_SAVEPOSITION) && rc==SQLITE_OK ){
    pCur->eState = CURSOR_INVALID;
    pCur->pPage->nOverflow = 0;
    ** from trying to save the current position of the cursor.  */
    ** Also, set the cursor state to invalid. This stops saveCursorPosition()
    ** fails. Internal data structure corruption will result otherwise. 
    /* Must make sure nOverflow is reset to zero even if the balance()

    rc = balance(pCur);
    pCur->curFlags &= ~(BTCF_ValidNKey);
    assert( rc==SQLITE_OK );
  if( pPage->nOverflow ){
  pCur->info.nSize = 0;
  */
  ** row without seeking the cursor. This can be a big performance boost.
  ** larger than the largest existing key, it is possible to insert the
  ** entry in the table, and the next row inserted has an integer key
  ** the b-tree if possible. If the cursor is left pointing to the last
  ** is advantageous to leave the cursor pointing to the last entry in
  ** happen while processing an "INSERT INTO ... SELECT" statement), it
  ** multiple records into an intkey b-tree using a single cursor (as can
  ** There is a subtle but important optimization here too. When inserting
  **
  ** slightly faster.
  ** set the cursor state to "invalid". This makes common insert operations
  ** of BtCursor.apPage[] and BtCursor.aiIdx[]. Instead of doing that,
  ** back to the root page as balance() used to invalidate the contents
  ** Previous versions of SQLite called moveToRoot() to move the cursor
  **
  ** variables.
  ** the cursor, zero the BtCursor.info.nSize and BTCF_ValidNKey
  ** to redistribute the cells within the tree. Since balance() may move
  /* If no error has occurred and pPage has an overflow cell, call balance() 

  assert( rc!=SQLITE_OK || pPage->nCell>0 || pPage->nOverflow>0 );
  assert( pPage->nOverflow==0 || rc==SQLITE_OK );
  insertCell(pPage, idx, newCell, szNew, 0, 0, &rc);
  }
    assert( pPage->leaf );
  }else{
    pCur->curFlags &= ~BTCF_ValidNKey;
    idx = ++pCur->ix;
    assert( pPage->leaf );
  }else if( loc<0 && pPage->nCell>0 ){
    if( rc ) goto end_insert;
    dropCell(pPage, idx, info.nSize, &rc);
    }
      return SQLITE_OK;
      memcpy(oldCell, newCell, szNew);
      }
        return SQLITE_CORRUPT_BKPT;
      if( oldCell+szNew > pPage->aDataEnd ){
      }
        return SQLITE_CORRUPT_BKPT;
      if( oldCell < pPage->aData+pPage->hdrOffset+10 ){
      assert( rc==SQLITE_OK ); /* clearCell never fails when nLocal==nPayload */
      ** necessary to add the PTRMAP_OVERFLOW1 pointer-map entry.  */
      ** new entry uses overflow pages, as the insertCell() call below is
      ** This optimization cannot be used on an autovacuum database if the
      **
      ** calling dropCell() and insertCell(). 
      ** doing that is no faster then skipping this optimization and just
      ** the leftover space to the free list.  But experiments show that
      ** We could also try to do this if the old cell is smaller, then add
      /* Overwrite the old cell with the new if they are the same size.
    ){
     && (!ISAUTOVACUUM || szNew<pPage->minLocal)
    if( info.nSize==szNew && info.nLocal==info.nPayload 
    invalidateOverflowCache(pCur);
    testcase( pCur->curFlags & BTCF_ValidOvfl );
    rc = clearCell(pPage, oldCell, &info);
    }
      memcpy(newCell, oldCell, 4);
    if( !pPage->leaf ){
    oldCell = findCell(pPage, idx);
    }
      goto end_insert;
    if( rc ){
    rc = sqlite3PagerWrite(pPage->pDbPage);
    assert( idx<pPage->nCell );
    CellInfo info;
  if( loc==0 ){
  idx = pCur->ix;
  assert( szNew <= MX_CELL_SIZE(pBt) );
  assert( szNew==pPage->xCellSize(pPage, newCell) );
  if( rc ) goto end_insert;
  rc = fillInCell(pPage, newCell, pX, &szNew);
  assert( newCell!=0 );
  newCell = pBt->pTmpSpace;
  assert( pPage->isInit );
          loc==0 ? "overwrite" : "new entry"));
          pCur->pgnoRoot, pX->nKey, pX->nData, pPage->pgno,
  TRACE(("INSERT: table=%d nkey=%lld ndata=%d page=%d %s\n",

  }
    if( rc ) return rc;
    rc = btreeComputeFreeSpace(pPage);
  if( pPage->nFree<0 ){
  assert( pPage->leaf || !pPage->intKey );
  assert( pPage->intKey || pX->nKey>=0 );
  pPage = pCur->pPage;

       || CORRUPT_DB );
       || (pCur->eState==CURSOR_INVALID && loc)
  assert( pCur->eState==CURSOR_VALID 
  }

    }
      }
        return btreeOverwriteCell(pCur, &x2);
        x2.nZero = 0;
        x2.nData = pX->nKey;
        x2.pData = pX->pKey;
        BtreePayload x2;
      if( pCur->info.nKey==pX->nKey ){
      getCellInfo(pCur);
    if( loc==0 ){
    */
    ** overwrite optimization.
    ** and the new content is the same as as the old, then use the
    /* If the cursor is currently pointing to an entry to be overwritten

    }
      if( rc ) return rc;
      }
        rc = btreeMoveto(pCur, pX->pKey, pX->nKey, flags!=0, &loc);
      }else{
        rc = sqlite3BtreeMovetoUnpacked(pCur, &r, 0, flags!=0, &loc);
        r.eqSeen = 0;
        r.r2 = 0;
        r.r1 = 0;
        r.errCode = 0;
        r.default_rc = 0;
        r.nField = pX->nMem;
        r.aMem = pX->aMem;
        r.pKeyInfo = pCur->pKeyInfo;
        UnpackedRecord r;
      if( pX->nMem ){
    if( loc==0 && (flags & BTREE_SAVEPOSITION)==0 ){
    */
    ** so that it does.
    ** not pointing to an immediately adjacent cell, then move the cursor
    ** overwritten, or if a new cell is being inserted, if the cursor is
    /* If the cursor is not already pointing either to the cell to be

    assert( (flags & BTREE_SAVEPOSITION)==0 || loc==0 );
    */
    ** to a row with the same key as the new entry being inserted.
    /* If BTREE_SAVEPOSITION is set, the cursor must already be pointing 

    /* This is an index or a WITHOUT ROWID table */
  }else{
    }
      if( rc ) return rc;
      rc = sqlite3BtreeMovetoUnpacked(pCur, 0, pX->nKey, flags!=0, &loc);
      */
      ** to the cell to be overwritten or an adjacent cell.
      ** to an adjacent cell.  Move the cursor so that it is pointing either
      /* The cursor is *not* pointing to the cell to be overwritten, nor
    }else if( loc==0 ){
      assert( loc==0 );
      }
        return btreeOverwriteCell(pCur, pX);
        /* New entry is the same size as the old.  Do an overwrite */
      ){
       && pCur->info.nPayload==(u32)pX->nData+pX->nZero
      if( pCur->info.nSize!=0
      assert( pX->nData>=0 && pX->nZero>=0 );
      ** overwritten */
      /* The cursor is pointing to the entry that is to be
    if( (pCur->curFlags&BTCF_ValidNKey)!=0 && pX->nKey==pCur->info.nKey ){
    */
    ** So do a complete check.
    ** that the cursor is not pointing to a row to be overwritten.
    /* On the other hand, BTREE_SAVEPOSITION==0 does not imply

#endif
    }
      assert( loc==0 );
      assert( pX->nKey==pCur->info.nKey );
      assert( pCur->curFlags & BTCF_ValidNKey );
    if( flags & BTREE_SAVEPOSITION ){
#ifdef SQLITE_DEBUG
    */
    ** to a row with the same key as the new entry being inserted.
    /* If BTREE_SAVEPOSITION is set, the cursor must already be pointing 

    invalidateIncrblobCursors(p, pCur->pgnoRoot, pX->nKey, 0);
    ** cursors open on the row being replaced */
    /* If this is an insert into a table b-tree, invalidate any incrblob 
    assert( pX->pKey==0 );
  if( pCur->pKeyInfo==0 ){

  }
    if( rc ) return rc;
    rc = saveAllCursors(pBt, pCur->pgnoRoot, pCur);
  if( pCur->curFlags & BTCF_Multiple ){
  */
  ** not to clear the cursor here.
  ** doing any work. To avoid thwarting these optimizations, it is important
  ** that the cursor is already where it needs to be and returns without
  ** data into the intkey B-Tree. In this case btreeMoveto() recognizes
  ** integer key to use. It then calls this function to actually insert the 
  ** keys, the VDBE layer invokes sqlite3BtreeLast() to figure out the 
  ** example, when inserting data into a table with auto-generated integer
  ** In some cases, the call to btreeMoveto() below is a no-op. For
  **
  /* Save the positions of any other cursors open on this table.

  assert( (pX->pKey==0)==(pCur->pKeyInfo==0) );
  ** blob of associated data.  */
  ** intkey table, the caller should be inserting integer keys with a
  ** keys with no associated data. If the cursor was opened expecting an
  ** expecting an index b-tree, then the caller should be inserting blob
  /* Assert that the caller has been consistent. If this cursor was opened

  assert( hasSharedCacheTableLock(p, pCur->pgnoRoot, pCur->pKeyInfo!=0, 2) );
              && (pBt->btsFlags & BTS_READ_ONLY)==0 );
              && pBt->inTransaction==TRANS_WRITE
  assert( (pCur->curFlags & BTCF_WriteFlag)!=0
  assert( cursorOwnsBtShared(pCur) );

  }
    return pCur->skipNext;
    assert( pCur->skipNext!=SQLITE_OK );
  if( pCur->eState==CURSOR_FAULT ){

  assert( (flags & (BTREE_SAVEPOSITION|BTREE_APPEND))==flags );

  unsigned char *newCell = 0;
  unsigned char *oldCell;
  BtShared *pBt = p->pBt;
  Btree *p = pCur->pBtree;
  MemPage *pPage;
  int idx;
  int szNew = 0;
  int loc = seekResult;          /* -1: before desired location  +1: after */
  int rc;
){
  int seekResult                 /* Result of prior MovetoUnpacked() call */
  int flags,                     /* True if this is likely an append */
  const BtreePayload *pX,        /* Content of the row to be inserted */
  BtCursor *pCur,                /* Insert data into the table of this cursor */
int sqlite3BtreeInsert(
*/
** to decode the key.
** key values and pX->aMem can be used instead of pX->pKey to avoid having
** if pX->nMem is non-zero, then pX->aMem contains pointers to the unpacked
** point for (pKey,nKey) before doing the insertion.  For index btrees,
** In that case, this routine must seek the cursor to the correct insertion
** If seekResult==0, that means pCur is pointing at some unknown location.
**
** that is larger than (pKey,nKey).
** smaller then (pKey,nKey).  If seekResult>0 then pCur points to a cell
** to be inserted.  If seekResult<0 then pCur points to a cell that is
** is currently pointing to a cell that will be adjacent to the cell
** been performed.  In other words, if seekResult!=0 then the cursor
** MovetoUnpacked() to seek cursor pCur to (pKey,nKey) has already
** If the seekResult parameter is non-zero, then a successful call to
**
** pX.pData,nData,nZero fields must be zero.
** key is an arbitrary byte sequence stored in pX.pKey,nKey.  The 
** For an index btree (used for indexes and WITHOUT ROWID tables), the
**
** hold the content of the row.
** rowid or INTEGER PRIMARY KEY of the row.  The pX.nData,pData,nZero fields
** the key is used. The pX.pKey value must be NULL.  The pX.nKey is the
** For a table btree (used for rowid tables), only the pX.nKey value of
**
** pointing at a random location.
** define what table the record should be inserted into, and is left
** is described by the pX object.  The pCur cursor is used only to
** Insert a new record into the BTree.  The content of the new record
/*


}
  return SQLITE_OK;    
  }while( iOffset<nTotal );
    iOffset += ovflPageSize;
    if( rc ) return rc;
    sqlite3PagerUnref(pPage->pDbPage);
    }
                                 iOffset, ovflPageSize);
      rc = btreeOverwriteContent(pPage, pPage->aData+4, pX,
      }
        ovflPageSize = nTotal - iOffset;
      }else{
        ovflPgno = get4byte(pPage->aData);
      if( iOffset+ovflPageSize<(u32)nTotal ){
    }else{
      rc = SQLITE_CORRUPT_BKPT;
    if( sqlite3PagerPageRefcount(pPage->pDbPage)!=1 ){
    if( rc ) return rc;
    rc = btreeGetPage(pBt, ovflPgno, &pPage, 0);
  do{
  ovflPageSize = pBt->usableSize - 4;
  pBt = pPage->pBt;
  ovflPgno = get4byte(pCur->info.pPayload + iOffset);
  assert( iOffset>=0 );
  assert( nTotal>=0 );
  iOffset = pCur->info.nLocal;
  /* Now overwrite the overflow pages */

  if( pCur->info.nLocal==nTotal ) return SQLITE_OK;
  if( rc ) return rc;
                             0, pCur->info.nLocal);
  rc = btreeOverwriteContent(pPage, pCur->info.pPayload, pX,
  /* Overwrite the local portion first */
  }
    return SQLITE_CORRUPT_BKPT;
  ){
   || pCur->info.pPayload < pPage->aData + pPage->cellOffset
  if( pCur->info.pPayload + pCur->info.nLocal > pPage->aDataEnd

  u32 ovflPageSize;                   /* Size to write on overflow page */
  Pgno ovflPgno;                      /* Next overflow page to write */
  BtShared *pBt;                      /* Btree */
  MemPage *pPage = pCur->pPage;       /* Page being written */
  int rc;                             /* Return code */
  int nTotal = pX->nData + pX->nZero; /* Total bytes of to write */
  int iOffset;                        /* Next byte of pX->pData to write */
static int btreeOverwriteCell(BtCursor *pCur, const BtreePayload *pX){
*/
** contained in pX.
** Overwrite the cell that cursor pCur is pointing to with fresh content
/*

}
  return SQLITE_OK;
  }
    }
      memmove(pDest, ((u8*)pX->pData) + iOffset, iAmt);
      ** memmove(). */
      ** corrupt but it does cause valgrind and ASAN warnings.  So use
      ** buffers to overlap.  This is harmless since the database is already
      /* In a corrupt database, it is possible for the source and destination
      if( rc ) return rc;
      int rc = sqlite3PagerWrite(pPage->pDbPage);
    if( memcmp(pDest, ((u8*)pX->pData) + iOffset, iAmt)!=0 ){
    }
      iAmt = nData;
      if( rc ) return rc;
                                 iAmt-nData);
      int rc = btreeOverwriteContent(pPage, pDest+nData, pX, iOffset+nData,
      ** to write the zeros then fall through to write the real data */
      /* Mixed read data and zeros at the end.  Make a recursive call
    if( nData<iAmt ){
  }else{
    }
      memset(pDest + i, 0, iAmt - i);
      if( rc ) return rc;
      int rc = sqlite3PagerWrite(pPage->pDbPage);
    if( i<iAmt ){
    for(i=0; i<iAmt && pDest[i]==0; i++){}
    int i;
    /* Overwritting with zeros */
  if( nData<=0 ){
  int nData = pX->nData - iOffset;
){
  int iAmt                  /* Number of bytes to be written */
  int iOffset,              /* Offset of first byte to write */
  const BtreePayload *pX,   /* Source of data to write */
  u8 *pDest,                /* Pointer to the place to start writing */
  MemPage *pPage,           /* MemPage on which writing will occur */
static int btreeOverwriteContent(
*/
** content is different from what is already there.
/* Overwrite content from pX into pDest.  Only do the write if the

}
  return rc;
  }
    sqlite3PageFree(pFree);
  if( pFree ){

  }while( rc==SQLITE_OK );
    }
      pCur->pPage = pCur->apPage[pCur->iPage];
      assert( pCur->iPage>=0 );
      pCur->iPage--;
      releasePage(pPage);
      /* The next iteration of the do-loop balances the parent page. */

      pPage->nOverflow = 0;

      }
        }
          pFree = pSpace;
          ** comes first. */
          ** balance_nonroot(), or just before this function returns, whichever
          /* The pSpace buffer will be freed after the next call to

          }
            sqlite3PageFree(pFree);
            ** new pSpace buffer, so it may be safely freed here. */
            ** now stored either on real database pages or within the 
            ** by a previous call to balance_nonroot(). Its contents are
            /* If pFree is not NULL, it points to the pSpace buffer used 
          if( pFree ){
                               pCur->hints&BTREE_BULKLOAD);
          rc = balance_nonroot(pParent, iIdx, pSpace, iPage==1,
          u8 *pSpace = sqlite3PageMalloc(pCur->pBt->pageSize);
          */
          ** pSpace buffer passed to the latter call to balance_nonroot().
          ** copied either into the body of a database page or into the new
          ** the previous call, as the overflow cell data will have been 
          ** has completed, it is safe to release the pSpace buffer used by
          ** different page). Once this subsequent call to balance_nonroot() 
          ** but it doesn't deal with overflow cells - just moves them to a
          ** calling balance_nonroot() (balance_deeper() may be called first,
          ** A subsequent iteration of the do-loop will deal with this by
          ** are stored in the pSpace buffer allocated immediately below. 
          ** If the parent page becomes overfull, the overflow cell or cells
          ** 
          ** will balance the parent page to correct this.
          ** become overfull or underfull. The next iteration of the do-loop
          ** modifying the contents of pParent, which may cause pParent to
          ** between pPage and up to 2 of its sibling pages. This involves
          /* In this case, call balance_nonroot() to redistribute cells
        {
#endif
        }else
          rc = balance_quick(pParent, pPage, aBalanceQuickSpace);
          VVA_ONLY( balance_quick_called++ );
          assert( balance_quick_called==0 ); 
          */
          ** of the aBalanceQuickSpace[] might sneak in.
          ** function. If this were not verified, a subtle bug involving reuse
          ** single call to balance_quick() is made for each call to this
          ** The purpose of the following assert() is to check that only a
          **
          ** buffer. 
          ** happens, the overflow cell is stored in the aBalanceQuickSpace[]
          ** use either balance_nonroot() or balance_deeper(). Until this
          ** happens, the next iteration of the do-loop will balance pParent 
          ** into pParent, which may cause pParent overflow. If this
          ** to store the overflow cell. balance_quick() inserts a new cell
          /* Call balance_quick() to create a new sibling of pPage on which
        ){
         && pParent->nCell==iIdx
         && pParent->pgno!=1
         && pPage->aiOvfl[0]==pPage->nCell
         && pPage->nOverflow==1
        if( pPage->intKeyLeaf
#ifndef SQLITE_OMIT_QUICKBALANCE
      if( rc==SQLITE_OK ){
      }
        rc = btreeComputeFreeSpace(pParent);
      if( rc==SQLITE_OK && pParent->nFree<0 ){
      rc = sqlite3PagerWrite(pParent->pDbPage);

      int const iIdx = pCur->aiIdx[iPage-1];
      MemPage * const pParent = pCur->apPage[iPage-1];
    }else{
      }
        break;
      }else{
        }
          assert( pCur->pPage->nOverflow );
          pCur->pPage = pCur->apPage[1];
          pCur->apPage[0] = pPage;
          pCur->aiIdx[0] = 0;
          pCur->ix = 0;
          pCur->iPage = 1;
        if( rc==SQLITE_OK ){
        rc = balance_deeper(pPage, &pCur->apPage[1]);
        VVA_ONLY( balance_deeper_called++ );
        assert( balance_deeper_called==0 );
        */ 
        ** next iteration of the do-loop will balance the child page.
        ** and copy the current contents of the root-page to it. The
        ** balance_deeper() function to create a new child for the root-page
        /* The root page of the b-tree is overfull. In this case call the
      if( pPage->nOverflow && (rc = anotherValidCursor(pCur))==SQLITE_OK ){
    }else if( (iPage = pCur->iPage)==0 ){
      break;
    if( pPage->nOverflow==0 && pPage->nFree<=nMin ){
    if( NEVER(pPage->nFree<0) && btreeComputeFreeSpace(pPage) ) break;

    MemPage *pPage = pCur->pPage;
    int iPage;
  do {

  VVA_ONLY( int balance_deeper_called = 0 );
  VVA_ONLY( int balance_quick_called = 0 );

  u8 *pFree = 0;
  u8 aBalanceQuickSpace[13];
  const int nMin = pCur->pBt->usableSize * 2 / 3;
  int rc = SQLITE_OK;
static int balance(BtCursor *pCur){
*/
**   balance_nonroot()
**   balance_deeper()
**   balance_quick()
**
** routine. Balancing routines are:
** tree needs to be balanced, and if so calls the appropriate balancing 
** some way. This function figures out if this modification means the
** The page that pCur currently points to has just been modified in
/*

}
  return SQLITE_OK;
  }
    }
      return SQLITE_CORRUPT_BKPT;
    ){
     && pOther->pPage==pCur->pPage
     && pOther->eState==CURSOR_VALID
    if( pOther!=pCur
  for(pOther=pCur->pBt->pCursor; pOther; pOther=pOther->pNext){
  BtCursor *pOther;
static int anotherValidCursor(BtCursor *pCur){
*/
** first SQL table, violating invariants on the first insert.
** rebalance, that can change content out from under the cursor on the
** table linked to the same b-tree.  If the secondary insert causes a
** and causes a BEFORE TRIGGER to do a secondary insert on the other SQL
** pointing to the same b-tree.  If an insert occurs on one SQL table
** This can if a database is corrupt with two or more SQL tables
**
** on the same B-tree as pCur.
** Return SQLITE_CORRUPT if any cursor other than pCur is currently valid
/*

}
  return SQLITE_OK;
  *ppChild = pChild;

  put4byte(&pRoot->aData[pRoot->hdrOffset+8], pgnoChild);
  zeroPage(pRoot, pChild->aData[0] & ~PTF_LEAF);
  /* Zero the contents of pRoot. Then install pChild as the right-child. */

  pChild->nOverflow = pRoot->nOverflow;
         pRoot->nOverflow*sizeof(pRoot->apOvfl[0]));
  memcpy(pChild->apOvfl, pRoot->apOvfl,
         pRoot->nOverflow*sizeof(pRoot->aiOvfl[0]));
  memcpy(pChild->aiOvfl, pRoot->aiOvfl,
  /* Copy the overflow cells from pRoot to pChild */

  TRACE(("BALANCE: copy root %d into %d\n", pRoot->pgno, pChild->pgno));

  assert( pChild->nCell==pRoot->nCell || CORRUPT_DB );
  assert( sqlite3PagerIswriteable(pRoot->pDbPage) );
  assert( sqlite3PagerIswriteable(pChild->pDbPage) );
  }
    return rc;
    releasePage(pChild);
    *ppChild = 0;
  if( rc ){
  }
    }
      ptrmapPut(pBt, pgnoChild, PTRMAP_BTREE, pRoot->pgno, &rc);
    if( ISAUTOVACUUM ){
    copyNodeContent(pRoot, pChild, &rc);
    rc = allocateBtreePage(pBt,&pChild,&pgnoChild,pRoot->pgno,0);
  if( rc==SQLITE_OK ){
  rc = sqlite3PagerWrite(pRoot->pDbPage);
  */
  ** of the node stored on pRoot into the new child page.
  ** page that will become the new right-child of pPage. Copy the contents
  /* Make pRoot, the root page of the b-tree, writable. Allocate a new 

  assert( sqlite3_mutex_held(pBt->mutex) );
  assert( pRoot->nOverflow>0 );

  BtShared *pBt = pRoot->pBt;    /* The BTree */
  Pgno pgnoChild = 0;            /* Page number of the new child page */
  MemPage *pChild = 0;           /* Pointer to a new child page */
  int rc;                        /* Return value from subprocedures */
static int balance_deeper(MemPage *pRoot, MemPage **ppChild){
*/
** an error code is returned and *ppChild is set to 0.
** to call releasePage() on *ppChild exactly once. If an error occurs,
** page and SQLITE_OK is returned. In this case the caller is required
** If successful, *ppChild is set to contain a reference to the child 
**
** page is also updated.
** entry corresponding to the new right-child pointer of the root
** that the new child-page now contains pointers to are updated. The
** Before returning, all pointer-map entries corresponding to pages 
**
** pointer pointing to the new page.
** page is then overwritten to make it an empty page with the right-child 
** page, including overflow cells, are copied into the child. The root
** A new child page is allocated and the contents of the current root
**
** overfull (has one or more overflow pages).
** This function is called when the root page of a b-tree structure is
/*


}
  return rc;

  }
    releasePage(apNew[i]);
  for(i=0; i<nNew; i++){
  }
    releasePage(apOld[i]);
  for(i=0; i<nOld; i++){
  sqlite3StackFree(0, b.apCell);
balance_cleanup:
  */
  ** Cleanup before returning.
  /*

#endif
  }
    ptrmapCheckPages(&pParent, 1);
    ptrmapCheckPages(apNew, nNew);
    ** cause an assert() statement to fail.  */
    ** debugging. This is usually disabled because a corrupt database may
    ** all pointer map pages are set correctly. This is helpful while 
    /* The ptrmapCheckPages() contains assert() statements that verify that
  if( ISAUTOVACUUM && rc==SQLITE_OK && apNew[0]->isInit ){
#if 0

  }
    freePage(apOld[i], &rc);
  for(i=nNew; i<nOld; i++){
  */
  /* Free any old pages that were not reused as new pages.

          nOld, nNew, b.nCell));
  TRACE(("BALANCE: finished: old=%d new=%d cells=%d\n",
  assert( pParent->isInit );

  }
    }
      ptrmapPut(pBt, key, PTRMAP_BTREE, apNew[i]->pgno, &rc);
      u32 key = get4byte(&apNew[i]->aData[8]);
    for(i=0; i<nNew; i++){
    ** care of.  */
    ** sibling page. All other pointer map entries have already been taken
    /* Fix the pointer map entries associated with the right-child of each
  }else if( ISAUTOVACUUM && !leafCorrection ){
    freePage(apNew[0], &rc);
    copyNodeContent(apNew[0], pParent, &rc);
    );
      || rc!=SQLITE_OK
          - apNew[0]->nCell*2)
        (get2byteNotZero(&apNew[0]->aData[5]) - apNew[0]->cellOffset
    assert( apNew[0]->nFree == 
    testcase( rc!=SQLITE_OK );
    rc = defragmentPage(apNew[0], -1);
    assert( nNew==1 || CORRUPT_DB );
    */
    ** free space needs to be up front.
    ** by smaller than the child due to the database header, and so all the
    ** copied into the parent, because if the parent is page 1 then it will
    ** It is critical that the child page be defragmented before being
    **
    ** for which the pointer is stored within the content being copied.
    ** sets all pointer-map entries corresponding to database image pages 
    ** If this is an auto-vacuum database, the call to copyNodeContent() 
    **
    ** sub-algorithm in some documentation.
    ** b-tree structure by one. This is described as the "balance-shallower"
    ** child page into the parent, decreasing the overall height of the
    ** page is the right-child of the parent. Copy the contents of the
    /* The root page of the b-tree now contains no cells. The only sibling
  if( isRoot && pParent->nCell==0 && pParent->hdrOffset<=apNew[0]->nFree ){

  assert( nNew>0 );
  assert( nOld>0 );

  assert( memcmp(abDone, "\01\01\01\01\01", nNew)==0 );
  /* All pages have been processed exactly once */

  }
    }
      assert( apNew[iPg]->nCell==nNewCell );
      assert( apNew[iPg]->nOverflow==0 );
      apNew[iPg]->nFree = usableSpace-szNew[iPg];
      abDone[iPg]++;
      if( rc ) goto balance_cleanup;
      rc = editPage(apNew[iPg], iOld, iNew, nNewCell, &b);

      }
        nNewCell = cntNew[iPg] - iNew;
        iNew = cntNew[iPg-1] + !leafData;
        iOld = iPg<nOld ? (cntOld[iPg-1] + !leafData) : b.nCell;
      }else{
        nNewCell = cntNew[0];
        iNew = iOld = 0;
      if( iPg==0 ){

      assert( cntNew[iPg]>=cntOld[iPg] || abDone[iPg+1] );
      ** only after iPg+1 has already been updated. */
      /* Verify condition (2):  If cells are moving right, update iPg

      assert( iPg==0 || cntOld[iPg-1]>=cntNew[iPg-1] || abDone[iPg-1] );
      ** only after iPg-1 has already been updated. */
      /* Verify condition (1):  If cells are moving left, update iPg

      int nNewCell;
      int iOld;
      int iNew;
    ){
     || cntOld[iPg-1]>=cntNew[iPg-1]    /* Condition (1) is true */
    if( i>=0                            /* On the upwards pass, or... */
    if( abDone[iPg] ) continue;         /* Skip pages already processed */
    assert( iPg>=0 && iPg<nNew );
    int iPg = i<0 ? -i : i;
  for(i=1-nNew; i<nNew; i++){
  */
  ** pass.
  ** upwards pass simply processes pages that were missed on the downward
  ** step.  On the upward pass, both conditions are always true, so the
  ** needs to be tested because (2) will always be true from the previous
  ** the pages.  On the initial downward pass, only condition (1) above
  ** to 0, then back up to nNew-1 again, thus making two passes over
  ** The iPg value in the following loop starts at nNew-1 goes down
  **
  ** If neither of the above apply, the page is safe to update.
  **
  **      the right-hand sibling apNew[iPg+1] has been updated.
  **      then it is not safe to update page apNew[iPg] until after
  **  (2) If cells are moving right (from apNew[iPg] to apNew[iPg+1])
  **
  **      the left-hand sibling apNew[iPg-1] has been updated.
  **      then it is not safe to update page apNew[iPg] until after
  **  (1) If cells are moving left (from apNew[iPg] to apNew[iPg-1])
  **
  ** cells may still to be read. In practice, this means:
  ** is important, as this code needs to avoid disrupting any page from which
  /* Now update the actual sibling pages. The order in which they are updated

  }
    assert( sqlite3PagerIswriteable(pParent->pDbPage) );
    if( rc!=SQLITE_OK ) goto balance_cleanup;
    insertCell(pParent, nxDiv+i, pCell, sz, pTemp, pNew->pgno, &rc);
    assert( iOvflSpace <= (int)pBt->pageSize );
    assert( sz<=pBt->maxLocal+23 );
    iOvflSpace += sz;
    }
      }
        sz = pParent->xCellSize(pParent, pCell);
        assert(leafCorrection==4);
      if( b.szCell[j]==4 ){
      */
      ** primary key.
      ** and WITHOUT ROWID tables with exactly one column which is the
      ** This can only happen for b-trees used to evaluate "IN (SELECT ...)"
      **
      ** insertCell(), so reparse the cell now.
      ** any cell). But it is important to pass the correct size to 
      ** (see btreeParseCellPtr(), 4 bytes is the minimum size of
      ** bytes, then it may actually be smaller than this 
      ** previously stored on a leaf node, and its reported size was 4
      /* Obscure case for non-leaf-data trees: If the cell at pCell was
      pCell -= 4;
    }else{
      pTemp = 0;
      sz = 4 + putVarint(&pCell[4], info.nKey);
      pCell = pTemp;
      pNew->xParseCell(pNew, b.apCell[j], &info);
      j--;
      CellInfo info;
      */
      ** the sibling-page assembled above only.
      ** cell consists of the integer key for the right-most cell of 
      ** then there is no divider cell in b.apCell[]. Instead, the divider 
      /* If the tree is a leaf-data tree, and the siblings are leaves, 
    }else if( leafData ){
      memcpy(&pNew->aData[8], pCell, 4);
    if( !pNew->leaf ){
    pTemp = &aOvflSpace[iOvflSpace];
    sz = b.szCell[j] + leafCorrection;
    pCell = b.apCell[j];
    assert( b.apCell[j]!=0 );
    assert( j<nMaxCells );

    j = cntNew[i];
    MemPage *pNew = apNew[i];
    int sz;
    u8 *pTemp;
    u8 *pCell;
  for(i=0; i<nNew-1; i++){
  /* Insert new divider cells into pParent. */

  }
    }
      }
        if( rc ) goto balance_cleanup;
        }
          ptrmapPutOvflPtr(pNew, pOld, pCell, &rc);
        if( cachedCellSize(&b,i)>pNew->minLocal ){
        }
          ptrmapPut(pBt, get4byte(pCell), PTRMAP_BTREE, pNew->pgno, &rc);
        if( !leafCorrection ){
      ){
       || !SQLITE_WITHIN(pCell,pOld->aData,pOld->aDataEnd)
       || pNew->pgno!=aPgno[iOld]
      if( iOld>=nNew
      ** overflow cell), we can skip updating the pointer map entries.  */
      ** pCell really was a part of sibling page iOld (not a divider or
      ** if sibling page iOld had the same page number as pNew, and if
      ** or else the divider cell to the left of sibling page iOld. So,
      ** was either part of sibling page iOld (possibly an overflow cell), 
      /* Cell pCell is destined for new sibling page pNew. Originally, it

      }
        if( !leafData ) continue;
        pNew = apNew[++iNew];
      if( i==cntNew[iNew] ){
      }
        cntOldNext += pOld->nCell + pOld->nOverflow + !leafData;
        pOld = iOld<nNew ? apNew[iOld] : apOld[iOld];
        assert( iOld>=0 && iOld<NB );
        assert( iOld<nNew || iOld<nOld );
        iOld++;
      while( i==cntOldNext ){
      u8 *pCell = b.apCell[i];
    for(i=0; i<b.nCell; i++){

    int iOld = 0;
    int iNew = 0;
    int cntOldNext = pNew->nCell + pNew->nOverflow;
    MemPage *pNew = pOld = apNew[0];
    MemPage *pOld;
  if( ISAUTOVACUUM ){
  */
  ** populated, not here.
  ** updated. This happens below, after the sibling pages have been 
  ** associated with the right-child of each sibling may also need to be 
  ** If the sibling pages are not leaves, then the pointer map entry 
  **
  **      with the cell.
  **   b) if the sibling pages are not leaves, the child page associated
  **
  **      entry associated with the first page in the overflow chain, and
  **   a) if the cell contains a reference to an overflow chain, the
  **
  ** routine. The associated pointer map entries are:
  ** map entries associated with divider cells are set by the insertCell()
  ** cells stored on sibling pages following the balance operation. Pointer
  /* Make any required updates to pointer map entries associated with 

  }
    memcpy(&apNew[nNew-1]->aData[8], &pOld->aData[8], 4);
    MemPage *pOld = (nNew>nOld ? apNew : apOld)[nOld-1];
  if( (pageFlags & PTF_LEAF)==0 && nOld!=nNew ){
  ** originally in the same field of the right-most old sibling page. */
  ** of the right-most new sibling page is set to the value that was 
  /* If the sibling pages are not leaves, ensure that the right-child pointer

  put4byte(pRight, apNew[nNew-1]->pgno);
  assert( apNew[nNew-1]!=0 );
  assert( nNew>=1 && nNew<=ArraySize(apNew) );
  assert( sqlite3PagerIswriteable(pParent->pDbPage) );

  ));
    nNew>=5 ? cntNew[4] - cntNew[3] - !leafData : 0
    nNew>=5 ? apNew[4]->pgno : 0, nNew>=5 ? szNew[4] : 0,
    nNew>=4 ? cntNew[3] - cntNew[2] - !leafData : 0,
    nNew>=4 ? apNew[3]->pgno : 0, nNew>=4 ? szNew[3] : 0,
    nNew>=3 ? cntNew[2] - cntNew[1] - !leafData : 0,
    nNew>=3 ? apNew[2]->pgno : 0, nNew>=3 ? szNew[2] : 0,
    nNew>=2 ? cntNew[1] - cntNew[0] - !leafData : 0,
    nNew>=2 ? apNew[1]->pgno : 0, nNew>=2 ? szNew[1] : 0,
    apNew[0]->pgno, szNew[0], cntNew[0],
         "%d(%d nc=%d) %d(%d nc=%d)\n",
  TRACE(("BALANCE: new: %d(%d nc=%d) %d(%d nc=%d) %d(%d nc=%d) "

  }
    }
      apNew[i]->pgno = pgno;
      sqlite3PagerRekey(apNew[i]->pDbPage, pgno, aPgFlags[iBest]);
      }
        sqlite3PagerRekey(apNew[iBest]->pDbPage, pBt->nPage+iBest+1, 0);
      if( iBest>i ){
    if( iBest!=i ){
    aPgOrder[iBest] = 0xffffffff;
    pgno = aPgOrder[iBest];
    }
      if( aPgOrder[j]<aPgOrder[iBest] ) iBest = j;
    for(j=1; j<nNew; j++){
    int iBest = 0;                /* aPgno[] index of page number to use */
  for(i=0; i<nNew; i++){
  }
    }
      }
        goto balance_cleanup;
        rc = SQLITE_CORRUPT_BKPT;
        assert( CORRUPT_DB );
        ** page number.  */
        ** cache with two separate objects associated with the same
        ** we do the detection here in order to avoid populating the pager
        ** It would be simpler to detect this as part of the loop below, but
        ** duplicate entries. This can happen if the database is corrupt. 
        /* This branch is taken if the set of sibling pages somehow contains
      if( aPgno[j]==aPgno[i] ){
    for(j=0; j<i; j++){
    aPgFlags[i] = apNew[i]->pDbPage->flags;
    aPgOrder[i] = aPgno[i] = apNew[i]->pgno;
  for(i=0; i<nNew; i++){
  */
  ** for large insertions and deletions.
  ** When NB==3, this one optimization makes the database about 25% faster 
  **
  ** than (NB+2) (a small constant), that should not be a problem.
  ** An O(n^2) insertion sort algorithm is used, but since n is never more 
  **
  ** helps the operating system to deliver pages from the disk more rapidly.
  ** of the table is closer to a linear scan through the file. That in turn 
  ** This helps to keep entries in the disk file in order so that a scan
  ** Reassign page numbers so that the new pages are in ascending order. 
  /*

  }
    }
      }
        }
          goto balance_cleanup;
        if( rc!=SQLITE_OK ){
        ptrmapPut(pBt, pNew->pgno, PTRMAP_BTREE, pParent->pgno, &rc);
      if( ISAUTOVACUUM ){
      /* Set the pointer-map entry for the new sibling page. */

      cntOld[i] = b.nCell;
      nNew++;
      apNew[i] = pNew;
      zeroPage(pNew, pageFlags);
      if( rc ) goto balance_cleanup;
      rc = allocateBtreePage(pBt, &pNew, &pgno, (bBulk ? 1 : pgno), 0);
      assert( i>0 );
    }else{
      if( rc ) goto balance_cleanup;
      nNew++;
      rc = sqlite3PagerWrite(pNew->pDbPage);
      apOld[i] = 0;
      pNew = apNew[i] = apOld[i];
    if( i<nOld ){
    MemPage *pNew;
  for(i=0; i<k; i++){
  pageFlags = apOld[0]->aData[0];
  */
  ** Allocate k new pages.  Reuse old pages where possible.
  /*

  ));
    nOld>=3 ? apOld[2]->pgno : 0, nOld>=3 ? apOld[2]->nCell : 0
    nOld>=2 ? apOld[1]->pgno : 0, nOld>=2 ? apOld[1]->nCell : 0,
    apOld[0]->pgno, apOld[0]->nCell,
  TRACE(("BALANCE: old: %d(nc=%d) %d(nc=%d) %d(nc=%d)\n",
  assert( cntNew[0]>0 || (pParent->pgno==1 && pParent->nCell==0) || CORRUPT_DB);
  */
  **        that page.
  **        the real root page is page 1 and we are the only child of
  **    (2) pPage is a virtual root page.  A virtual root page is when
  **    (1) We found one or more cells (cntNew[0])>0), or
  ** must be true:
  /* Sanity check:  For a non-corrupt database file one of the follwing

  }
    }
      goto balance_cleanup;
      rc = SQLITE_CORRUPT_BKPT;
    if( cntNew[i-1] <= (i>1 ? cntNew[i-2] : 0) ){
    szNew[i-1] = szLeft;
    szNew[i] = szRight;
    }while( r>=0 );
      d--;
      r--;
      cntNew[i-1] = r;
      szLeft -= b.szCell[r] + 2;
      szRight += b.szCell[d] + 2;
      }
        break;
       && (bBulk || szRight+b.szCell[d]+2 > szLeft-(b.szCell[r]+(i==k-1?0:2)))){
      if( szRight!=0
      (void)cachedCellSize(&b, r);
      assert( r<nMaxCells );
      assert( d<nMaxCells );
    do{
    (void)cachedCellSize(&b, d);
    d = r + 1 - leafData;
    r = cntNew[i-1] - 1;

    int d;              /* Index of first cell to the left of right sibling */
    int r;              /* Index of right-most cell in left sibling */
    int szLeft = szNew[i-1]; /* Size of sibling on the left */
    int szRight = szNew[i];  /* Size of sibling on the right */
  for(i=k-1; i>0; i--){
  */
  ** sibling might be completely empty.  This adjustment is not optional.
  ** be so out of balance as to be illegal.  For example, the right-most
  ** This adjustment is more than an optimization.  The packing above might
  **
  ** get a better balance.
  ** The next block of code attempts to adjust the packing of siblings to
  ** always nearly full, while the right-most sibling might be nearly empty.
  ** on the left side (siblings with smaller keys). The left siblings are
  ** The packing computed by the previous block is biased toward the siblings
  /*

  }
    }
      goto balance_cleanup;
      rc = SQLITE_CORRUPT_BKPT;
    }else if( cntNew[i] <= (i>0 ? cntNew[i-1] : 0) ){
      k = i+1;
    if( cntNew[i]>=b.nCell ){
    }
      szNew[i+1] -= sz;
      }
        }
          sz = 0;
        }else{
          sz = 2 + cachedCellSize(&b, cntNew[i]);
        if( cntNew[i]<b.nCell ){
      if( !leafData ){
      cntNew[i]++;
      szNew[i] += sz;
      if( szNew[i]+sz>usableSpace ) break;
      sz = 2 + cachedCellSize(&b, cntNew[i]);
    while( cntNew[i]<b.nCell ){
    }
      cntNew[i]--;
      szNew[i+1] += sz;
      }
        }
          sz = 0;
        }else{
          sz = 2 + cachedCellSize(&b, cntNew[i]);
        if( cntNew[i]<b.nCell ){
      if( !leafData ){
      szNew[i] -= sz;
      sz = 2 + cachedCellSize(&b, cntNew[i]-1);
      }
        cntNew[k-1] = b.nCell;
        szNew[k-1] = 0;
        if( k>NB+2 ){ rc = SQLITE_CORRUPT_BKPT; goto balance_cleanup; }
        k = i+2;
      if( i+1>=k ){
    while( szNew[i]>usableSpace ){
    int sz;
  for(i=0; i<k; i++){
  k = nOld;
  }
    cntNew[i] = cntOld[i];
    }
      szNew[i] += 2 + p->xCellSize(p, p->apOvfl[j]);
    for(j=0; j<p->nOverflow; j++){
    szNew[i] = usableSpace - p->nFree;
    assert( p->nFree>=0 );
    }
      b.ixNx[k] = cntOld[i]+1;
      b.apEnd[k] = pParent->aDataEnd;
      k++;
    if( !leafData ){
    }
      k--;  /* Omit b.ixNx[] entry for child pages with no cells */
    if( k && b.ixNx[k]==b.ixNx[k-1] ){
    b.ixNx[k] = cntOld[i];
    b.apEnd[k] = p->aDataEnd;
    MemPage *p = apOld[i];
  for(i=k=0; i<nOld; i++, k++){
  usableSpace = pBt->usableSize - 12 + leafCorrection;
  */
  ** 
  ** usableSpace: Number of bytes of space available on each sibling.
  **              the right of the i-th sibling page.
  **   cntNew[i]: Index in b.apCell[] and b.szCell[] for the first cell to
  **    szNew[i]: Spaced used on the i-th sibling page.
  **           k: The total number of sibling pages
  **
  ** Values computed by this block:
  **
  ** cntNew[k] should equal b.nCell.
  ** in b.apCell[] of the cell that divides page i from page i+1.  
  ** size of all cells on the i-th page and cntNew[] which is the index
  ** Store this number in "k".  Also compute szNew[] which is the total
  ** Figure out the number of pages needed to hold all b.nCell cells.
  /*

  }
    }
      b.nCell++;
      }
        }
          b.szCell[b.nCell]++;
          aSpace1[iSpace1++] = 0x00;
          assert( b.apCell[b.nCell]==&aSpace1[iSpace1-3] || CORRUPT_DB );
          assert( b.szCell[b.nCell]==3 || CORRUPT_DB );
          ** does exist, pad it with 0x00 bytes. */
          /* Do not allow any cells smaller than 4 bytes. If a smaller cell
        while( b.szCell[b.nCell]<4 ){
        assert( leafCorrection==4 );
      }else{
        memcpy(b.apCell[b.nCell], &pOld->aData[8], 4);
        ** pointer of the divider cell */
        /* The right pointer of the child page pOld becomes the left
        assert( pOld->hdrOffset==0 );
        assert( leafCorrection==0 );
      if( !pOld->leaf ){
      b.szCell[b.nCell] = b.szCell[b.nCell] - leafCorrection;
      assert( leafCorrection==0 || leafCorrection==4 );
      b.apCell[b.nCell] = pTemp+leafCorrection;
      memcpy(pTemp, apDiv[i], sz);
      assert( iSpace1 <= (int)pBt->pageSize );
      assert( sz<=pBt->maxLocal+23 );
      iSpace1 += sz;
      pTemp = &aSpace1[iSpace1];
      b.szCell[b.nCell] = sz;
      assert( b.nCell<nMaxCells );
      u8 *pTemp;
      u16 sz = (u16)szNew[i];
    if( i<nOld-1 && !leafData){
    cntOld[i] = b.nCell;

    assert( (b.nCell-nCellAtStart)==(pOld->nCell+pOld->nOverflow) );
    }
      b.nCell++;
      piCell += 2;
      b.apCell[b.nCell] = aData + (maskPage & get2byteAligned(piCell));
      assert( b.nCell<nMaxCells );
    while( piCell<piEnd ){
    piEnd = aData + pOld->cellOffset + 2*pOld->nCell;
    }
      }
        b.nCell++;
        b.apCell[b.nCell] = pOld->apOvfl[k];
        assert( k==0 || pOld->aiOvfl[k-1]+1==pOld->aiOvfl[k] );/* NOTE 1 */
      for(k=0; k<pOld->nOverflow; k++){
      }
        b.nCell++;
        piCell += 2;
        b.apCell[b.nCell] = aData + (maskPage & get2byteAligned(piCell));
      for(j=0; j<limit; j++){
      limit = pOld->aiOvfl[0];
      }
        goto balance_cleanup;
        rc = SQLITE_CORRUPT_BKPT;
      if( NEVER(limit<pOld->aiOvfl[0]) ){
    if( pOld->nOverflow>0 ){
    memset(&b.szCell[b.nCell], 0, sizeof(b.szCell[0])*(limit+pOld->nOverflow));
    */
    ** first.
    ** long be able to find the cells if a pointer to each cell is not saved
    ** offset section of the btree page will be overwritten and we will no
    ** This must be done in advance.  Once the balance starts, the cell
    **
    ** invariant.
    ** with "NOTE 1" in the overflow cell insertion loop to prove this
    ** adjacent and are inserted in order.  There is an assert() tagged
    ** cells into a parent on a prior balance, and divider cells are always
    ** because multiple overflows can only occurs when inserting divider
    ** case that they are sequential and adjacent.  This invariant arises
    ** Note that when there are multiple overflow cells, it is always the
    **
    ** in the correct spot.
    ** contains overflow cells, include them in the b.apCell[] array
    /* Load b.apCell[] with pointers to all cells in pOld.  If pOld

    }
      goto balance_cleanup;
      rc = SQLITE_CORRUPT_BKPT;
    if( pOld->aData[0]!=apOld[0]->aData[0] ){
    */
    ** table-interior, index-leaf, or index-interior).
    /* Verify that all sibling pages are of the same "type" (table-leaf,

    VVA_ONLY( int nCellAtStart = b.nCell; )
    u8 *piEnd;
    u8 *piCell = aData + pOld->cellOffset;
    u16 maskPage = pOld->maskPage;
    u8 *aData = pOld->aData;
    int limit = pOld->nCell;
    MemPage *pOld = apOld[i];
  for(i=0; i<nOld; i++){
  leafData = b.pRef->intKeyLeaf;
  leafCorrection = b.pRef->leaf*4;
  b.pRef = apOld[0];
  */
  **       leafData:  1 if pPage holds key+data and pParent holds only keys.
  ** leafCorrection:  4 if pPage is a leaf.  0 if pPage is not a leaf.
  **
  ** are alike.
  ** b.apCell[] include child pointers.  Either way, all cells in b.apCell[]
  ** child pointers.  If siblings are not leaves, then all cell in
  ** into aSpace1[].  In this way, all cells in b.apCell[] are without
  ** divider cells are stripped from the cells before they are copied
  ** If the siblings are on leaf pages, then the child pointers of the
  **
  ** been removed from pParent.
  ** into space obtained from aSpace1[]. The divider cells have already
  ** into the local b.apCell[] array.  Make copies of the divider cells
  ** Load pointers to all cells on sibling pages and the divider cells
  /*

  assert( EIGHT_BYTE_ALIGNMENT(aSpace1) );
  aSpace1 = (u8*)&b.szCell[nMaxCells];
  b.szCell = (u16*)&b.apCell[nMaxCells];
  }
    goto balance_cleanup;
    rc = SQLITE_NOMEM_BKPT;
  if( b.apCell==0 ){
  b.apCell = sqlite3StackAllocRaw(0, szScratch );
  assert( szScratch<=7*(int)pBt->pageSize );

     + pBt->pageSize;                              /* aSpace1 */
     + nMaxCells*sizeof(u16)                       /* b.szCell */
       nMaxCells*sizeof(u8*)                       /* b.apCell */
  szScratch =
  */
  ** Allocate space for memory structures
  /*

  nMaxCells = (nMaxCells + 3)&~3;
  nMaxCells = nOld*(MX_CELL(pBt) + ArraySize(pParent->apOvfl));
  ** alignment */
  /* Make nMaxCells a multiple of 4 in order to preserve 8-byte

  }
    }
      dropCell(pParent, i+nxDiv-pParent->nOverflow, szNew[i], &rc);
      }
        }
          apDiv[i] = &aOvflSpace[apDiv[i]-pParent->aData];
          memcpy(&aOvflSpace[iOff], apDiv[i], szNew[i]);
        }else{
          goto balance_cleanup;
          memset(apOld, 0, (i+1)*sizeof(MemPage*));
          rc = SQLITE_CORRUPT_BKPT;
        if( (iOff+szNew[i])>(int)pBt->usableSize ){
        iOff = SQLITE_PTR_TO_INT(apDiv[i]) - SQLITE_PTR_TO_INT(pParent->aData);

        int iOff;
      if( pBt->btsFlags & BTS_FAST_SECURE ){
      ** is allocated.  */
      ** buffer. It will be copied out again as soon as the aSpace[] buffer
      ** In this case, temporarily copy the cell into the aOvflSpace[]
      ** the dropCell() routine will overwrite the entire cell with zeroes.
      ** But not if we are in secure-delete mode. In secure-delete mode,
      **
      ** later on.  
      ** four bytes of the divider cell. So the pointer is safe to use
      ** four bytes of it, and this function does not need the first
      ** This is safe because dropping a cell only overwrites the first
      ** the cell within the parent, even though it has been dropped.
      /* Drop the cell from the parent page. apDiv[i] still points to

      szNew[i] = pParent->xCellSize(pParent, apDiv[i]);
      pgno = get4byte(apDiv[i]);
      apDiv[i] = findCell(pParent, i+nxDiv-pParent->nOverflow);
    }else{
      pParent->nOverflow = 0;
      szNew[i] = pParent->xCellSize(pParent, apDiv[i]);
      pgno = get4byte(apDiv[i]);
      apDiv[i] = pParent->apOvfl[0];
    if( pParent->nOverflow && i+nxDiv==pParent->aiOvfl[0] ){

    if( (i--)==0 ) break;
    }
      }
        goto balance_cleanup;
        memset(apOld, 0, (i)*sizeof(MemPage*));
      if( rc ){
      rc = btreeComputeFreeSpace(apOld[i]);
    if( apOld[i]->nFree<0 ){
    }
      goto balance_cleanup;
      memset(apOld, 0, (i+1)*sizeof(MemPage*));
    if( rc ){
    rc = getAndInitPage(pBt, pgno, &apOld[i], 0, 0);
  while( 1 ){
  pgno = get4byte(pRight);
  }
    pRight = findCell(pParent, i+nxDiv-pParent->nOverflow);
  }else{
    pRight = &pParent->aData[pParent->hdrOffset+8];
  if( (i+nxDiv-pParent->nOverflow)==pParent->nCell ){
  nOld = i+1;
  }
    i = 2-bBulk;
    }
      nxDiv = iParentIdx-1;
    }else{
      nxDiv = i-2+bBulk;
    }else if( iParentIdx==i ){
      nxDiv = 0;
    if( iParentIdx==0 ){                 
    assert( bBulk==0 || bBulk==1 );
  }else{
    nxDiv = 0;
  if( i<2 ){
  i = pParent->nOverflow + pParent->nCell;
  */
  ** have already been removed.
  ** overflow cells in the parent page, since if any existed they will
  ** way, the remainder of the function does not have to deal with any
  ** This loop also drops the divider cells from the parent page. This
  **
  ** has NB or fewer children then all children of pParent are taken.  
  ** if there are fewer than NN siblings on the other side. If pParent
  ** either side of pPage. More siblings are taken from one side, however, 
  ** that divide the siblings. An attempt is made to find NN siblings on 
  /* Find the sibling pages to balance. Also locate the cells in pParent 

  assert( pParent->nFree>=0 );
  }
    return SQLITE_NOMEM_BKPT;
  if( !aOvflSpace ){

  assert( pParent->nOverflow==0 || pParent->aiOvfl[0]==iParentIdx );
  assert( pParent->nOverflow==0 || pParent->nOverflow==1 );
  */
  ** is called (indirectly) from sqlite3BtreeDelete().
  ** index iParentIdx. This scenario comes about when this function
  ** this overflow cell is present, it must be the cell with 
  /* At this point pParent may have at most one overflow cell. And if

  assert( sqlite3PagerIswriteable(pParent->pDbPage) );
  assert( sqlite3_mutex_held(pBt->mutex) );
  pBt = pParent->pBt;
  b.apCell = 0;
  b.nCell = 0;
  memset(abDone, 0, sizeof(abDone));

  CellArray b;                  /* Parsed information on cells being balanced */
  u16 aPgFlags[NB+2];          /* flags field of new pages before shuffling */
  Pgno aPgOrder[NB+2];         /* Copy of aPgno[] used for sorting pages */
  Pgno aPgno[NB+2];            /* Page numbers of new pages before shuffling */
  u8 abDone[NB+2];             /* True after i'th new page is populated */
  Pgno pgno;                   /* Temp var to store a page number in */
  u8 *aSpace1;                 /* Space for copies of dividers cells */
  int szNew[NB+2];             /* Combined size of cells placed on i-th page */
  int cntOld[NB+2];            /* Old index in b.apCell[] */
  int cntNew[NB+2];            /* Index in b.paCell[] of cell after i-th page */
  u8 *apDiv[NB-1];             /* Divider cells in pParent */
  u8 *pRight;                  /* Location in parent of right-sibling pointer */
  MemPage *apNew[NB+2];        /* pPage and up to NB siblings after balancing */
  MemPage *apOld[NB];          /* pPage and up to two siblings */
  int szScratch;               /* Size of scratch memory requested */
  int iOvflSpace = 0;          /* First unused byte of aOvflSpace[] */
  int iSpace1 = 0;             /* First unused byte of aSpace1[] */
  int pageFlags;               /* Value of pPage->aData[0] */
  int usableSpace;             /* Bytes in pPage beyond the header */
  int leafData;                /* True if pPage is a leaf of a LEAFDATA tree */
  u16 leafCorrection;          /* 4 if pPage is a leaf.  0 if not */
  int rc = SQLITE_OK;          /* The return code */
  int nxDiv;                   /* Next divider slot in pParent->aCell[] */
  int i, j, k;                 /* Loop counters */
  int nOld;                    /* Number of pages in apOld[] */
  int nNew = 0;                /* Number of pages in apNew[] */
  int nMaxCells = 0;           /* Allocated size of apCell, szCell, aFrom. */
  BtShared *pBt;               /* The whole database */
){
  int bBulk                       /* True if this call is part of a bulk load */
  int isRoot,                     /* True if pParent is a root-page */
  u8 *aOvflSpace,                 /* page-size bytes of space for parent ovfl */
  int iParentIdx,                 /* Index of "the page" in pParent */
  MemPage *pParent,               /* Parent page of siblings being balanced */
static int balance_nonroot(
*/
** SQLITE_NOMEM.
** If aOvflSpace is set to a null pointer, this function returns 
**
** enough for all overflow cells.
** of the page-size, the aOvflSpace[] buffer is guaranteed to be large
** size of a cell stored within an internal node is always less than 1/4
** a maximum of four divider cells into the parent page, and the maximum
** used to store the parent's overflow cells. Because this function inserts
** page (pParent) the parent page becomes overfull, this buffer is
** buffer big enough to hold one page. If while inserting cells into the parent
** The third argument to this function, aOvflSpace, is a pointer to a
**
** be rolled back.
** in a corrupted state. So if this routine fails, the database should
** If this routine fails for any reason, it might leave the database
**
** balancing routine to fix this problem (see the balance() routine). 
** happens, it is the responsibility of the caller to invoke the correct
** may cause the parent page to become overfull or underfull. If this
** inserted into or removed from the parent page (pParent). Doing so
** In the course of balancing the page and its siblings, cells may be
**
** to the page and its siblings fit into MemPage.aData[] before returning.
** if the page is overfull. This routine ensures that all cells allocated
** might not actually be stored in MemPage.aData[]. This can happen
** Note that when this routine is called, some of the cells on the page
**
** one or two in an effort to keep pages nearly full but not over full. 
** The number of siblings of the page might be increased or decreased by 
**
** participate in the balancing.
** is a root page or a child of a root page) then all available siblings
** has fewer than 2 siblings (something which can only happen if the page
** side if the page is the first or last child of its parent. If the page 
** page are used in the balancing, though both siblings might come from one
** same amount of free space. Usually a single sibling on either side of the
** (hereafter "the page") and up to 2 siblings so that all pages have about the
** This routine redistributes cells on the iParentIdx'th child of pParent
/*

}
  }
    }
      *pRC = setChildPtrmaps(pTo);
    if( ISAUTOVACUUM ){
    */
    ** for any b-tree or overflow pages that pTo now contains the pointers to.
    /* If this is an auto-vacuum database, update the pointer-map entries
  
    }
      return;
      *pRC = rc;
    if( rc!=SQLITE_OK ){
    if( rc==SQLITE_OK ) rc = btreeComputeFreeSpace(pTo);
    rc = btreeInitPage(pTo);
    pTo->isInit = 0;
    */
    ** page pFrom.
    ** fairly obscure circumstances, even though it is a copy of initialized 
    ** match the new data. The initialization of pTo can actually fail under
    /* Reinitialize page pTo so that the contents of the MemPage structure
  
    memcpy(&aTo[iToHdr], &aFrom[iFromHdr], pFrom->cellOffset + 2*pFrom->nCell);
    memcpy(&aTo[iData], &aFrom[iData], pBt->usableSize-iData);
    iData = get2byte(&aFrom[iFromHdr+5]);
    /* Copy the b-tree node content from page pFrom to page pTo. */
  
    assert( get2byte(&aFrom[iFromHdr+5]) <= (int)pBt->usableSize );
    assert( pFrom->nFree>=iToHdr );
    assert( pFrom->isInit );
  
  
    int iData;
    int rc;
    int const iToHdr = ((pTo->pgno==1) ? 100 : 0);
    int const iFromHdr = pFrom->hdrOffset;
    u8 * const aTo = pTo->aData;
    u8 * const aFrom = pFrom->aData;
    BtShared * const pBt = pFrom->pBt;
  if( (*pRC)==SQLITE_OK ){
static void copyNodeContent(MemPage *pFrom, MemPage *pTo, int *pRC){
*/
** which are called often under normal circumstances.
** the balance_shallower() and balance_deeper() procedures, neither of
** The performance of this function is not critical. It is only used by 
**
** Before returning, page pTo is reinitialized using btreeInitPage().
**
** MemPage.apOvfl[] array), they are not copied to pTo. 
** If pFrom is currently carrying any overflow cells (entries in the
**
** map entries are also updated so that the parent page is page pTo.
** any cells with overflow page pointers, then the corresponding pointer
** parent page stored in the pointer map is page pTo. If pFrom contained
** the pointer-map entries for each child page are updated so that the
** on page pFrom to page pTo. If page pFrom was not a leaf page, then
** This function is used to copy the contents of the b-tree node stored 
/*

#endif
}
  return 1;
  }
    }
      assert( n==pPage->pgno && e==PTRMAP_BTREE );
      ptrmapGet(pBt, child, &e, &n);
      Pgno child = get4byte(&pPage->aData[pPage->hdrOffset+8]);
    if( !pPage->leaf ){
    }
      }
        assert( n==pPage->pgno && e==PTRMAP_BTREE );
        ptrmapGet(pBt, child, &e, &n);
        Pgno child = get4byte(z);
      if( !pPage->leaf ){
      }
        assert( n==pPage->pgno && e==PTRMAP_OVERFLOW1 );
        ptrmapGet(pBt, ovfl, &e, &n);
        Pgno ovfl = get4byte(&z[info.nSize-4]);
      if( info.nLocal<info.nPayload ){
      pPage->xParseCell(pPage, z, &info);
      z = findCell(pPage, j);
     
      u8 *z;
      CellInfo info;
    for(j=0; j<pPage->nCell; j++){

    assert( pPage->isInit );
    BtShared *pBt = pPage->pBt;
    MemPage *pPage = apPage[i];
    u8 e;
    Pgno n;
  for(i=0; i<nPage; i++){
  int i, j;
static int ptrmapCheckPages(MemPage **apPage, int nPage){
*/
** for setting pointer-map entries.
** it is sometimes activated temporarily while debugging code responsible 
** This function does not contribute anything to the operation of SQLite.
/*
#if 0

#endif /* SQLITE_OMIT_QUICKBALANCE */
}
  return rc;

  }
    releasePage(pNew);
    /* Release the reference to the new page. */
  
    put4byte(&pParent->aData[pParent->hdrOffset+8], pgnoNew);
    /* Set the right-child pointer of pParent to point to the new page. */

    }
                   0, pPage->pgno, &rc);
      insertCell(pParent, pParent->nCell, pSpace, (int)(pOut-pSpace),
    if( rc==SQLITE_OK ){
    /* Insert the new divider cell into pParent. */

    while( ((*(pOut++) = *(pCell++))&0x80) && pCell<pStop );
    pStop = &pCell[9];
    while( (*(pCell++)&0x80) && pCell<pStop );
    pStop = &pCell[9];
    pCell = findCell(pPage, pPage->nCell-1);
    */
    ** cell on pPage into the pSpace buffer.
    ** field. The second while(...) loop copies the key value from the
    ** The first of the while(...) loops below skips over the record-length
    ** and the key value (a variable length integer, may have any value).
    ** record-length (a variable length integer at most 32-bits in size)
    ** cell on pPage. The first two fields of this cell are the 
    ** To find the largest key value on pPage, first find the right-most 
    **
    ** largest key on pPage).
    ** a variable length key value (which must be the same value as the
    ** consists of a 4-byte page number (the page number of pPage) and
    /* Create a divider cell to insert into pParent. The divider cell
  
    }
      }
        ptrmapPutOvflPtr(pNew, pNew, pCell, &rc);
      if( szCell>pNew->minLocal ){
      ptrmapPut(pBt, pgnoNew, PTRMAP_BTREE, pParent->pgno, &rc);
    if( ISAUTOVACUUM ){
    */
    ** rollback, undoing any changes made to the parent page.
    ** be marked as dirty. Returning an error code will cause a
    ** That is Ok, at this point the parent page is guaranteed to
    ** of the parent page are still manipulated by thh code below.
    ** operations fails, the return code is set, but the contents
    ** cell on the page to an overflow page. If either of these
    ** with entries for the new page, and any pointer from the 
    /* If this is an auto-vacuum database, update the pointer map

    pNew->nFree = pBt->usableSize - pNew->cellOffset - 2 - szCell;
    }
      return rc;
      releasePage(pNew);
    if( NEVER(rc) ){
    rc = rebuildPage(&b, 0, 1, pNew);
    b.ixNx[0] = 2;
    b.apEnd[0] = pPage->aDataEnd;
    b.szCell = &szCell;
    b.apCell = &pCell;
    b.pRef = pPage;
    b.nCell = 1;
    zeroPage(pNew, PTF_INTKEY|PTF_LEAFDATA|PTF_LEAF);
    assert( CORRUPT_DB || pPage->aData[0]==(PTF_INTKEY|PTF_LEAFDATA|PTF_LEAF) );
    assert( sqlite3PagerIswriteable(pNew->pDbPage) );

    CellArray b;
    u8 *pStop;
    u16 szCell = pPage->xCellSize(pPage, pCell);
    u8 *pCell = pPage->apOvfl[0];
    u8 *pOut = &pSpace[4];

  if( rc==SQLITE_OK ){

  rc = allocateBtreePage(pBt, &pNew, &pgnoNew, 0, 0);
  */
  ** may be inserted. If both these operations are successful, proceed.
  ** pPage. Make the parent page writable, so that the new divider cell
  /* Allocate a new page. This page will become the right-sibling of 

  assert( pParent->nFree>=0 );
  assert( pPage->nFree>=0 );
  if( pPage->nCell==0 ) return SQLITE_CORRUPT_BKPT;  /* dbfuzz001.test */
  
  assert( pPage->nOverflow==1 );
  assert( sqlite3PagerIswriteable(pParent->pDbPage) );
  assert( sqlite3_mutex_held(pPage->pBt->mutex) );

  Pgno pgnoNew;                        /* Page number of pNew */
  int rc;                              /* Return Code */
  MemPage *pNew;                       /* Newly allocated page */
  BtShared *const pBt = pPage->pBt;    /* B-Tree Database */
static int balance_quick(MemPage *pParent, MemPage *pPage, u8 *pSpace){
*/
** least 13 bytes in size.
** words, at most 13 bytes. Hence the pSpace buffer must be at
** byte page number followed by a variable length integer. In other
** cell that will be inserted into pParent. Such a cell consists of a 4
** The pSpace buffer is used to store a temporary copy of the divider
**
** which is also the right-most entry on the page.
** pParent is its parent.  pPage must have a single overflow entry
** pPage is the leaf page which is the right-most page in the tree.
**
** fill up.  On average.
** at the end soon afterwards so the nearly empty page will quickly
** unbalanced.  But odds are that we will be inserting new entries
** that page.  This leaves the right side of the tree somewhat
** a new page to the right-hand side and put the one new entry in
** Instead of trying to balance the 3 right-most leaf pages, just add
**
** entry in the tree.
** tree, in other words, when the new entry will become the largest
** a new entry is being inserted on the extreme right-end of the
** This version of balance() handles the common special case where
/*
#ifndef SQLITE_OMIT_QUICKBALANCE


}
  return rebuildPage(pCArray, iNew, nNew, pPg);
  populateCellCache(pCArray, iNew, nNew);
  /* Unable to edit this page. Rebuild it from scratch instead. */
 editpage_fail:
  return SQLITE_OK;

#endif
  }
            pCArray->pRef->xCellSize(pCArray->pRef, pCArray->apCell[i+iNew])) );
    assert( 0==memcmp(pCell, &aData[iOff],
    }
      pCell = &pTmp[pCell - aData];
    if( SQLITE_WITHIN(pCell, aData, &aData[pPg->pBt->usableSize]) ){
    int iOff = get2byteAligned(&pPg->aCellIdx[i*2]);
    u8 *pCell = pCArray->apCell[i+iNew];
  for(i=0; i<nNew && !CORRUPT_DB; i++){
#ifdef SQLITE_DEBUG

  put2byte(&aData[hdr+5], pData - aData);
  put2byte(&aData[hdr+3], pPg->nCell);

  pPg->nOverflow = 0;
  pPg->nCell = nNew;

  ) ) goto editpage_fail;
        iNew+nCell, nNew-nCell, pCArray
        pPg, pBegin, &pData, pCellptr,
  if( pageInsertArray(
  pCellptr = &pPg->aCellIdx[nCell*2];
  assert( nCell>=0 );
  /* Append cells to the end of the page */

  }
    }
      ) ) goto editpage_fail;
            iCell+iNew, 1, pCArray
            pPg, pBegin, &pData, pCellptr,
      if( pageInsertArray(
      cachedCellSize(pCArray, iCell+iNew);
      nCell++;
      }
        memmove(&pCellptr[2], pCellptr, (nCell - iCell) * 2);
      if( nCell>iCell ){
      pCellptr = &pPg->aCellIdx[iCell * 2];
    if( iCell>=0 && iCell<nNew ){
    int iCell = (iOld + pPg->aiOvfl[i]) - iNew;
  for(i=0; i<pPg->nOverflow; i++){
  /* Add any overflow cells */

  }
    nCell += nAdd;
    ) ) goto editpage_fail;
          iNew, nAdd, pCArray
          pPg, pBegin, &pData, pCellptr,
    if( pageInsertArray(
    memmove(&pCellptr[nAdd*2], pCellptr, nCell*2);
    pCellptr = pPg->aCellIdx;
    assert( nAdd>=0 );
    assert( (iOld-iNew)<nNew || nCell==0 || CORRUPT_DB );
    int nAdd = MIN(nNew,iOld-iNew);
  if( iNew<iOld ){
  /* Add cells to the start of the page */

  if( pData<pBegin ) goto editpage_fail;
  pData = &aData[get2byteNotZero(&aData[hdr+5])];

  }
    nCell -= nTail;
    assert( nCell>=nTail );
    int nTail = pageFreeArray(pPg, iNewEnd, iOldEnd - iNewEnd, pCArray);
  if( iNewEnd < iOldEnd ){
  }
    nCell -= nShift;
    memmove(pPg->aCellIdx, &pPg->aCellIdx[nShift*2], nCell*2);
    if( nShift>nCell ) return SQLITE_CORRUPT_BKPT;
    int nShift = pageFreeArray(pPg, iOld, iNew-iOld, pCArray);
  if( iOld<iNew ){
  assert( nCell>=0 );
  /* Remove cells from the start and end of the page */

#endif
  memcpy(pTmp, aData, pPg->pBt->usableSize);
  u8 *pTmp = sqlite3PagerTempSpace(pPg->pBt->pPager);
#ifdef SQLITE_DEBUG

  int iNewEnd = iNew + nNew;
  int iOldEnd = iOld + pPg->nCell + pPg->nOverflow;
  int i;
  u8 *pCellptr;
  u8 *pData;
  int nCell = pPg->nCell;       /* Cells stored on pPg */
  u8 *pBegin = &pPg->aCellIdx[nNew * 2];
  const int hdr = pPg->hdrOffset;
  u8 * const aData = pPg->aData;
){
  CellArray *pCArray              /* Array of cells and sizes */
  int nNew,                       /* Final number of cells on page */
  int iNew,                       /* Index of new first cell on page */
  int iOld,                       /* Index of first cell currently on page */
  MemPage *pPg,                   /* Edit this page */
static int editPage(
*/
** responsibility of the caller to set it correctly.
** The pPg->nFree field is invalid when this function returns. It is the
**
** the correct cells after being balanced.
** This routine makes the necessary adjustments to pPg so that it contains
**
** starting at apCell[iNew].
** pCArray->apCell[iOld].  After balancing, this page should hold nNew cells
** balanced.  The current page, pPg, has pPg->nCell cells starting with
** pCArray contains pointers to and sizes of all cells in the page being
/*

}
  return nRet;
  }
    freeSpace(pPg, (u16)(pFree - aData), szFree);
    assert( pFree>aData && (pFree - aData)<65536 );
  if( pFree ){
  }
    }
      nRet++;
      }
        szFree += sz;
        pFree = pCell;
      }else{
        if( pFree+sz>pEnd ) return 0;
        szFree = sz;
        pFree = pCell;
        }
          freeSpace(pPg, (u16)(pFree - aData), szFree);
          assert( pFree>aData && (pFree - aData)<65536 );
        if( pFree ){
      if( pFree!=(pCell + sz) ){
      sz = pCArray->szCell[i];  assert( sz>0 );
      ** cells need freeing */
      ** are to be freed have already been computing while deciding which
      /* No need to use cachedCellSize() here.  The sizes of all cells that
      int sz;
    if( SQLITE_WITHIN(pCell, pStart, pEnd) ){
    u8 *pCell = pCArray->apCell[i];
  for(i=iFirst; i<iEnd; i++){

  int szFree = 0;
  u8 *pFree = 0;
  int iEnd = iFirst + nCell;
  int i;
  int nRet = 0;
  u8 * const pStart = &aData[pPg->hdrOffset + 8 + pPg->childPtrSize];
  u8 * const pEnd = &aData[pPg->pBt->usableSize];
  u8 * const aData = pPg->aData;
){
  CellArray *pCArray              /* Array of cells */
  int nCell,                      /* Cells to delete */
  int iFirst,                     /* First cell to delete */
  MemPage *pPg,                   /* Page to edit */
static int pageFreeArray(
*/
** This function returns the total number of cells added to the free-list.
**
** The cell-pointers and other fields of the page are not updated.
** that is currently stored within the body of pPg to the pPg free-list.
** This function adds the space associated with each cell in the array
**
** The pCArray object contains pointers to b-tree cells and their sizes.
/*

}
  return 0;
  *ppData = pData;
  }
    }
      pEnd = pCArray->apEnd[k];
      k++;
    if( pCArray->ixNx[k]<=i ){
    if( i>=iEnd ) break;
    i++;
    pCellptr += 2;
    put2byte(pCellptr, (pSlot - aData));
    memmove(pSlot, pCArray->apCell[i], sz);
    }
      return 1;
      (void)SQLITE_CORRUPT_BKPT;
      assert( CORRUPT_DB );
    ){
     && (uptr)(pCArray->apCell[i])<(uptr)pEnd
    if( (uptr)(pCArray->apCell[i]+sz)>(uptr)pEnd
         || CORRUPT_DB );
         || pSlot>=(pCArray->apCell[i]+sz)
    assert( (pSlot+sz)<=pCArray->apCell[i]
    ** since memcpy() sends SIGABORT with overlapping buffers on OpenBSD */
    ** database.  But they might for a corrupt database.  Hence use memmove()
    /* pSlot and pCArray->apCell[i] will never overlap on a well-formed
    }
      pSlot = pData;
      pData -= sz;
      if( (pData - pBegin)<sz ) return 1;
    if( (aData[1]==0 && aData[2]==0) || (pSlot = pageFindSlot(pPg,sz,&rc))==0 ){
    sz = pCArray->szCell[i];
    assert( pCArray->szCell[i]!=0 );
    u8 *pSlot;
    int sz, rc;
  while( 1 /*Exit by break*/ ){
  pEnd = pCArray->apEnd[k];
  for(k=0; pCArray->ixNx[k]<=i && ALWAYS(k<NB*2); k++){}
  if( iEnd<=iFirst ) return 0;
  assert( CORRUPT_DB || pPg->hdrOffset==0 );    /* Never called on page 1 */
  u8 *pEnd;                       /* Maximum extent of cell data */
  int k;                          /* Current slot in pCArray->apEnd[] */
  int iEnd = iFirst + nCell;      /* End of loop. One past last cell to ins */
  u8 *pData = *ppData;            /* Content area.  A subset of aData[] */
  u8 *aData = pPg->aData;         /* Complete page */
  int i = iFirst;                 /* Loop counter - cell index to insert */
){
  CellArray *pCArray              /* Array of cells */
  int nCell,                      /* Number of cells to add to pPg */
  int iFirst,                     /* Index of first cell to add */
  u8 *pCellptr,                   /* Pointer to cell-pointer area */
  u8 **ppData,                    /* IN/OUT: Page content-area pointer */
  u8 *pBegin,                     /* End of cell-pointer array */
  MemPage *pPg,                   /* Page to add cells to */
static int pageInsertArray(
*/
** cells in apCell[], then the cells do not fit and non-zero is returned.
** area must be extended to before this point in order to accomodate all
** all cells - not just those inserted by the current call). If the content
** end of the space required by this page for the cell-pointer area (for
** Finally, argument pBegin points to the byte immediately following the
**
** before returning.
** *ppData is updated to point to the new start of the content area
** content area on page pPg. If the size of the content area is extended,
** When this function is called, *ppData points to the start of the 
**
** that it is safe to overwrite this part of the cell-pointer array.
** cell in the array. It is the responsibility of the caller to ensure
** page body, a 16-bit offset is written to pCellptr. And so on, for each
** (part of page pPg) to populate. After cell apCell[0] is written to the
** Argument pCellptr points to the first entry in the cell-pointer array
**
** successfully, zero is returned.
** will fit), non-zero is returned. Otherwise, if the cells are added
** If it cannot (because the page needs to be defragmented before the cells
** This function attempts to add the cells stored in the array to page pPg.
** The pCArray objects contains pointers to b-tree cells and the cell sizes.
/*

}
  return SQLITE_OK;
  aData[hdr+7] = 0x00;
  put2byte(&aData[hdr+5], pData - aData);
  put2byte(&aData[hdr+3], pPg->nCell);
  put2byte(&aData[hdr+1], 0);

  pPg->nOverflow = 0;
  pPg->nCell = nCell;
  /* The pPg->nFree field is now set incorrectly. The caller will fix it. */

  }
    }
      pSrcEnd = pCArray->apEnd[k];
      k++;
    if( pCArray->ixNx[k]<=i ){
    if( i>=iEnd ) break;
    i++;
    testcase( sz!=pPg->xCellSize(pPg,pCell) )
    assert( sz==pPg->xCellSize(pPg, pCell) || CORRUPT_DB );
    memcpy(pData, pCell, sz);
    if( pData < pCellptr ) return SQLITE_CORRUPT_BKPT;
    pCellptr += 2;
    put2byte(pCellptr, (pData - aData));
    pData -= sz;

    }
      return SQLITE_CORRUPT_BKPT;
    ){
           && (uptr)(pCell)<(uptr)pSrcEnd
    }else if( (uptr)(pCell+sz)>(uptr)pSrcEnd
      pCell = &pTmp[pCell - aData];
      if( ((uptr)(pCell+sz))>(uptr)pEnd ) return SQLITE_CORRUPT_BKPT;
    if( SQLITE_WITHIN(pCell,aData,pEnd) ){
    assert( sz>0 );
    u16 sz = pCArray->szCell[i];
    u8 *pCell = pCArray->apCell[i];
  while( 1/*exit by break*/ ){
  pData = pEnd;

  pSrcEnd = pCArray->apEnd[k];
  for(k=0; pCArray->ixNx[k]<=i && ALWAYS(k<NB*2); k++){}

  memcpy(&pTmp[j], &aData[j], usableSize - j);
  if( NEVER(j>(u32)usableSize) ){ j = 0; }
  j = get2byte(&aData[hdr+5]);
  assert( i<iEnd );

  u8 *pSrcEnd;                    /* Current pCArray->apEnd[k] value */
  int k;                          /* Current slot in pCArray->apEnd[] */
  u8 *pData;
  u8 *pTmp = sqlite3PagerTempSpace(pPg->pBt->pPager);
  u8 *pCellptr = pPg->aCellIdx;
  int iEnd = i+nCell;             /* Loop terminator */
  u32 j;                          /* Start of cell content area */
  int i = iFirst;                 /* Which cell to copy from pCArray*/
  u8 * const pEnd = &aData[usableSize];
  const int usableSize = pPg->pBt->usableSize;
  u8 * const aData = pPg->aData;           /* Pointer to data for pPg */
  const int hdr = pPg->hdrOffset;          /* Offset of header on pPg */
){
  MemPage *pPg                    /* The page to be reconstructed */
  int nCell,                      /* Final number of cells on page */
  int iFirst,                     /* First cell in pCArray to use */
  CellArray *pCArray,             /* Content to be added to page pPg */
static int rebuildPage(
*/
** responsibility of the caller to set it correctly.
** The MemPage.nFree field is invalidated by this function. It is the 
**
** such cells before overwriting the page data.
** function works around problems caused by this by making a copy of any 
** Some of the cells in apCell[] may currently be stored in pPg. This
**
** array.
** replaces the current contents of page pPg with the contents of the cell
** szCell[] array contains the size in bytes of each cell. This function
** Array apCell[] contains pointers to nCell b-tree page cells. The 
/*

}
  return computeCellSize(p, N);
  if( p->szCell[N] ) return p->szCell[N];
  assert( N>=0 && N<p->nCell );
static u16 cachedCellSize(CellArray *p, int N){
}
  return p->szCell[N];
  p->szCell[N] = p->pRef->xCellSize(p->pRef, p->apCell[N]);
  assert( p->szCell[N]==0 );
  assert( N>=0 && N<p->nCell );
static SQLITE_NOINLINE u16 computeCellSize(CellArray *p, int N){
*/
** Return the size of the Nth element of the cell array
/*

}
  }
    N--;
    idx++;
    }
              p->szCell[idx]==p->pRef->xCellSize(p->pRef, p->apCell[idx]) );
      assert( CORRUPT_DB ||
    }else{
      p->szCell[idx] = p->pRef->xCellSize(p->pRef, p->apCell[idx]);
    if( p->szCell[idx]==0 ){
    assert( p->apCell[idx]!=0 );
  while( N>0 ){
  assert( idx>=0 && idx+N<=p->nCell );
static void populateCellCache(CellArray *p, int idx, int N){
*/
** computed.
** Make sure the cell sizes at idx, idx+1, ..., idx+N-1 have been
/*

};
  int ixNx[NB*2];         /* Index of at which we move to the next apEnd[] */
  u8 *apEnd[NB*2];        /* MemPage.aDataEnd values */
  u16 *szCell;            /* Local size of all cells in apCell[] */
  u8 **apCell;            /* All cells begin balanced */
  MemPage *pRef;          /* Reference page */
  int nCell;              /* Number of cells in apCell[] */
struct CellArray {
typedef struct CellArray CellArray;
*/
** be larger than the previous
** entries, shift down.  The end result is that each ixNx[] entry should
** cases, ixNx[] entries with higher indexes, and the corresponding apEnd[]
** Sometimes when deleting, a child page can have zero cells.  In those
**
**    ixNx[2] = Total number of cells.
**    ixNx[1] = Number of cells in Child-1 and Child-2.
**    ixNx[0] = Number of cells in Child-1.
**
** are used and they point to the leaf pages only, and the ixNx value are:
** For a table-btree, the concept is similar, except only apEnd[0]..apEnd[2]
**
**    ixNx[4] = Total number of cells.
**    ixNx[3] = Number of cells in Child-1 and Child-2 + both divider cells
**    ixNx[2] = Number of cells in Child-1 and Child-2 + 1 for 1st divider.
**    ixNx[1] = Number of cells in Child-1 plus 1 for first divider.
**    ixNx[0] = Number of cells in Child-1.
**
** each of these 5 stages, and all stages to the left.  Hence:
** respectively. The ixNx[] array holds the number of cells contained in
** for Child-1, the Parent, Child-2, the Parent (again), and Child-3,
** For an index btree, the apEnd[] array holds pointer to the end of page
**
** content exists only in leaves and there are no divider cells.
** For a table-btree (with rowids) the items 2 and 4 are empty because
**
**       5.  All cells from Child-3 in order
**       4.  The second divider cell from Parent
**       3.  All cells from Child-2 in order
**       2.  The first divider cell from Parent
**       1.  All cells from Child-1 in order
**
** The order of cells is in the array is for an index btree is:
**
**  ---------   ---------   ---------
**  |Child-1|   |Child-2|   |Child-3|
**  ---------   ---------   ---------
**           /      |      \
**            /     |     \
**             -----------
**             | Parent  |
**             -----------
**
** The cells come from as many as four different pages:
**
** A szCell[] of zero means the size of that cell has not yet been computed.
**
** to content area of the pages.
** to overflow cells.  In other words, some apCel[] pointers might not point
** cell and the size of each cell.  Some of the apCell[] pointers might refer
** apCell[] and szCell[] hold, respectively, pointers to the start of each
**
** which should be common to all pages that contribute cells to this array.
** used to access information such as MemPage.intKey and MemPage.pBt->pageSize
** pRef is a pointer to one of the pages that contributes cells.  This is
**
** page plus up to three child pages.  There are a total of nCell cells.
** The cells in this array are the divider cell or cells from the pParent
**
** consecutive sequence of cells that might be held on multiple pages.
** A CellArray object contains a cache of pointers and sizes for a
/*

#define NB 3             /* (NN*2+1): Total pages involved in the balance */
#define NN 1             /* Number of neighbors on either side of pPage */
*/
** we have never tested any other value.
** But that is unlikely.  NB has been 3 since the inception of SQLite and
** tunable - as if you could change them and recompile and it would all work.
** (Later:) The description above makes it seem as if these values are
**
** The value of NN appears to give the best results overall.
** in exchange for a larger degradation in INSERT and UPDATE performance.
** (to 2 or 3) gives a modest improvement in SELECT and DELETE performance
** The minimum value of NN is 1 (of course).  Increasing NN above 1
**
** NN neighbors on either side.
** total number of pages that participate, including the target page and
** of the page that participate in the balancing operation.  NB is the
** in a balancing operation.  NN is the number of neighbors on either side
** The following parameters determine how many adjacent pages get involved
/*

}
  }
#endif
    }
      ptrmapPutOvflPtr(pPage, pPage, pCell, pRC);
      */
      ** the entry for the overflow page into the pointer map.
      /* The cell may contain a pointer to an overflow page. If so, write
    if( pPage->pBt->autoVacuum ){
#ifndef SQLITE_OMIT_AUTOVACUUM
    assert( get2byte(&data[pPage->hdrOffset+3])==pPage->nCell || CORRUPT_DB );
    if( (++data[pPage->hdrOffset+4])==0 ) data[pPage->hdrOffset+3]++;
    /* increment the cell count */
    pPage->nCell++;
    put2byte(pIns, idx);
    memmove(pIns+2, pIns, 2*(pPage->nCell - i));
    pIns = pPage->aCellIdx + i*2;
    }
      memcpy(&data[idx], pCell, sz);
    }else{
      put4byte(&data[idx], iChild);
      memcpy(&data[idx+4], pCell+4, sz-4);
      ** reading the first 4 bytes */
      ** the source page.  Make sure this does not cause problems by not
      ** as many as 4 bytes in front of the start of the aData buffer for
      ** a btree page has a value of 3 or less, the pCell value might point
      /* In a corrupt database where an entry in the cell index section of
    if( iChild ){
    pPage->nFree -= (u16)(2 + sz);
    assert( idx+sz <= (int)pPage->pBt->usableSize );
    assert( idx >= pPage->cellOffset+2*pPage->nCell+2 || CORRUPT_DB );
    assert( idx >= 0 );
    ** if it returns successfully */
    /* The allocateSpace() routine guarantees the following properties
    if( rc ){ *pRC = rc; return; }
    rc = allocateSpace(pPage, sz, &idx);
    assert( &data[pPage->cellOffset]==pPage->aCellIdx );
    data = pPage->aData;
    assert( sqlite3PagerIswriteable(pPage->pDbPage) );
    }
      return;
      *pRC = rc;
    if( rc!=SQLITE_OK ){
    int rc = sqlite3PagerWrite(pPage->pDbPage);
  }else{
    assert( j==0 || i==pPage->aiOvfl[j-1]+1 );   /* Overflows are sequential */
    assert( j==0 || pPage->aiOvfl[j-1]<(u16)i ); /* Overflows in sorted order */
    */
    ** balancing, and the dividers are adjacent and sorted.
    ** only occur when inserting divider cells into the parent page during
    ** sorted order.  This invariants arise because multiple overflows can
    /* When multiple overflows occur, they are always sequential and in

    pPage->aiOvfl[j] = (u16)i;
    pPage->apOvfl[j] = pCell;
    assert( j < ArraySize(pPage->apOvfl)-1 );
    ** slots but 4 are allocated, just to be safe. */
    ** as a contingency.  In other words, never need more than 3 overflow
    /* Comparison against ArraySize-1 since we hold back one extra slot
    j = pPage->nOverflow++;
    }
      put4byte(pCell, iChild);
    if( iChild ){
    }
      pCell = pTemp;
      memcpy(pTemp, pCell, sz);
    if( pTemp ){
  if( pPage->nOverflow || sz+2>pPage->nFree ){
  assert( pPage->nFree>=0 );
  assert( sz==pPage->xCellSize(pPage, pCell) || CORRUPT_DB );
  assert( sqlite3_mutex_held(pPage->pBt->mutex) );
  assert( ArraySize(pPage->apOvfl)==ArraySize(pPage->aiOvfl) );
  assert( pPage->nOverflow<=ArraySize(pPage->apOvfl) );
  assert( pPage->nCell<=MX_CELL(pPage->pBt) || CORRUPT_DB );
  assert( MX_CELL(pPage->pBt)<=10921 );
  assert( i>=0 && i<=pPage->nCell+pPage->nOverflow );
  assert( *pRC==SQLITE_OK );

  u8 *pIns;         /* The point in pPage->aCellIdx[] where no cell inserted */
  u8 *data;         /* The content of the whole page */
  int j;            /* Loop counter */
  int idx = 0;      /* Where to write new cell content in data[] */
){
  int *pRC          /* Read and write return code from here */
  Pgno iChild,      /* If non-zero, replace first 4 bytes with this value */
  u8 *pTemp,        /* Temp storage space for pCell, if needed */
  int sz,           /* Bytes of content in pCell */
  u8 *pCell,        /* Content of the new cell */
  int i,            /* New cell becomes the i-th cell of the page */
  MemPage *pPage,   /* Page into which we are copying */
static void insertCell(
*/
** *pRC must be SQLITE_OK when this routine is called.
**
** pPage->nOverflow is incremented.
** Allocating a new entry in pPage->aCell[] implies that 
** in pTemp or the original pCell) and also record its index. 
** in pPage->apOvfl[] and make it point to the cell content (either
** pTemp is not null.  Regardless of pTemp, allocate a new entry
** will not fit, then make a copy of the cell content into pTemp if
** If the cell content will fit on the page, then put it there.  If it
**
** content of the cell.
** Insert a new cell on pPage at cell index "i".  pCell points to the
/*

}
  }
    pPage->nFree += 2;
    put2byte(&data[hdr+3], pPage->nCell);
    memmove(ptr, ptr+2, 2*(pPage->nCell - idx));
  }else{
                       - pPage->childPtrSize - 8;
    pPage->nFree = pPage->pBt->usableSize - pPage->hdrOffset
    put2byte(&data[hdr+5], pPage->pBt->usableSize);
    data[hdr+7] = 0;
    memset(&data[hdr+1], 0, 4);
  if( pPage->nCell==0 ){
  pPage->nCell--;
  }
    return;
    *pRC = rc;
  if( rc ){
  rc = freeSpace(pPage, pc, sz);
  }
    return;
    *pRC = SQLITE_CORRUPT_BKPT;
  if( pc+sz > pPage->pBt->usableSize ){
  testcase( pc+sz==pPage->pBt->usableSize );
  testcase( pc==get2byte(&data[hdr+5]) );
  hdr = pPage->hdrOffset;
  pc = get2byte(ptr);
  ptr = &pPage->aCellIdx[2*idx];
  data = pPage->aData;
  assert( pPage->nFree>=0 );
  assert( sqlite3_mutex_held(pPage->pBt->mutex) );
  assert( sqlite3PagerIswriteable(pPage->pDbPage) );
  assert( CORRUPT_DB || sz==cellSize(pPage, idx) );
  assert( idx>=0 && idx<pPage->nCell );
  if( *pRC ) return;

  int hdr;        /* Beginning of the header.  0 most pages.  100 page 1 */
  int rc;         /* The return code */
  u8 *ptr;        /* Used to move bytes around within data[] */
  u8 *data;       /* pPage->aData */
  u32 pc;         /* Offset to cell content of cell being deleted */
static void dropCell(MemPage *pPage, int idx, int sz, int *pRC){
*/
** "sz" must be the number of bytes in the cell.
**
** removes the reference to the cell from pPage.
** the cell content has been copied someplace else.  This routine just
** The cell content is not freed or deallocated.  It is assumed that
** Remove the i-th cell from pPage.  This routine effects pPage only.
/*

}
  return SQLITE_OK;
  releasePage(pToRelease);
  }
    }
      spaceLeft = pBt->usableSize - 4;
      pPayload = &pOvfl->aData[4];
      put4byte(pPrior, 0);
      pPrior = pOvfl->aData;
      pToRelease = pOvfl;
      releasePage(pToRelease);
      put4byte(pPrior, pgnoOvfl);

            || sqlite3PagerIswriteable(pPage->pDbPage) );
      assert( pPrior<pPage->aData || pPrior>=&pPage->aData[pBt->pageSize]
      ** is still writeable */
      /* If pPrior is part of the data area of pPage, then make sure pPage

      assert( pToRelease==0 || sqlite3PagerIswriteable(pToRelease->pDbPage) );
      ** of pToRelease.  Make sure pToRelease is still writeable. */
      /* If pToRelease is not zero than pPrior points into the data area

      }
        return rc;
        releasePage(pToRelease);
      if( rc ){
#endif
      }
        }
          releasePage(pOvfl);
        if( rc ){
        ptrmapPut(pBt, pgnoOvfl, eType, pgnoPtrmap, &rc);
        u8 eType = (pgnoPtrmap?PTRMAP_OVERFLOW2:PTRMAP_OVERFLOW1);
      if( pBt->autoVacuum && rc==SQLITE_OK ){
      */
      ** wrong pages from the database.
      ** may misinterpret the uninitialized values and delete the
      ** then the optimistic overflow chain processing in clearCell()
      ** to the pointer-map. If we write nothing to this pointer-map slot,
      ** If this is the first overflow page, then write a partial entry 
      **
      ** for that page now. 
      ** overflow page is being allocated, add an entry to the pointer-map
      /* If the database supports auto-vacuum, and the second or subsequent
#ifndef SQLITE_OMIT_AUTOVACUUM
      rc = allocateBtreePage(pBt, &pOvfl, &pgnoOvfl, pgnoOvfl, 0);
#endif
      }
        );
          PTRMAP_ISPAGE(pBt, pgnoOvfl) || pgnoOvfl==PENDING_BYTE_PAGE(pBt) 
        } while( 
          pgnoOvfl++;
        do{
      if( pBt->autoVacuum ){
      Pgno pgnoPtrmap = pgnoOvfl; /* Overflow page pointer-map entry page */
#ifndef SQLITE_OMIT_AUTOVACUUM
      MemPage *pOvfl = 0;
    if( spaceLeft==0 ){
    spaceLeft -= n;
    nSrc -= n;
    pSrc += n;
    pPayload += n;
    if( nPayload<=0 ) break;
    nPayload -= n;
    }
      memset(pPayload, 0, n);
    }else{
      memcpy(pPayload, pSrc, n);
      n = nSrc;
    }else if( nSrc>0 ){
      memcpy(pPayload, pSrc, n);
    if( nSrc>=n ){

            || sqlite3PagerIswriteable(pPage->pDbPage) );
    assert( pPayload<pPage->aData || pPayload>=&pPage->aData[pBt->pageSize]
    ** is still writeable */
    /* If pPayload is part of the data area of pPage, then make sure pPage

    assert( pToRelease==0 || sqlite3PagerIswriteable(pToRelease->pDbPage) );
    ** of pToRelease.  Make sure pToRelease is still writeable. */
    /* If pToRelease is not zero than pPayload points into the data area

    if( n>spaceLeft ) n = spaceLeft;
    n = nPayload;
  while( 1 ){
  /* Write the payload into the local Cell and any extra into overflow pages */

#endif
  }
    assert( spaceLeft == info.nLocal );
    assert( *pnSize == info.nSize );
    assert( info.nKey==pX->nKey );
    assert( nHeader==(int)(info.pPayload - pCell) );
    pPage->xParseCell(pPage, pCell, &info);
    CellInfo info;
  {
#ifdef SQLITE_DEBUG
  */
  ** were computed correctly.
  ** Use a call to btreeParseCellPtr() to verify that the values above
  **
  **   pPrior             Where to write the pgno of the first overflow page
  **   *pnSize            Size of the local cell (not counting overflow pages)
  **                      that means content must spill into overflow pages.
  **   spaceLeft          Space available at pPayload.  If nPayload>spaceLeft,
  **   pPayload           Begin writing payload here
  **   nPayload           Total payload size in bytes
  **
  /* At this point variables should be set as follows:

  pBt = pPage->pBt;
  pgnoOvfl = 0;
  pToRelease = 0;
  pPrior = &pCell[nHeader+n];
  *pnSize = n + nHeader + 4;
  spaceLeft = n;
  if( n > pPage->maxLocal ) n = mn;
  testcase( n==pPage->maxLocal+1 );
  testcase( n==pPage->maxLocal );
  n = mn + (nPayload - mn) % (pPage->pBt->usableSize - 4);
  mn = pPage->minLocal;
  */
  ** to spill onto overflow pages.
  /* If we reach this point, it means that some of the content will need

  }
    return SQLITE_OK;
    memset(pPayload+nSrc, 0, nPayload-nSrc);
    memcpy(pPayload, pSrc, nSrc);
    testcase( nSrc<nPayload );
    assert( nSrc<=nPayload );
    *pnSize = n;
    if( n<4 ) n = 4;
    testcase( n==4 );
    testcase( n==3 );
    n = nHeader + nPayload;
    ** and no overflow pages are required. */
    /* This is the common case where everything fits on the btree page
  if( nPayload<=pPage->maxLocal ){
  pPayload = &pCell[nHeader];
  /* Fill in the payload */
  
  }
    nHeader += putVarint32(&pCell[nHeader], nPayload);
    pSrc = pX->pKey;
    nSrc = nPayload = (int)pX->nKey;
    assert( pX->nKey<=0x7fffffff && pX->pKey!=0 );
  }else{
    nHeader += putVarint(&pCell[nHeader], *(u64*)&pX->nKey);
    nHeader += putVarint32(&pCell[nHeader], nPayload);
    assert( pPage->intKeyLeaf ); /* fillInCell() only called for leaves */
    nSrc = pX->nData;
    pSrc = pX->pData;
    nPayload = pX->nData + pX->nZero;
  if( pPage->intKey ){
  nHeader = pPage->childPtrSize;
  /* Fill in the header. */

            || sqlite3PagerIswriteable(pPage->pDbPage) );
  assert( pCell<pPage->aData || pCell>=&pPage->aData[pPage->pBt->pageSize]
  ** buffer space that is separate from the pPage buffer area */
  /* pPage is not necessarily writeable since pCell might be auxiliary

  assert( sqlite3_mutex_held(pPage->pBt->mutex) );

  int nHeader;
  Pgno pgnoOvfl;
  BtShared *pBt;
  unsigned char *pPayload;
  unsigned char *pPrior;
  MemPage *pToRelease;
  int spaceLeft;
  int nSrc, n, rc, mn;
  const u8 *pSrc;
  int nPayload;
){
  int *pnSize                    /* Write cell size here */
  const BtreePayload *pX,        /* Payload with which to construct the cell */
  unsigned char *pCell,          /* Complete text of the cell */
  MemPage *pPage,                /* The page that contains the cell */
static int fillInCell(
*/
** later.
** be constructed in this temporary area then copied into pPage->aData
** area.  pCell might point to some temporary storage.  The cell will
** Note that pCell does not necessary need to point to the pPage->aData
**
** for pCell[].
** is responsible for making sure sufficient space has been allocated
** allocated and filled in as necessary.  The calling procedure
** and write that byte sequence into pCell[].  Overflow pages are
** Create the byte sequence used to represent a cell on page pPage
/*

}
  return SQLITE_OK;
  }
    ovflPgno = iNext;
    if( rc ) return rc;
    }
      sqlite3PagerUnref(pOvfl->pDbPage);
    if( pOvfl ){

    }
      rc = freePage2(pBt, pOvfl, ovflPgno);
    }else{
      rc = SQLITE_CORRUPT_BKPT;
      */
      ** can be problematic.
      ** caller is iterating through or using in some other way, this
      ** enabled. If this 'overflow' page happens to be a page that the
      ** freePage2() may zero the page contents if secure-delete mode is
      ** It is helpful to detect this before calling freePage2(), as 
      ** must not really be an overflow page and the database must be corrupt. 
      ** So if there exists more than one reference to this page, then it 
      ** to an overflow page belonging to a cell that is being deleted/updated.
      /* There is no reason any cursor should have an outstanding reference 
    ){
     && sqlite3PagerPageRefcount(pOvfl->pDbPage)!=1
    if( ( pOvfl || ((pOvfl = btreePageLookup(pBt, ovflPgno))!=0) )

    }
      if( rc ) return rc;
      rc = getOverflowPage(pBt, ovflPgno, &pOvfl, &iNext);
    if( nOvfl ){
    }
      return SQLITE_CORRUPT_BKPT;
      ** file the database must be corrupt. */
      ** overflow page. Therefore if ovflPgno<2 or past the end of the 
      /* 0 is not a legal page number and page 1 cannot be an 
    if( ovflPgno<2 || ovflPgno>btreePagecount(pBt) ){
    MemPage *pOvfl = 0;
    Pgno iNext = 0;
  while( nOvfl-- ){
  );
    (CORRUPT_DB && (pInfo->nPayload + ovflPageSize)<ovflPageSize)
  assert( nOvfl>0 || 
  nOvfl = (pInfo->nPayload - pInfo->nLocal + ovflPageSize - 1)/ovflPageSize;
  ovflPageSize = pBt->usableSize - 4;
  assert( pBt->usableSize > 4 );
  pBt = pPage->pBt;
  ovflPgno = get4byte(pCell + pInfo->nSize - 4);
  }
    return SQLITE_CORRUPT_PAGE(pPage);
    /* Cell extends past end of page */
  if( pCell + pInfo->nSize > pPage->aDataEnd ){
  testcase( pCell + (pInfo->nSize-1) == pPage->aDataEnd );
  testcase( pCell + pInfo->nSize == pPage->aDataEnd );
  }
    return SQLITE_OK;  /* No overflow pages. Return without doing anything */
  if( pInfo->nLocal==pInfo->nPayload ){
  pPage->xParseCell(pPage, pCell, pInfo);
  assert( sqlite3_mutex_held(pPage->pBt->mutex) );

  u32 ovflPageSize;
  int nOvfl;
  int rc;
  Pgno ovflPgno;
  BtShared *pBt;
){
  CellInfo *pInfo          /* Size information about the cell */
  unsigned char *pCell,    /* First byte of the Cell */
  MemPage *pPage,          /* The page that contains the Cell */
static int clearCell(
*/
** size information about the cell in pInfo.
** Free any overflow pages associated with the given Cell.  Store
/*

}
  }
    *pRC = freePage2(pPage->pBt, pPage, pPage->pgno);
  if( (*pRC)==SQLITE_OK ){
static void freePage(MemPage *pPage, int *pRC){
}
  return rc;
  releasePage(pTrunk);
  releasePage(pPage);
  }
    pPage->isInit = 0;
  if( pPage ){
freepage_out:

  TRACE(("FREE-PAGE: %d new trunk page replacing %d\n", pPage->pgno, iTrunk));
  put4byte(&pPage1->aData[32], iPage);
  put4byte(&pPage->aData[4], 0);
  put4byte(pPage->aData, iTrunk);
  }
    goto freepage_out;
  if( rc!=SQLITE_OK ){
  rc = sqlite3PagerWrite(pPage->pDbPage);
  }
    goto freepage_out;
  if( pPage==0 && SQLITE_OK!=(rc = btreeGetPage(pBt, iPage, &pPage, 0)) ){
  */
  ** will become the new first trunk page in the free-list.
  ** first trunk in the free-list is full. Either way, the page being freed
  ** Possibly because the free-list is empty, or possibly because the 
  ** the page being freed as a leaf page of the first trunk in the free-list.
  /* If control flows to this point, then it was not possible to add the

  }
    }
      goto freepage_out;
      TRACE(("FREE-PAGE: %d leaf on trunk page %d\n",pPage->pgno,pTrunk->pgno));
      }
        rc = btreeSetHasContent(pBt, iPage);
        }
          sqlite3PagerDontWrite(pPage->pDbPage);
        if( pPage && (pBt->btsFlags & BTS_SECURE_DELETE)==0 ){
        put4byte(&pTrunk->aData[8+nLeaf*4], iPage);
        put4byte(&pTrunk->aData[4], nLeaf+1);
      if( rc==SQLITE_OK ){
      rc = sqlite3PagerWrite(pTrunk->pDbPage);
      */
      ** read by older versions of SQLite.
      ** order that database files created by newer versions of SQLite can be
      ** avoid using the last six entries in the freelist trunk page array in
      ** EVIDENCE-OF: R-19920-11576 However, newer versions of SQLite still
      **
      ** to read "usableSize/4-2" instead of "usableSize/4-8".
      ** to 3.6.0 or later) we should consider fixing the conditional above
      ** for now.  At some point in the future (once everyone has upgraded
      ** we will continue to restrict the number of entries to usableSize/4 - 8
      ** to maintain backwards compatibility with older versions of SQLite,
      ** usableSize/4 - 8 entries will be reported as corrupt.  In order
      ** 3.6.0, databases with freelist trunk pages holding more than
      ** coded.  But due to a coding error in versions of SQLite prior to
      ** usableSize/4 - 2 entries, not usableSize/4 - 8 entries as we have
      ** Note that the trunk page is not really full until it contains
      **
      ** being freed as a new leaf.
      /* In this case there is room on the trunk page to insert the page
    if( nLeaf < (u32)pBt->usableSize/4 - 8 ){
    }
      goto freepage_out;
      rc = SQLITE_CORRUPT_BKPT;
    if( nLeaf > (u32)pBt->usableSize/4 - 2 ){
    assert( pBt->usableSize>32 );
    nLeaf = get4byte(&pTrunk->aData[4]);

    }
      goto freepage_out;
    if( rc!=SQLITE_OK ){
    rc = btreeGetPage(pBt, iTrunk, &pTrunk, 0);
    iTrunk = get4byte(&pPage1->aData[32]);

    u32 nLeaf;                /* Initial number of leaf cells on trunk page */
  if( nFree!=0 ){
  */
  ** is possible to add the page as a new free-list leaf.
  ** first trunk page in the current free-list. This block tests if it
  ** new free-list trunk page. Otherwise, it will become a leaf of the
  ** trunk page in the free-list is full, then this page will become a
  ** possibilities. If the free-list is currently empty, or if the first
  /* Now manipulate the actual database free-list structure. There are two

  }
    if( rc ) goto freepage_out;
    ptrmapPut(pBt, iPage, PTRMAP_FREEPAGE, 0, &rc);
  if( ISAUTOVACUUM ){
  */
  ** to indicate that the page is free.
  /* If the database supports auto-vacuum, write an entry in the pointer-map

  }
    memset(pPage->aData, 0, pPage->pBt->pageSize);
    }
      goto freepage_out;
    ){
     ||            ((rc = sqlite3PagerWrite(pPage->pDbPage))!=0)
    if( (!pPage && ((rc = btreeGetPage(pBt, iPage, &pPage, 0))!=0) )
    */
    ** always fully overwrite deleted information with zeros.
    /* If the secure_delete option is enabled, then
  if( pBt->btsFlags & BTS_SECURE_DELETE ){

  put4byte(&pPage1->aData[36], nFree+1);
  nFree = get4byte(&pPage1->aData[36]);
  if( rc ) goto freepage_out;
  rc = sqlite3PagerWrite(pPage1->pDbPage);
  /* Increment the free page count on pPage1 */

  }
    pPage = btreePageLookup(pBt, iPage);
  }else{
    sqlite3PagerRef(pPage->pDbPage);
    pPage = pMemPage;
  if( pMemPage ){
  }
    return SQLITE_CORRUPT_BKPT;
  if( iPage<2 || iPage>pBt->nPage ){

  assert( !pMemPage || pMemPage->pgno==iPage );
  assert( CORRUPT_DB || iPage>1 );
  assert( sqlite3_mutex_held(pBt->mutex) );

  u32 nFree;                          /* Initial number of pages on free-list */
  int rc;                             /* Return Code */
  MemPage *pPage;                     /* Page being freed. May be NULL. */
  MemPage *pPage1 = pBt->pPage1;      /* Local reference to page 1 */
  Pgno iTrunk = 0;                    /* Page number of free-list trunk page */ 
  MemPage *pTrunk = 0;                /* Free-list trunk page */
static int freePage2(BtShared *pBt, MemPage *pMemPage, Pgno iPage){
*/
** its reference count is not altered by this function.
** If a pointer to a MemPage object is passed as the second argument,
**
** Otherwise, it may pass NULL.
** corresponding to page iPage handy, it may pass it as the second value. 
** If the caller happens to have a pointer to the MemPage object 
** The value passed as the second argument to this function is optional.
**
** It is assumed that the page is not already a part of the free-list.
** This function is used to add page iPage to the database file free-list. 
/*

}
  return rc;
  assert( rc!=SQLITE_OK || (*ppPage)->isInit==0 );
  assert( rc!=SQLITE_OK || sqlite3PagerPageRefcount((*ppPage)->pDbPage)<=1 );
  releasePage(pPrevTrunk);
  releasePage(pTrunk);
end_allocate_page:

  assert( CORRUPT_DB || *pPgno!=PENDING_BYTE_PAGE(pBt) );

  }
    TRACE(("ALLOCATE: %d from end of file\n", *pPgno));
    }
      *ppPage = 0;
      releasePage(*ppPage);
    if( rc!=SQLITE_OK ){
    rc = sqlite3PagerWrite((*ppPage)->pDbPage);
    if( rc ) return rc;
    rc = btreeGetUnusedPage(pBt, *pPgno, ppPage, bNoContent);
    assert( *pPgno!=PENDING_BYTE_PAGE(pBt) );

    *pPgno = pBt->nPage;
    put4byte(28 + (u8*)pBt->pPage1->aData, pBt->nPage);
#endif
    }
      if( pBt->nPage==PENDING_BYTE_PAGE(pBt) ){ pBt->nPage++; }
      pBt->nPage++;
      if( rc ) return rc;
      }
        releasePage(pPg);
        rc = sqlite3PagerWrite(pPg->pDbPage);
      if( rc==SQLITE_OK ){
      rc = btreeGetUnusedPage(pBt, pBt->nPage, &pPg, bNoContent);
      assert( pBt->nPage!=PENDING_BYTE_PAGE(pBt) );
      TRACE(("ALLOCATE: %d from end of file (pointer-map page)\n", pBt->nPage));
      MemPage *pPg = 0;
      */
      ** becomes a new pointer-map page, the second is used by the caller.
      ** at the end of the file instead of one. The first allocated page
      /* If *pPgno refers to a pointer-map page, allocate two new pages
    if( pBt->autoVacuum && PTRMAP_ISPAGE(pBt, pBt->nPage) ){
#ifndef SQLITE_OMIT_AUTOVACUUM

    if( pBt->nPage==PENDING_BYTE_PAGE(pBt) ) pBt->nPage++;
    pBt->nPage++;
    if( rc ) return rc;
    rc = sqlite3PagerWrite(pBt->pPage1->pDbPage);

    int bNoContent = (0==IfNotOmitAV(pBt->bDoTruncate))? PAGER_GET_NOCONTENT:0;
    */
    ** database image and the end of the database file.
    ** here are confined to those pages that lie between the end of the
    ** file on disk. So the effects of disabling the no-content optimization
    ** content for any page that really does lie past the end of the database
    ** Note that the pager will not actually attempt to load or journal 
    **
    ** the current page content before overwriting it.
    ** not set the no-content flag. This causes the pager to load and journal
    ** that is required in the event of a rollback. In this case, do
    ** steps, then the page we are about to allocate may contain content
    ** current transaction has already run one or more incremental-vacuum
    ** from trying to read the pages content from disk. However, if the
    ** pager layer with the 'no-content' flag set. This prevents the pager
    ** Normally, new pages allocated by this block can be requested from the
    **
    ** database image.
    /* There are no pages on the freelist, so append a new page to the
  }else{
    }while( searchList );
      pPrevTrunk = 0;
      releasePage(pPrevTrunk);
      }
        }
          searchList = 0;
          }
            }
              *ppPage = 0;
              releasePage(*ppPage);
            if( rc!=SQLITE_OK ){
            rc = sqlite3PagerWrite((*ppPage)->pDbPage);
          if( rc==SQLITE_OK ){
          rc = btreeGetUnusedPage(pBt, *pPgno, ppPage, noContent);
          noContent = !btreeGetHasContent(pBt, *pPgno)? PAGER_GET_NOCONTENT : 0;
          put4byte(&aData[4], k-1);
          }
            memcpy(&aData[8+closest*4], &aData[4+k*4], 4);
          if( closest<k-1 ){
          if( rc ) goto end_allocate_page;
          rc = sqlite3PagerWrite(pTrunk->pDbPage);
                 *pPgno, closest+1, k, pTrunk->pgno, n-1));
                 ": %d more free pages\n",
          TRACE(("ALLOCATE: %d was leaf %d of %d on trunk %d"
          *pPgno = iPage;
          int noContent;
        ){
         || (iPage==nearby || (iPage<nearby && eMode==BTALLOC_LE)) 
        if( !searchList 
        testcase( iPage==mxPage );
        }
          goto end_allocate_page;
          rc = SQLITE_CORRUPT_PGNO(iTrunk);
        if( iPage>mxPage ){
        testcase( iPage==mxPage );
        iPage = get4byte(&aData[8+closest*4]);

        }
          closest = 0;
        }else{
          }
            }
              }
                dist = d2;
                closest = i;
              if( d2<dist ){
              int d2 = sqlite3AbsInt32(get4byte(&aData[8+i*4]) - nearby);
            for(i=1; i<k; i++){
            dist = sqlite3AbsInt32(get4byte(&aData[8]) - nearby);
            int dist;
          }else{
            }
              }
                break;
                closest = i;
              if( iPage<=nearby ){
              iPage = get4byte(&aData[8+i*4]);
            for(i=0; i<k; i++){
          if( eMode==BTALLOC_LE ){
          closest = 0;
          u32 i;
        if( nearby>0 ){
        unsigned char *aData = pTrunk->aData;
        Pgno iPage;
        u32 closest;
        /* Extract a leaf from the trunk */
      }else if( k>0 ){
#endif
        TRACE(("ALLOCATE: %d trunk - %d free pages left\n", *pPgno, n-1));
        pTrunk = 0;
        }
          }
            put4byte(&pPrevTrunk->aData[0], iNewTrunk);
            }
              goto end_allocate_page;
            if( rc ){
            rc = sqlite3PagerWrite(pPrevTrunk->pDbPage);
          }else{
            put4byte(&pPage1->aData[32], iNewTrunk);
            assert( sqlite3PagerIswriteable(pPage1->pDbPage) );
          if( !pPrevTrunk ){
          releasePage(pNewTrunk);
          memcpy(&pNewTrunk->aData[8], &pTrunk->aData[12], (k-1)*4);
          put4byte(&pNewTrunk->aData[4], k-1);
          memcpy(&pNewTrunk->aData[0], &pTrunk->aData[0], 4);
          }
            goto end_allocate_page;
            releasePage(pNewTrunk);
          if( rc!=SQLITE_OK ){
          rc = sqlite3PagerWrite(pNewTrunk->pDbPage);
          }
            goto end_allocate_page;
          if( rc!=SQLITE_OK ){
          rc = btreeGetUnusedPage(pBt, iNewTrunk, &pNewTrunk, 0);
          testcase( iNewTrunk==mxPage );
          }
            goto end_allocate_page;
            rc = SQLITE_CORRUPT_PGNO(iTrunk);
          if( iNewTrunk>mxPage ){ 
          Pgno iNewTrunk = get4byte(&pTrunk->aData[8]);
          MemPage *pNewTrunk;
          */
          ** page in this case.
          ** pointers to free-list leaves. The first leaf becomes a trunk
          /* The trunk page is required by the caller but it contains 
        }else{
          }
            memcpy(&pPrevTrunk->aData[0], &pTrunk->aData[0], 4);
            }
              goto end_allocate_page;
            if( rc!=SQLITE_OK ){
            rc = sqlite3PagerWrite(pPrevTrunk->pDbPage);
          }else{
            memcpy(&pPage1->aData[32], &pTrunk->aData[0], 4);
          if( !pPrevTrunk ){
        if( k==0 ){
        }
          goto end_allocate_page;
        if( rc ){
        rc = sqlite3PagerWrite(pTrunk->pDbPage);
        searchList = 0;
        *ppPage = pTrunk;
        *pPgno = iTrunk;
        */
        ** to allocate, regardless of whether it has leaves.
        /* The list is being searched and this trunk page is the page
      ){
            && (nearby==iTrunk || (iTrunk<nearby && eMode==BTALLOC_LE)) 
      }else if( searchList 
#ifndef SQLITE_OMIT_AUTOVACUUM
        goto end_allocate_page;
        rc = SQLITE_CORRUPT_PGNO(iTrunk);
        /* Value of k is out of range.  Database corruption */
      }else if( k>(u32)(pBt->usableSize/4 - 2) ){
        TRACE(("ALLOCATE: %d trunk - %d free pages left\n", *pPgno, n-1));
        pTrunk = 0;
        *ppPage = pTrunk;
        memcpy(&pPage1->aData[32], &pTrunk->aData[0], 4);
        *pPgno = iTrunk;
        }
          goto end_allocate_page;
        if( rc ){
        rc = sqlite3PagerWrite(pTrunk->pDbPage);
        assert( pPrevTrunk==0 );
        ** allocated page */
        ** So extract the trunk page itself and use it as the newly 
        /* The trunk has no leaves and the list is not being searched. 
      if( k==0 && !searchList ){
      k = get4byte(&pTrunk->aData[4]);
      ** is the number of leaf page pointers to follow. */
      /* EVIDENCE-OF: R-13523-04394 The second integer on a freelist trunk page
      assert( pTrunk->aData!=0 );
      assert( pTrunk!=0 );
      }
        goto end_allocate_page;
        pTrunk = 0;
      if( rc ){
      }
        rc = btreeGetUnusedPage(pBt, iTrunk, &pTrunk, 0);
      }else{
        rc = SQLITE_CORRUPT_PGNO(pPrevTrunk ? pPrevTrunk->pgno : 1);
      if( iTrunk>mxPage || nSearch++ > n ){
      testcase( iTrunk==mxPage );
      }
        iTrunk = get4byte(&pPage1->aData[32]);
        ** the freelist is empty. */
        ** stores the page number of the first page of the freelist, or zero if
        /* EVIDENCE-OF: R-59841-13798 The 4-byte big-endian integer at offset 32
      }else{
        iTrunk = get4byte(&pPrevTrunk->aData[0]);
        ** zero if this is the last freelist trunk page. */
        ** is the page number of the next freelist trunk page in the list or
        /* EVIDENCE-OF: R-01506-11053 The first integer on a freelist trunk page
      if( pPrevTrunk ){
      pPrevTrunk = pTrunk;
    do {
    */
    ** or until a page less than 'nearby' is located (eMode==BTALLOC_LT)
    ** free-list until the page 'nearby' is located (eMode==BTALLOC_EXACT)
    ** is not true. Otherwise, it runs once for each trunk-page on the
    /* The code within this loop is run only once if the 'searchList' variable

    put4byte(&pPage1->aData[36], n-1);
    if( rc ) return rc;
    rc = sqlite3PagerWrite(pPage1->pDbPage);
    */
    ** first free-list trunk page. iPrevTrunk is initially 1.
    /* Decrement the free-list count by 1. Set iTrunk to the index of the

#endif
    }
      searchList = 1;
    }else if( eMode==BTALLOC_LE ){
      }
        }
          searchList = 1;
        if( eType==PTRMAP_FREEPAGE ){
        if( rc ) return rc;
        rc = ptrmapGet(pBt, nearby, &eType, 0);
        assert( pBt->autoVacuum );
        assert( nearby>0 );
        u8 eType;
      if( nearby<=mxPage ){
    if( eMode==BTALLOC_EXACT ){
#ifndef SQLITE_OMIT_AUTOVACUUM
    */
    ** the entire-list will be searched for that page.
    ** shows that the page 'nearby' is somewhere on the free-list, then
    /* If eMode==BTALLOC_EXACT and a query of the pointer-map
    
    u32 nSearch = 0;   /* Count of the number of search attempts */
    u8 searchList = 0; /* If the free-list must be searched for 'nearby' */
    Pgno iTrunk;
    /* There are pages on the freelist.  Reuse one of those pages. */
  if( n>0 ){
  }
    return SQLITE_CORRUPT_BKPT;
  if( n>=mxPage ){
  testcase( n==mxPage-1 );
  n = get4byte(&pPage1->aData[36]);
  ** stores stores the total number of pages on the freelist. */
  /* EVIDENCE-OF: R-05119-02637 The 4-byte big-endian integer at offset 36
  mxPage = btreePagecount(pBt);
  pPage1 = pBt->pPage1;
  assert( eMode==BTALLOC_ANY || (nearby>0 && IfNotOmitAV(pBt->autoVacuum)) );
  assert( sqlite3_mutex_held(pBt->mutex) );

  Pgno mxPage;     /* Total size of the database file */
  MemPage *pPrevTrunk = 0;
  MemPage *pTrunk = 0;
  u32 k;     /* Number of leaves on the trunk of the freelist */
  u32 n;     /* Number of pages on the freelist */
  int rc;
  MemPage *pPage1;
){
  u8 eMode               /* BTALLOC_EXACT, BTALLOC_LT, or BTALLOC_ANY */
  Pgno nearby,           /* Search for a page near this one */
  Pgno *pPgno,           /* Store the page number here */
  MemPage **ppPage,      /* Store pointer to the allocated page here */
  BtShared *pBt,         /* The btree */
static int allocateBtreePage(
*/
** are no restrictions on which page is returned.
** to nearby if any such page exists.  If eMode is BTALLOC_ANY then there
** eMode is BTALLOC_LT then the page returned will be less than or equal
** anywhere on the free-list, then it is guaranteed to be returned.  If
** If the eMode parameter is BTALLOC_EXACT and the nearby page exists
**
** which in turn can make database access faster.
** attempt to keep related pages close to each other in the database file,
** locate a page close to the page number "nearby".  This can be used in an
** If the "nearby" parameter is not 0, then an effort is made to 
**
** an error.  *ppPage is set to NULL in the event of an error.
** SQLITE_OK is returned on success.  Any other return value indicates
**
** sqlite3PagerUnref() on the new page when it is done.
** been referenced and the calling routine is responsible for calling
** has already been called on the new page.)  The new page has also
** The new page is marked as dirty.  (In other words, sqlite3PagerWrite()
**
** Allocate a new page from the database file.
/*

}
  return SQLITE_OK;
  pCur->ix--;
  }
    return btreePrevious(pCur);
  ){
   || pCur->pPage->leaf==0
   || pCur->ix==0
  if( pCur->eState!=CURSOR_VALID
  pCur->info.nSize = 0;
  pCur->curFlags &= ~(BTCF_AtLast|BTCF_ValidOvfl|BTCF_ValidNKey);
  UNUSED_PARAMETER( flags );  /* Used in COMDB2 but not native SQLite */
  assert( flags==0 || flags==1 );
  assert( cursorOwnsBtShared(pCur) );
int sqlite3BtreePrevious(BtCursor *pCur, int flags){
}
  return rc;
  }
    }
      rc = SQLITE_OK;
    }else{
      rc = sqlite3BtreePrevious(pCur, 0);
    if( pPage->intKey && !pPage->leaf ){
    pPage = pCur->pPage;
    pCur->ix--;

    assert( (pCur->curFlags & (BTCF_ValidOvfl))==0 );
    assert( pCur->info.nSize==0 );
    }
      moveToParent(pCur);
      }
        return SQLITE_DONE;
        pCur->eState = CURSOR_INVALID;
      if( pCur->iPage==0 ){
    while( pCur->ix==0 ){
  }else{
    rc = moveToRightmost(pCur);
    if( rc ) return rc;
    rc = moveToChild(pCur, get4byte(findCell(pPage, idx)));
    int idx = pCur->ix;
  if( !pPage->leaf ){
  assert( pPage->isInit );
  pPage = pCur->pPage;

  }
    }
      if( pCur->skipNext<0 ) return SQLITE_OK;
      pCur->eState = CURSOR_VALID;
    if( CURSOR_SKIPNEXT==pCur->eState ){
    }
      return SQLITE_DONE;
    if( CURSOR_INVALID==pCur->eState ){
    }
      return rc;
    if( rc!=SQLITE_OK ){
    rc = restoreCursorPosition(pCur);
  if( pCur->eState!=CURSOR_VALID ){
  assert( pCur->info.nSize==0 );
  assert( (pCur->curFlags & (BTCF_AtLast|BTCF_ValidOvfl|BTCF_ValidNKey))==0 );
  assert( cursorOwnsBtShared(pCur) );

  MemPage *pPage;
  int rc;
static SQLITE_NOINLINE int btreePrevious(BtCursor *pCur){
*/
** use this hint, but COMDB2 does.
** hint to the implement.  The native SQLite btree implementation does not
** skipped if the SQL index had been a unique index.  The F argument is a
** the cursor corresponds to an SQL index and this routine could have been
** If bit 0x01 of the F argument to sqlite3BtreePrevious(C,F) is 1, then
**
** or to restore the cursor.
** helper routine is called when it is necessary to move to a different page
** to the previous cell on the current page.  The (slower) btreePrevious()
** for the common case of merely decrementing the cell counter BtCursor.aiIdx
** The main entry point is sqlite3BtreePrevious().  That routine is optimized
**
**     otherwise     some kind of error occurred
**     SQLITE_DONE   the cursor is already on the first element of the table
**     SQLITE_OK     success
**
** Return values:
** Step the cursor to the back to the previous entry in the database.
/*

}
  }
    return moveToLeftmost(pCur);
  }else{
    return SQLITE_OK;
  if( pPage->leaf ){
  }
    return btreeNext(pCur);
    pCur->ix--;
  if( (++pCur->ix)>=pPage->nCell ){
  pPage = pCur->pPage;
  if( pCur->eState!=CURSOR_VALID ) return btreeNext(pCur);
  pCur->curFlags &= ~(BTCF_ValidNKey|BTCF_ValidOvfl);
  pCur->info.nSize = 0;
  assert( flags==0 || flags==1 );
  assert( cursorOwnsBtShared(pCur) );
  UNUSED_PARAMETER( flags );  /* Used in COMDB2 but not native SQLite */
  MemPage *pPage;
int sqlite3BtreeNext(BtCursor *pCur, int flags){
}
  }
    return moveToLeftmost(pCur);
  }else{
    return SQLITE_OK;
  if( pPage->leaf ){
  }
    }
      return SQLITE_OK;
    }else{
      return sqlite3BtreeNext(pCur, 0);
    if( pPage->intKey ){
    }while( pCur->ix>=pPage->nCell );
      pPage = pCur->pPage;
      moveToParent(pCur);
      }
        return SQLITE_DONE;
        pCur->eState = CURSOR_INVALID;
      if( pCur->iPage==0 ){
    do{
    }
      return moveToLeftmost(pCur);
      if( rc ) return rc;
      rc = moveToChild(pCur, get4byte(&pPage->aData[pPage->hdrOffset+8]));
    if( !pPage->leaf ){
  if( idx>=pPage->nCell ){

  harmless( idx>pPage->nCell );
  ** addition of anotherValidCursor() condition on balance_deeper().  */
  ** Update 2019-12-23: appears to long longer be possible after the
  **
  ** page into more than one b-tree structure.
  ** only happen if the database is corrupt in such a way as to link the
  ** the page while cursor pCur is holding a reference to it. Which can
  ** to be invalid here. This can only occur if a second cursor modifies
  /* If the database file is corrupt, it is possible for the value of idx 

  }
    return SQLITE_CORRUPT_BKPT;
    ** example. */
    ** module cov1/btree78.test testcase 220 (2018-06-08) for an
    ** has pages in common with the table being queried.  See TH3
    ** in a corrupt database file where the table being DELETE-ed from
    ** SELECT which deletes content out from under an active cursor
    ** recursive SQL function that does a DELETE operation as part of a
    /* The only known way for this to happen is for there to be a
  if( !pPage->isInit ){
  idx = ++pCur->ix;
  pPage = pCur->pPage;

  }
    }
      if( pCur->skipNext>0 ) return SQLITE_OK;
      pCur->eState = CURSOR_VALID;
    if( pCur->eState==CURSOR_SKIPNEXT ){
    }
      return SQLITE_DONE;
    if( CURSOR_INVALID==pCur->eState ){
    }
      return rc;
    if( rc!=SQLITE_OK ){
    rc = restoreCursorPosition(pCur);
    assert( (pCur->curFlags & BTCF_ValidOvfl)==0 );
  if( pCur->eState!=CURSOR_VALID ){
  assert( cursorOwnsBtShared(pCur) );

  MemPage *pPage;
  int idx;
  int rc;
static SQLITE_NOINLINE int btreeNext(BtCursor *pCur){
*/
** this hint, but COMDB2 does.
** is a hint to the implement.  SQLite btree implementation does not use
** skipped if the SQL index had been a unique index.  The F argument
** cursor corresponds to an SQL index and this routine could have been
** If bit 0x01 of the F argument in sqlite3BtreeNext(C,F) is 1, then the
**
** to restore the cursor.
** routine is called when it is necessary to move to a different page or
** to the next cell on the current page.  The (slower) btreeNext() helper
** for the common case of merely incrementing the cell counter BtCursor.aiIdx
** The main entry point is sqlite3BtreeNext().  That routine is optimized
**
**    otherwise        some kind of error occurred
**    SQLITE_DONE      cursor is already pointing at the last element
**    SQLITE_OK        success
**
** Return value:
** Advance the cursor to the next entry in the database. 
/*

}
  return n;
  }
    n *= pCur->apPage[i]->nCell;
  for(i=0; i<pCur->iPage; i++){
  n = pCur->pPage->nCell;

  if( NEVER(pCur->pPage->leaf==0) ) return -1;
  if( NEVER(pCur->eState!=CURSOR_VALID) ) return -1;
  ** will always point to a leaf node. */
  ** opcode, and it that case the cursor will always be valid and
  /* Currently this interface is only called by the OP_IfSmaller

  assert( sqlite3_mutex_held(pCur->pBtree->db->mutex) );
  assert( cursorOwnsBtShared(pCur) );

  u8 i;
  i64 n;
i64 sqlite3BtreeRowCountEst(BtCursor *pCur){
*/
** available.
** pointing to.  Return a negative number if no estimate is currently 
** Return an estimate for the number of rows in the table that pCur is
/*

}
  return (CURSOR_VALID!=pCur->eState);
  */
  ** as well as the boolean result value.
  ** have been deleted? This API will need to change to return an error code
  /* TODO: What if the cursor is in CURSOR_REQUIRESEEK but all table entries
int sqlite3BtreeEof(BtCursor *pCur){
*/
** the first entry.  TRUE is also returned if the table is empty.
** past the last entry in the table or sqlite3BtreePrev() moves past
** TRUE will be returned after a call to sqlite3BtreeNext() moves
**
** Return TRUE if the cursor is not pointing at an entry of the table.
/*


}
  return rc;
  assert( (pCur->curFlags & BTCF_ValidOvfl)==0 );
  pCur->info.nSize = 0;
moveto_finish:
  }
    if( rc ) break;
    rc = moveToChild(pCur, chldPg);
    pCur->ix = (u16)lwr;
    }
      chldPg = get4byte(findCell(pPage, lwr));
    }else{
      chldPg = get4byte(&pPage->aData[pPage->hdrOffset+8]);
    if( lwr>=pPage->nCell ){
moveto_next_layer:
    }
      goto moveto_finish;
      rc = SQLITE_OK;
      *pRes = c;
      pCur->ix = (u16)idx;
      assert( pCur->ix<pCur->pPage->nCell );
    if( pPage->leaf ){
    assert( pPage->isInit );
    assert( lwr==upr+1 || (pPage->intKey && !pPage->leaf) );
    }
      }
        idx = (lwr+upr)>>1;  /* idx = (lwr+upr)/2 */
        assert( lwr+upr>=0 );
        if( lwr>upr ) break;
        }
          goto moveto_finish;
          if( pIdxKey->errCode ) rc = SQLITE_CORRUPT_BKPT;
          pCur->ix = (u16)idx;
          rc = SQLITE_OK;
          *pRes = 0;
          assert( c==0 );
        }else{
          upr = idx-1;
        }else if( c>0 ){
          lwr = idx+1;
        if( c<0 ){
        );
         && (pIdxKey->errCode!=SQLITE_NOMEM || pCur->pBtree->db->mallocFailed)
            (pIdxKey->errCode!=SQLITE_CORRUPT || c==0)
        assert( 
        }
          sqlite3_free(pCellKey);
          c = sqlite3VdbeRecordCompare(nCell, pCellKey, pIdxKey);
          }
            goto moveto_finish;
            sqlite3_free(pCellKey);
          if( rc ){
          pCur->curFlags &= ~BTCF_ValidOvfl;
          memset(((u8*)pCellKey)+nCell,0,nOverrun); /* Fix uninit warnings */
          rc = accessPayload(pCur, 0, nCell, (unsigned char*)pCellKey, 0);
          pCur->ix = (u16)idx;
          }
            goto moveto_finish;
            rc = SQLITE_NOMEM_BKPT;
          if( pCellKey==0 ){
          pCellKey = sqlite3Malloc( nCell+nOverrun );
          }
            goto moveto_finish;
            rc = SQLITE_CORRUPT_PAGE(pPage);
          if( nCell<2 || nCell/pCur->pBt->usableSize>pCur->pBt->nPage ){
          testcase( nCell==2 );  /* Minimum legal index key size */
          testcase( nCell==1 );  /* Invalid key size:  0x80 0x80 0x01 */
          testcase( nCell==0 );  /* Invalid key size:  0x80 0x80 0x00 */
          testcase( nCell<0 );   /* True if key size is 2^32 or more */
          nCell = (int)pCur->info.nKey;
          pPage->xParseCell(pPage, pCellBody, &pCur->info);
          const int nOverrun = 18;  /* Size of the overrun padding */
          u8 * const pCellBody = pCell - pPage->childPtrSize;
          void *pCellKey;
          ** case this happens.  */
          ** bytes of padding is allocated at the end of the buffer in
          ** up to two varints past the end of the buffer. An extra 18 
          ** If the record is corrupt, the xRecordCompare routine may read
          **
          ** buffer before VdbeRecordCompare() can be called. 
          ** and accessPayload() used to retrieve the record into the
          ** this case the whole cell needs to be parsed, a buffer allocated
          /* The record flows over onto one or more overflow pages. In
        }else{
          c = xRecordCompare(nCell, (void*)&pCell[2], pIdxKey);
          testcase( pCell+nCell+2==pPage->aDataEnd );
          ** fits entirely on the main b-tree page.  */
          /* The record-size field is a 2 byte varint and the record 
        ){
          && (nCell = ((nCell&0x7f)<<7) + pCell[1])<=pPage->maxLocal
        }else if( !(pCell[1] & 0x80) 
          c = xRecordCompare(nCell, (void*)&pCell[1], pIdxKey);
          testcase( pCell+nCell+1==pPage->aDataEnd );
          ** b-tree page.  */
          ** single byte varint and the record fits entirely on the main
          /* This branch runs if the record-size field of the cell is a
        if( nCell<=pPage->max1bytePayload ){
        nCell = pCell[0];
        */
        ** 2 bytes of the cell.
        ** stored entirely within the b-tree page by inspecting the first 
        ** the entire cell by checking for the cases where the record is 
        ** varint. This information is used to attempt to avoid parsing 
        ** page is less than 16384 bytes and may be stored as a 2-byte
        ** the maximum number of record bytes stored on an index B-Tree
        /* The maximum supported page-size is 65536 bytes. This means that

        pCell = findCellPastPtr(pPage, idx);
        int nCell;  /* Size of the pCell cell in bytes */
      for(;;){
    }else{
      }
        idx = (lwr+upr)>>1;  /* idx = (lwr+upr)/2; */
        assert( lwr+upr>=0 );
        }
          }
            return SQLITE_OK;
            *pRes = 0;
            pCur->info.nSize = 0;
            pCur->info.nKey = nCellKey;
            pCur->curFlags |= BTCF_ValidNKey;
          }else{
            goto moveto_next_layer;
            lwr = idx;
          if( !pPage->leaf ){
          pCur->ix = (u16)idx;
          assert( nCellKey==intKey );
        }else{
          if( lwr>upr ){ c = +1; break; }
          upr = idx-1;
        }else if( nCellKey>intKey ){
          if( lwr>upr ){ c = -1; break; }
          lwr = idx+1;
        if( nCellKey<intKey ){
        getVarint(pCell, (u64*)&nCellKey);
        }
          }
            }
              return SQLITE_CORRUPT_PAGE(pPage);
            if( pCell>=pPage->aDataEnd ){
          while( 0x80 <= *(pCell++) ){
        if( pPage->intKeyLeaf ){
        pCell = findCellPastPtr(pPage, idx);
        i64 nCellKey;
      for(;;){
    if( xRecordCompare==0 ){
    pCur->ix = (u16)idx;
    idx = upr>>(1-biasRight); /* idx = biasRight ? upr : (lwr+upr)/2; */
    assert( biasRight==0 || biasRight==1 );
    upr = pPage->nCell-1;
    lwr = 0;
    assert( pPage->intKey==(pIdxKey==0) );
    assert( pPage->nCell>0 );
    ** a moveToChild() or moveToRoot() call would have detected corruption.  */
    ** be the right kind (index or table) of b-tree page. Otherwise
    ** would have already detected db corruption. Similarly, pPage must
    ** not run. If this is not the root-page, then the moveToChild() routine
    ** the cursor would have been INVALID above and this for(;;) loop
    /* pPage->nCell must be greater than zero. If this is the root-page

    u8 *pCell;                          /* Pointer to current cell in pPage */
    MemPage *pPage = pCur->pPage;
    Pgno chldPg;
    int lwr, upr, idx, c;
  for(;;){
  assert( pCur->curIntKey || pIdxKey );
  assert( pCur->iPage==0 || pCur->apPage[0]->intKey==pCur->curIntKey );
  assert( pCur->pPage->nCell > 0 );
  assert( pCur->eState==CURSOR_VALID );
  assert( pCur->pPage->isInit );
  assert( pCur->pPage );
  }
    return rc;
    }
      return SQLITE_OK;
      *pRes = -1;
      assert( pCur->pgnoRoot==0 || pCur->pPage->nCell==0 );
    if( rc==SQLITE_EMPTY ){
  if( rc ){
  rc = moveToRoot(pCur);

  }
    xRecordCompare = 0; /* All keys are integers */
  }else{
    );
         || pIdxKey->default_rc==-1
         || pIdxKey->default_rc==0 
    assert( pIdxKey->default_rc==1 
    pIdxKey->errCode = 0;
    xRecordCompare = sqlite3VdbeFindCompare(pIdxKey);
  if( pIdxKey ){

  }
    }
      }
        }
          return rc;
        }else{
          rc = SQLITE_OK;
        }else if( rc==SQLITE_DONE ){
          }
            return SQLITE_OK;
          if( pCur->info.nKey==intKey ){
          getCellInfo(pCur);
        if( rc==SQLITE_OK ){
        rc = sqlite3BtreeNext(pCur, 0);
        *pRes = 0;
      if( pCur->info.nKey+1==intKey ){
      ** is still obtained without this case, only a little more slowely */
      ** binary search.  This is an optimization only.  The correct answer
      ** try to get there using sqlite3BtreeNext() rather than a full
      /* If the requested key is one more than the previous key, then
      }
        return SQLITE_OK;
        *pRes = -1;
      if( (pCur->curFlags & BTCF_AtLast)!=0 ){
    if( pCur->info.nKey<intKey ){
    }
      return SQLITE_OK;
      *pRes = 0;
    if( pCur->info.nKey==intKey ){
  ){
   && pCur->eState==CURSOR_VALID && (pCur->curFlags & BTCF_ValidNKey)!=0
  if( pIdxKey==0
  ** to move to, then just return without doing any work */
  /* If the cursor is already positioned at the point we are trying

  assert( pCur->eState!=CURSOR_VALID || (pIdxKey==0)==(pCur->curIntKey!=0) );
  assert( (pIdxKey==0)==(pCur->pKeyInfo==0) );
  assert( pRes );
  assert( sqlite3_mutex_held(pCur->pBtree->db->mutex) );
  assert( cursorOwnsBtShared(pCur) );

  RecordCompare xRecordCompare;
  int rc;
){
  int *pRes                /* Write search results here */
  int biasRight,           /* If true, bias the search to the high end */
  i64 intKey,              /* The table key */
  UnpackedRecord *pIdxKey, /* Unpacked index key */
  BtCursor *pCur,          /* The cursor to be moved */
int sqlite3BtreeMovetoUnpacked(
*/
** exists an entry in the table that exactly matches pIdxKey.  
** For index tables, the pIdxKey->eqSeen field is set to 1 if there
**
**                  is larger than intKey/pIdxKey.
**     *pRes>0      The cursor is left pointing at an entry that
**
**                  exactly matches intKey/pIdxKey.
**     *pRes==0     The cursor is left pointing at an entry that
**
**                  and the cursor is therefore left point to nothing.
**                  is smaller than intKey/pIdxKey or if the table is empty
**     *pRes<0      The cursor is left pointing at an entry that
**
** *pRes is as follows:
** pointing.  The meaning of the integer written into
** comparing the key with the entry to which the cursor is 
** An integer is written into *pRes which is the result of
**
** before or after the key.
** were present.  The cursor might point to an entry that comes
** left pointing at a leaf page which would hold the entry if it
** If an exact match is not found, then the cursor is always
**
** is ignored.
** must be NULL.  For index tables, pIdxKey is used and intKey
** For INTKEY tables, the intKey parameter is used.  pIdxKey 
**
** specified by pIdxKey or intKey.   Return a success code.
/* Move the cursor so that it points to an entry near the key 

}
  return rc;
  }
    rc = SQLITE_OK;
    *pRes = 1;
    assert( pCur->pgnoRoot==0 || pCur->pPage->nCell==0 );
  }else if( rc==SQLITE_EMPTY ){
    }
      pCur->curFlags &= ~BTCF_AtLast;
    }else{
      pCur->curFlags |= BTCF_AtLast;
    if( rc==SQLITE_OK ){
    rc = moveToRightmost(pCur);
    *pRes = 0;
    assert( pCur->eState==CURSOR_VALID );
  if( rc==SQLITE_OK ){
  rc = moveToRoot(pCur);

  }
    return SQLITE_OK;
    *pRes = 0;
#endif
    assert( pCur->pPage->leaf );
    assert( pCur->ix==pCur->pPage->nCell-1 );
    }
      assert( pCur->aiIdx[ii]==pCur->apPage[ii]->nCell );
    for(ii=0; ii<pCur->iPage; ii++){
    int ii;
    ** to the last entry in the b-tree. */
    /* This block serves to assert() that the cursor really does point 
#ifdef SQLITE_DEBUG
  if( CURSOR_VALID==pCur->eState && (pCur->curFlags & BTCF_AtLast)!=0 ){
  /* If the cursor already points to the last entry, this is a no-op. */

  assert( sqlite3_mutex_held(pCur->pBtree->db->mutex) );
  assert( cursorOwnsBtShared(pCur) );
 
  int rc;
int sqlite3BtreeLast(BtCursor *pCur, int *pRes){
*/
** or set *pRes to 1 if the table is empty.
** on success.  Set *pRes to 0 if the cursor actually points to something
/* Move the cursor to the last entry in the table.  Return SQLITE_OK

}
  return rc;
  }
    rc = SQLITE_OK;
    *pRes = 1;
    assert( pCur->pgnoRoot==0 || pCur->pPage->nCell==0 );
  }else if( rc==SQLITE_EMPTY ){
    rc = moveToLeftmost(pCur);
    *pRes = 0;
    assert( pCur->pPage->nCell>0 );
  if( rc==SQLITE_OK ){
  rc = moveToRoot(pCur);
  assert( sqlite3_mutex_held(pCur->pBtree->db->mutex) );
  assert( cursorOwnsBtShared(pCur) );

  int rc;
int sqlite3BtreeFirst(BtCursor *pCur, int *pRes){
*/
** or set *pRes to 1 if the table is empty.
** on success.  Set *pRes to 0 if the cursor actually points to something
/* Move the cursor to the first entry in the table.  Return SQLITE_OK

}
  return SQLITE_OK;
  assert( (pCur->curFlags & BTCF_ValidNKey)==0 );
  assert( pCur->info.nSize==0 );
  pCur->ix = pPage->nCell-1;
  }
    if( rc ) return rc;
    rc = moveToChild(pCur, pgno);
    pCur->ix = pPage->nCell;
    pgno = get4byte(&pPage->aData[pPage->hdrOffset+8]);
  while( !(pPage = pCur->pPage)->leaf ){
  assert( pCur->eState==CURSOR_VALID );
  assert( cursorOwnsBtShared(pCur) );

  MemPage *pPage = 0;
  int rc = SQLITE_OK;
  Pgno pgno;
static int moveToRightmost(BtCursor *pCur){
*/
** key in ascending order.
** The right-most entry is the one with the largest key - the last
**
** finds the right-most entry beneath the *page*.
** finds the left-most entry beneath the *entry* whereas moveToRightmost()
** between moveToLeftmost() and moveToRightmost().  moveToLeftmost()
** page to which it is currently pointing.  Notice the difference
** Move the cursor down to the right-most leaf entry beneath the
/*

}
  return rc;
  }
    rc = moveToChild(pCur, pgno);
    pgno = get4byte(findCell(pPage, pCur->ix));
    assert( pCur->ix<pPage->nCell );
  while( rc==SQLITE_OK && !(pPage = pCur->pPage)->leaf ){
  assert( pCur->eState==CURSOR_VALID );
  assert( cursorOwnsBtShared(pCur) );

  MemPage *pPage;
  int rc = SQLITE_OK;
  Pgno pgno;
static int moveToLeftmost(BtCursor *pCur){
*/
** in ascending order.
** The left-most leaf is the one with the smallest key - the first
**
** entry to which it is currently pointing.
** Move the cursor down to the left-most leaf entry beneath the
/*

}
  return rc;
  }
    rc = SQLITE_EMPTY;
    pCur->eState = CURSOR_INVALID;
  }else{
    rc = moveToChild(pCur, subpage);
    pCur->eState = CURSOR_VALID;
    subpage = get4byte(&pRoot->aData[pRoot->hdrOffset+8]);
    if( pRoot->pgno!=1 ) return SQLITE_CORRUPT_BKPT;
    Pgno subpage;
  }else if( !pRoot->leaf ){
    pCur->eState = CURSOR_VALID;
  if( pRoot->nCell>0 ){
  pRoot = pCur->pPage;

  pCur->curFlags &= ~(BTCF_AtLast|BTCF_ValidNKey|BTCF_ValidOvfl);
  pCur->info.nSize = 0;
  pCur->ix = 0;
skip_init:  

  }
    return SQLITE_CORRUPT_PAGE(pCur->pPage);
  if( pRoot->isInit==0 || (pCur->pKeyInfo==0)!=pRoot->intKey ){
  assert( pRoot->intKey==1 || pRoot->intKey==0 );
  ** (or the freelist).  */
  ** in such a way that page pRoot is linked into a second b-tree table 
  ** if pCur->iPage>=0). But this is not so if the database is corrupted 
  ** if the root page was already loaded when this function was called (i.e.
  ** Earlier versions of SQLite assumed that this test could not fail
  **
  ** return an SQLITE_CORRUPT error. 
  ** NULL, the caller expects a table b-tree. If this is not the case,
  ** expected to open it on an index b-tree. Otherwise, if pKeyInfo is
  /* If pCur->pKeyInfo is not NULL, then the caller that opened this cursor

  assert( pRoot->pgno==pCur->pgnoRoot );
  pRoot = pCur->pPage;
  }
    pCur->curIntKey = pCur->pPage->intKey;
    pCur->iPage = 0;
    }
      return rc;
      pCur->eState = CURSOR_INVALID;
    if( rc!=SQLITE_OK ){
                        0, pCur->curPagerFlags);
    rc = getAndInitPage(pCur->pBtree->pBt, pCur->pgnoRoot, &pCur->pPage,
    }
      sqlite3BtreeClearCursor(pCur);
      }
        return pCur->skipNext;
        assert( pCur->skipNext!=SQLITE_OK );
      if( pCur->eState==CURSOR_FAULT ){
    if( pCur->eState>=CURSOR_REQUIRESEEK ){
    assert( pCur->iPage==(-1) );
  }else{
    return SQLITE_EMPTY;
    pCur->eState = CURSOR_INVALID;
  }else if( pCur->pgnoRoot==0 ){
    }
      goto skip_init;
      pCur->pPage = pCur->apPage[0];
      }
        releasePageNotNull(pCur->apPage[pCur->iPage]);
      while( --pCur->iPage ){
      releasePageNotNull(pCur->pPage);
    if( pCur->iPage ){
  if( pCur->iPage>=0 ){

  assert( pCur->pgnoRoot>0 || pCur->iPage<0 );
  assert( pCur->eState < CURSOR_REQUIRESEEK || pCur->iPage<0 );
  assert( CURSOR_FAULT   > CURSOR_REQUIRESEEK );
  assert( CURSOR_VALID   < CURSOR_REQUIRESEEK );
  assert( CURSOR_INVALID < CURSOR_REQUIRESEEK );
  assert( cursorOwnsBtShared(pCur) );

  int rc = SQLITE_OK;
  MemPage *pRoot;
static int moveToRoot(BtCursor *pCur){
*/
** b-tree).
** structure the flags byte is set to 0x02 or 0x0A, indicating an index
** indicating a table b-tree, or if the caller did specify a KeyInfo 
** specify a KeyInfo structure the flags byte is set to 0x05 or 0x0D,
** kind of b-tree page (i.e. if when opening the cursor the caller did not
** page-header flags indicate that the [virtual] root-page is the expected 
** If this function returns successfully, it may be assumed that the
**
** (or virtual root) page and the cursor state is set to CURSOR_VALID.
** the cursor is set to point to the first cell located on the root
** CURSOR_INVALID and this routine returns SQLITE_EMPTY. Otherwise,
** If the b-tree structure is empty, the cursor state is set to 
**
** single child page. This can only happen with the table rooted at page 1.
** virtual root page when the actual root page contains no cells and a 
** to the virtual root page instead of the actual root page. A table has a
** If the table has a virtual root page, then the cursor is moved to point
**
** Move the cursor to point to the root page of its b-tree structure.
/*

}
  releasePageNotNull(pLeaf);
  pCur->pPage = pCur->apPage[--pCur->iPage];
  pLeaf = pCur->pPage;
  pCur->ix = pCur->aiIdx[pCur->iPage-1];
  pCur->curFlags &= ~(BTCF_ValidNKey|BTCF_ValidOvfl);
  pCur->info.nSize = 0;
  testcase( pCur->aiIdx[pCur->iPage-1] > pCur->apPage[pCur->iPage-1]->nCell );
  );
    pCur->pPage->pgno
    pCur->aiIdx[pCur->iPage-1], 
    pCur->apPage[pCur->iPage-1], 
  assertParentIndex(
  assert( pCur->pPage );
  assert( pCur->iPage>0 );
  assert( pCur->eState==CURSOR_VALID );
  assert( cursorOwnsBtShared(pCur) );
  MemPage *pLeaf;
static void moveToParent(BtCursor *pCur){
*/
** the largest cell index.
** right-most child page then pCur->idx is set to one more than
** to the page we are coming from.  If we are coming from the
** pCur->idx is set to the cell index that contains the pointer
**
** Move the cursor up to the parent page.
/*

#endif
#  define assertParentIndex(x,y,z) 
#else
}
  }
    assert( get4byte(findCell(pParent, iIdx))==iChild );
  }else{
    assert( get4byte(&pParent->aData[pParent->hdrOffset+8])==iChild );
  if( iIdx==pParent->nCell ){
  assert( iIdx<=pParent->nCell );
                            ** in a corrupt database */
  if( CORRUPT_DB ) return;  /* The conditions tested below might not be true
static void assertParentIndex(MemPage *pParent, int iIdx, Pgno iChild){
*/
** the page.
** cells in pParent, that page number iChild is the right-child of
** cell in page pParent. Or, if iIdx is equal to the total number of
** asserts that page number iChild is the left-child if the iIdx'th
** Page pParent is an internal (non-leaf) tree page. This function 
/*
#ifdef SQLITE_DEBUG

}
  return getAndInitPage(pBt, newPgno, &pCur->pPage, pCur, pCur->curPagerFlags);
  pCur->iPage++;
  pCur->ix = 0;
  pCur->apPage[pCur->iPage] = pCur->pPage;
  pCur->aiIdx[pCur->iPage] = pCur->ix;
  pCur->curFlags &= ~(BTCF_ValidNKey|BTCF_ValidOvfl);
  pCur->info.nSize = 0;
  }
    return SQLITE_CORRUPT_BKPT;
  if( pCur->iPage>=(BTCURSOR_MAX_DEPTH-1) ){
  assert( pCur->iPage>=0 );
  assert( pCur->iPage<BTCURSOR_MAX_DEPTH );
  assert( pCur->eState==CURSOR_VALID );
  assert( cursorOwnsBtShared(pCur) );

  BtShared *pBt = pCur->pBt;
static int moveToChild(BtCursor *pCur, u32 newPgno){
*/
** vice-versa).
** if an intkey page appears to be the parent of a non-intkey page, or
** the new child page does not match the flags field of the parent (i.e.
** This function returns SQLITE_CORRUPT if the page-header flags field of
**
** page number of the child page to move to.
** Move the cursor down to a new child page.  The newPgno argument is the
/*


}
  return fetchPayload(pCur, pAmt);
const void *sqlite3BtreePayloadFetch(BtCursor *pCur, u32 *pAmt){
*/
** in the common case where no overflow pages are used.
** These routines is used to get quick access to key and data
**
** this routine.
** Hence, a mutex on the BtShared should be held prior to calling
** including calls from other threads against the same cache.
** or be destroyed on the next call to any Btree routine,
** The pointer returned is ephemeral.  The key/data may move
**
** b-tree page.  Write the number of available bytes into *pAmt.
** many bytes of the key or data as are available on the local
** For the entry that cursor pCur is point to, return as
/*


}
  return (void*)pCur->info.pPayload;
  *pAmt = (u32)amt;
  }
    amt = MAX(0, (int)(pCur->pPage->aDataEnd - pCur->info.pPayload));
    assert( CORRUPT_DB );
    ** of local content. Database must be corrupt. */
    /* There is too little space on the page for the expected amount
  if( amt>(int)(pCur->pPage->aDataEnd - pCur->info.pPayload) ){
  amt = pCur->info.nLocal;
  assert( pCur->info.pPayload<pCur->pPage->aDataEnd ||CORRUPT_DB);
  assert( pCur->info.pPayload>pCur->pPage->aData || CORRUPT_DB );
  assert( pCur->info.nSize>0 );
  assert( pCur->ix<pCur->pPage->nCell );
  assert( cursorOwnsBtShared(pCur) );
  assert( sqlite3_mutex_held(pCur->pBtree->db->mutex) );
  assert( pCur->eState==CURSOR_VALID );
  assert( pCur!=0 && pCur->iPage>=0 && pCur->pPage);
  int amt;
){
  u32 *pAmt            /* Write the number of available bytes here */
  BtCursor *pCur,      /* Cursor pointing to entry to read from */
static const void *fetchPayload(
*/
** any btree routine is called.
** page of the database.  The data might change or move the next time
** The pointer returned by this routine looks directly into the cached
**
** the key/data and copy it into a preallocated buffer.
** onto overflow pages, then accessPayload() must be used to reassemble
** key and data without making a copy.  If the key and/or data spills
** pages.  When that is so, this routine can be used to access the
** and data to fit on the local page and for there to be no overflow
** This routine is an optimization.  It is common for the entire key
**
** returned will not be a valid pointer.
** key/data is written into *pAmt.  If *pAmt==0, then the value
** table btrees (pPage->intKey==1). The number of bytes of available
** the key if index btrees (pPage->intKey==0) and is the data for
** pCur cursor is pointing to.  The pointer is to the beginning of
** Return a pointer to payload information from the entry that the 
/*

#endif /* SQLITE_OMIT_INCRBLOB */
}
  }
    return accessPayloadChecked(pCur, offset, amt, pBuf);
  }else{
    return accessPayload(pCur, offset, amt, pBuf, 0);
    assert( cursorOwnsBtShared(pCur) );
  if( pCur->eState==CURSOR_VALID ){
int sqlite3BtreePayloadChecked(BtCursor *pCur, u32 offset, u32 amt, void *pBuf){
}
  return rc ? rc : accessPayload(pCur, offset, amt, pBuf, 0);
  rc = btreeRestoreCursorPosition(pCur);
  assert( cursorOwnsBtShared(pCur) );
  }
    return SQLITE_ABORT;
  if ( pCur->eState==CURSOR_INVALID ){
  int rc;
){
  void *pBuf
  u32 amt,
  u32 offset,
  BtCursor *pCur,
static SQLITE_NOINLINE int accessPayloadChecked(
#ifndef SQLITE_OMIT_INCRBLOB
*/
** interface.
** in the CURSOR_VALID state.  It is only used by the sqlite3_blob_read()
** This variant of sqlite3BtreePayload() works even if the cursor has not
/*

}
  return accessPayload(pCur, offset, amt, (unsigned char*)pBuf, 0);
  assert( pCur->ix<pCur->pPage->nCell );
  assert( pCur->iPage>=0 && pCur->pPage );
  assert( pCur->eState==CURSOR_VALID );
  assert( cursorHoldsMutex(pCur) );
int sqlite3BtreePayload(BtCursor *pCur, u32 offset, u32 amt, void *pBuf){
*/
** the available payload.
** wrong.  An error is returned if "offset+amt" is larger than
** Return SQLITE_OK on success or an error code if anything goes
**
** cursor might be invalid or might need to be restored before being read.
** to a valid row in the table.  For sqlite3BtreePayloadChecked(), the
** For sqlite3BtreePayload(), the caller must ensure that pCur is pointing
**
** pCur is pointing to an index b-tree then the key section is read.
** If pointing to a table btree, then the content section is read.  If
** pCur can be pointing to either a table or an index b-tree.
**
** begins at "offset".
** pointing.  "amt" bytes will be transferred into pBuf[].  The transfer
** Read part of the payload for the row at which that cursor pCur is currently
/*

}
  return rc;
  }
    return SQLITE_CORRUPT_PAGE(pPage);
    /* Overflow chain ends prematurely */
  if( rc==SQLITE_OK && amt>0 ){

  }
    }
      iIdx++;
      if( rc ) break;
      }
        pBuf += a;
        if( amt==0 ) return rc;
        amt -= a;
        }
          }
            offset = 0;
            sqlite3PagerUnref(pDbPage);
            rc = copyPayload(&aPayload[offset+4], pBuf, a, eOp, pDbPage);
            nextPage = get4byte(aPayload);
            aPayload = sqlite3PagerGetData(pDbPage);
          if( rc==SQLITE_OK ){
          );
              (eOp==0 ? PAGER_GET_READONLY : 0)
          rc = sqlite3PagerGet(pBt->pPager, nextPage, &pDbPage,
          DbPage *pDbPage;
        {

#endif
        }else
          memcpy(aWrite, aSave, 4);
          nextPage = get4byte(aWrite);
          if( rc && nextPage>pBt->nPage ) rc = SQLITE_CORRUPT_BKPT;
          rc = sqlite3OsRead(fd, aWrite, a+4, (i64)pBt->pageSize*(nextPage-1));
          memcpy(aSave, aWrite, 4);
          assert( aWrite>=pBufStart );                         /* due to (6) */
          u8 *aWrite = &pBuf[-4];
          u8 aSave[4];
          sqlite3_file *fd = sqlite3PagerFile(pBt->pPager);
        ){
         && &pBuf[-4]>=pBufStart                               /* (6) */
         && sqlite3PagerDirectReadOk(pBt->pPager, nextPage)    /* (3,4,5) */
         && offset==0                                          /* (2) */
        if( eOp==0                                             /* (1) */
        */
        ** up loading large records that span many overflow pages.
        ** output buffer, bypassing the page-cache altogether. This speeds
        ** then data can be read directly from the database file into the
        **
        **   6) at least 4 bytes have already been read into the output buffer 
        **   5) the page is not in the WAL file
        **   4) the database is file-backed, and
        **   3) there are no dirty pages in the page-cache
        **   2) data is required from the start of this overflow page, and
        **   1) this is a read operation, and 
        **
        /* If all the following are true:
#ifdef SQLITE_DIRECT_OVERFLOW_READ

        }
          a = ovflSize - offset;
        if( a + offset > ovflSize ){
        int a = amt;
        */
        ** range of data that is being read (eOp==0) or written (eOp!=0).
        /* Need to read this page properly. It contains some of the
      }else{
        offset -= ovflSize;
        }
          rc = getOverflowPage(pBt, nextPage, 0, &nextPage);
        }else{
          nextPage = pCur->aOverflow[iIdx+1];
        if( pCur->aOverflow[iIdx+1] ){
        assert( pCur->pBtree->db==pBt->db );
        assert( pCur->curFlags & BTCF_ValidOvfl );
        */
        ** function.
        ** page-list cache, if any, then fall back to the getOverflowPage()
        ** data is not required. So first try to lookup the overflow
        ** number for the next page in the overflow chain. The page
        /* The only reason to read this page is to obtain the page
      if( offset>=ovflSize ){

      pCur->aOverflow[iIdx] = nextPage;
              || CORRUPT_DB );
              || pCur->aOverflow[iIdx]==nextPage
      assert( pCur->aOverflow[iIdx]==0
      /* If required, populate the overflow page-list cache. */
    while( nextPage ){
    assert( rc==SQLITE_OK && amt>0 );

    }
      }
        offset = (offset%ovflSize);
        nextPage = pCur->aOverflow[iIdx];
        iIdx = (offset/ovflSize);
      if( pCur->aOverflow[offset/ovflSize] ){
      */
      ** directly to it.
      ** entry for the first required overflow page is valid, skip
      /* If the overflow page-list cache has been allocated and the
    }else{
      pCur->curFlags |= BTCF_ValidOvfl;
      memset(pCur->aOverflow, 0, nOvfl*sizeof(Pgno));
      }
        }
          pCur->aOverflow = aNew;
        }else{
          return SQLITE_NOMEM_BKPT;
        if( aNew==0 ){
        );
            pCur->aOverflow, nOvfl*2*sizeof(Pgno)
        Pgno *aNew = (Pgno*)sqlite3Realloc(
      ){
       || nOvfl*(int)sizeof(Pgno) > sqlite3MallocSize(pCur->aOverflow)
      if( pCur->aOverflow==0
      int nOvfl = (pCur->info.nPayload-pCur->info.nLocal+ovflSize-1)/ovflSize;
    if( (pCur->curFlags & BTCF_ValidOvfl)==0 ){
    */
    ** means "not yet known" (the cache is lazily populated).
    ** stored in aOverflow[0], etc. A value of 0 in the aOverflow[] array
    ** in the overflow chain. The page number of the first overflow page is
    ** The aOverflow[] array is sized at one entry for each overflow page
    **
    /* If the BtCursor.aOverflow[] has not been allocated, allocate it now.

    nextPage = get4byte(&aPayload[pCur->info.nLocal]);

    Pgno nextPage;
    const u32 ovflSize = pBt->usableSize - 4;  /* Bytes content per ovfl page */
  if( rc==SQLITE_OK && amt>0 ){


  }
    offset -= pCur->info.nLocal;
  }else{
    amt -= a;
    pBuf += a;
    offset = 0;
    rc = copyPayload(&aPayload[offset], pBuf, a, eOp, pPage->pDbPage);
    }
      a = pCur->info.nLocal - offset;
    if( a+offset>pCur->info.nLocal ){
    int a = amt;
  if( offset<pCur->info.nLocal ){
  /* Check if data must be read/written to/from the btree page itself. */

  }
    return SQLITE_CORRUPT_PAGE(pPage);
    */
    ** but is recast into its current form to avoid integer overflow problems
    **    &aPayload[pCur->info.nLocal] > &pPage->aData[pBt->usableSize]
    ** conditional above is really:
    /* Trying to read or write past the end of the data is an error.  The
  if( (uptr)(aPayload - pPage->aData) > (pBt->usableSize - pCur->info.nLocal) ){
  assert( aPayload > pPage->aData );

  assert( offset+amt <= pCur->info.nPayload );
  aPayload = pCur->info.pPayload;
  getCellInfo(pCur);

  assert( cursorHoldsMutex(pCur) );
  assert( pCur->ix<pPage->nCell );
  assert( pCur->eState==CURSOR_VALID );
  assert( eOp==0 || eOp==1 );
  assert( pPage );

#endif
  unsigned char * const pBufStart = pBuf;     /* Start of original out buffer */
#ifdef SQLITE_DIRECT_OVERFLOW_READ
  BtShared *pBt = pCur->pBt;                  /* Btree this cursor belongs to */
  MemPage *pPage = pCur->pPage;               /* Btree page of current entry */
  int iIdx = 0;
  int rc = SQLITE_OK;
  unsigned char *aPayload;
){
  int eOp              /* zero to read. non-zero to write. */
  unsigned char *pBuf, /* Write the bytes into this buffer */ 
  u32 amt,             /* Read this many bytes */
  u32 offset,          /* Begin reading this far into payload */
  BtCursor *pCur,      /* Cursor pointing to entry to read from */
static int accessPayload(
*/
**   * Creating a table (may require moving an overflow page).
**   * A commit in auto_vacuum="full" mode,
**   * An incremental vacuum,
**
** mode, the following events may invalidate an overflow page-list cache.
** the cursor is moved to a different row. Additionally, in auto-vacuum
** invalidated if some other cursor writes to the same table, or if
** Once an overflow page-list cache has been allocated, it must be
**
** more efficient.
** Subsequent calls use this cache to make seeking to the supplied offset 
** the overflow page-list cache array (BtCursor.aOverflow). 
** this function may allocate space for and lazily populate
** If the current cursor entry uses one or more overflow pages
**
** or be scattered out on multiple overflow pages.
** The content being read or written might appear on the main page
**
** Data is read to or from the buffer pBuf.
** A total of "amt" bytes are read or written beginning at "offset".
**
**   1: The operation is a write. Populate the overflow cache.
**   0: The operation is a read. Populate the overflow cache.
**
** argument is interpreted as follows:
** for the entry that the pCur cursor is pointing to. The eOp
** This function is used to read or overwrite payload information
/*

}
  return SQLITE_OK;
  }
    memcpy(pBuf, pPayload, nByte);
    /* Copy data from page to buffer (a read operation) */
  }else{
    memcpy(pPayload, pBuf, nByte);
    }
      return rc;
    if( rc!=SQLITE_OK ){
    int rc = sqlite3PagerWrite(pDbPage);
    /* Copy data from buffer to page (a write operation) */
  if( eOp ){
){
  DbPage *pDbPage           /* Page containing pPayload */
  int eOp,                  /* 0 -> copy from page, 1 -> copy to page */
  int nByte,                /* Number of bytes to copy */
  void *pBuf,               /* Pointer to buffer */
  void *pPayload,           /* Pointer to page data */
static int copyPayload(
*/
** SQLITE_OK is returned on success, otherwise an error code.
**
** of data are copied from the buffer pBuf to pPayload.
** then sqlite3PagerWrite() is called on pDbPage and nByte bytes
** from pPayload to the buffer pointed at by pBuf. If eOp is true,
** If argument eOp is false, then nByte bytes of data are copied
** pPayload is a pointer to data stored on database page pDbPage.
**
** Copy data from a buffer to a page, or from a page to a buffer.
/*

}
  return (rc==SQLITE_DONE ? SQLITE_OK : rc);
  }
    releasePage(pPage);
  }else{
    *ppPage = pPage;
  if( ppPage ){
  *pPgnoNext = next;

  }
    }
      next = get4byte(pPage->aData);
    if( rc==SQLITE_OK ){
    assert( rc==SQLITE_OK || pPage==0 );
    rc = btreeGetPage(pBt, ovfl, &pPage, (ppPage==0) ? PAGER_GET_READONLY : 0);
  if( rc==SQLITE_OK ){
  assert( next==0 || rc==SQLITE_DONE );

#endif
  }
    }
      }
        rc = SQLITE_DONE;
        next = iGuess;
      if( rc==SQLITE_OK && eType==PTRMAP_OVERFLOW2 && pgno==ovfl ){
      rc = ptrmapGet(pBt, iGuess, &eType, &pgno);
    if( iGuess<=btreePagecount(pBt) ){

    }
      iGuess++;
    while( PTRMAP_ISPAGE(pBt, iGuess) || iGuess==PENDING_BYTE_PAGE(pBt) ){

    u8 eType;
    Pgno iGuess = ovfl+1;
    Pgno pgno;
  if( pBt->autoVacuum ){
  */
  ** number ovfl to determine the next page number.
  ** out to be wrong, fall back to loading the data of page 
  ** the overflow list is page number (ovfl+1). If that guess turns 
  ** autovacuum pointer-map pages. Guess that the next page in 
  /* Try to find the next page in the overflow list using the
#ifndef SQLITE_OMIT_AUTOVACUUM

  assert(pPgnoNext);
  assert( sqlite3_mutex_held(pBt->mutex) );

  int rc = SQLITE_OK;
  MemPage *pPage = 0;
  Pgno next = 0;
){
  Pgno *pPgnoNext              /* OUT: Next overflow page number */
  MemPage **ppPage,            /* OUT: MemPage handle (may be NULL) */
  Pgno ovfl,                   /* Current overflow page number */
  BtShared *pBt,               /* The database file */
static int getOverflowPage(
*/
** *ppPage is set to zero.
** the pointer-map was used to obtain the value for *pPgnoNext), then
** on *ppPage to free the reference. In no reference was obtained (because
** reference. It is the responsibility of the caller to call releasePage()
** to page number pOvfl was obtained, then *ppPage is set to point to that
** If ppPage is not NULL, and a reference to the MemPage object corresponding
**
** list, *pPgnoNext is set to zero. 
** written to *pPgnoNext. If page ovfl is the last page in its linked 
** The page number of the next overflow page in the linked list is 
**
** If an error occurs an SQLite error code is returned. Otherwise:
**
** pointer-map data instead of reading the content of page ovfl to do so. 
** linked list of overflow pages. If possible, it uses the auto-vacuum
** ovfl), this function finds the page number of the next page in the 
** Given the page number of an overflow page in the database (parameter
/*

}
  return pCur->pBt->pageSize * (sqlite3_int64)pCur->pBt->nPage;
  assert( pCur->eState==CURSOR_VALID );
  assert( cursorHoldsMutex(pCur) );
sqlite3_int64 sqlite3BtreeMaxRecordSize(BtCursor *pCur){
*/
** database file.
** The current implementation merely returns the size of the underlying
**
** corrupt datrabase.
** prevent large memory allocations when trying to interpret a
** that SQLite can handle) or more.  But returning a smaller value might
** routine always returns 2147483647 (which is the largest record
** This is an optimization.  Everything will still work if this
**
** that the cursor is pointing into.
** Return an upper bound on the size of any record for the table
/*

}
  return pCur->info.nPayload;
  getCellInfo(pCur);
  assert( pCur->eState==CURSOR_VALID );
  assert( cursorHoldsMutex(pCur) );
u32 sqlite3BtreePayloadSize(BtCursor *pCur){
*/
** that the cursor has Cursor.eState==CURSOR_VALID.
** valid entry.  In other words, the calling procedure must guarantee
** The caller must guarantee that the cursor is pointing to a non-NULL
**
** of data.  For index btrees, this will be the size of the key.
** currently pointing to.  For table btrees, this will be the amount
** Return the number of bytes of payload for the entry that pCur is
/*

#endif /* SQLITE_ENABLE_OFFSET_SQL_FUNC */
}
         (i64)(pCur->info.pPayload - pCur->pPage->aData);
  return (i64)pCur->pBt->pageSize*((i64)pCur->pPage->pgno - 1) +
  getCellInfo(pCur);
  assert( pCur->eState==CURSOR_VALID );
  assert( cursorHoldsMutex(pCur) );
i64 sqlite3BtreeOffset(BtCursor *pCur){
*/
** payload to which the cursor is pointing.
** Return the offset into the database file for the start of the
/*
#ifdef SQLITE_ENABLE_OFFSET_SQL_FUNC

}
  pCur->curFlags &= ~BTCF_Pinned;
  assert( (pCur->curFlags & BTCF_Pinned)!=0 );
void sqlite3BtreeCursorUnpin(BtCursor *pCur){
}
  pCur->curFlags |= BTCF_Pinned;
  assert( (pCur->curFlags & BTCF_Pinned)==0 );
void sqlite3BtreeCursorPin(BtCursor *pCur){
*/
** Pin or unpin a cursor.
/*

}
  return pCur->info.nKey;
  getCellInfo(pCur);
  assert( pCur->curIntKey );
  assert( pCur->eState==CURSOR_VALID );
  assert( cursorHoldsMutex(pCur) );
i64 sqlite3BtreeIntegerKey(BtCursor *pCur){
*/
** is invalid, the result of this routine is undefined.
** ordinary table btree.  If the cursor points to an index btree or
** This routine is only valid for a cursor that is pointing into a
** Return the value of the integer key or "rowid" for a table btree.
/*

}
  return pCur->eState==CURSOR_VALID;
  assert( pCur!=0 );
int sqlite3BtreeCursorIsValidNN(BtCursor *pCur){
#endif /* NDEBUG */
}
  return pCur && pCur->eState==CURSOR_VALID;
int sqlite3BtreeCursorIsValid(BtCursor *pCur){
*/
** This is a verification routine is used only within assert() statements.
** that is currently pointing to a row in a (non-empty) table.
** Return true if the given BtCursor is valid.  A valid cursor is one
/*
#ifndef NDEBUG  /* The next routine used only within assert() statements */

}
  }
    assertCellInfo(pCur);
  }else{
    btreeParseCell(pCur->pPage,pCur->ix,&pCur->info);
    pCur->curFlags |= BTCF_ValidNKey;
  if( pCur->info.nSize==0 ){
static SQLITE_NOINLINE void getCellInfo(BtCursor *pCur){
#endif
  #define assertCellInfo(x)
#else
  }
    assert( CORRUPT_DB || cellInfoEqual(&info, &pCur->info) );
    btreeParseCell(pCur->pPage, pCur->ix, &info);
    memset(&info, 0, sizeof(info));
    CellInfo info;
  static void assertCellInfo(BtCursor *pCur){
  }
    return 1;
    if( a->nSize!=b->nSize ) return 0;
    if( a->nLocal!=b->nLocal ) return 0;
    if( a->nPayload!=b->nPayload ) return 0;
    if( a->pPayload!=b->pPayload ) return 0;
    if( a->nKey!=b->nKey ) return 0;
  static int cellInfoEqual(CellInfo *a, CellInfo *b){
#ifndef NDEBUG
*/
** Using this cache reduces the number of calls to btreeParseCell().
** BtCursor.info is a cache of the information in the current cell.
**
** btreeParseCell() to fill it in.
** BtCursor.info structure.  If it is not already valid, call
** Make sure the BtCursor* given in the argument has a valid
/*

}
  return SQLITE_OK;
  }
    pCur->pBtree = 0;
    sqlite3BtreeLeave(pBtree);
    sqlite3_free(pCur->pKey);
    sqlite3_free(pCur->aOverflow);
    unlockBtreeIfUnused(pBt);
    btreeReleaseAllCursorPages(pCur);
    }
      }while( ALWAYS(pPrev) );
        pPrev = pPrev->pNext;
        }
          break;
          pPrev->pNext = pCur->pNext;
        if( pPrev->pNext==pCur ){
      do{
      BtCursor *pPrev = pBt->pCursor;
    }else{
      pBt->pCursor = pCur->pNext;
    if( pBt->pCursor==pCur ){
    assert( pBt->pCursor!=0 );
    sqlite3BtreeEnter(pBtree);
    BtShared *pBt = pCur->pBt;
  if( pBtree ){
  Btree *pBtree = pCur->pBtree;
int sqlite3BtreeCloseCursor(BtCursor *pCur){
*/
** when the last cursor is closed.
** Close a cursor.  The read lock on the database file is released
/*

}
  memset(p, 0, offsetof(BtCursor, BTCURSOR_FIRST_UNINIT));
void sqlite3BtreeCursorZero(BtCursor *p){
*/
** of run-time by skipping the initialization of those elements.
** do not need to be zeroed and they are large, so we can save a lot
** to zero.  But it turns out that the apPage[] and aiIdx[] arrays
** The simple approach here would be to memset() the entire object
**
** Initialize memory that will be converted into a BtCursor object.
/*

}
  return ROUND8(sizeof(BtCursor));
int sqlite3BtreeCursorSize(void){
*/
** this routine.
** to users so they cannot do the sizeof() themselves - they must call
** sufficient storage to hold a cursor.  The BtCursor object is opaque
** This interfaces is needed so that users of cursors can preallocate
**
** Return the size of a BtCursor object in bytes.
/*

}
  }
    return btreeCursor(p, iTable, wrFlag, pKeyInfo, pCur);
  }else{
    return btreeCursorWithLock(p, iTable, wrFlag, pKeyInfo, pCur);
  if( p->sharable ){
){
  BtCursor *pCur                              /* Write new cursor here */
  struct KeyInfo *pKeyInfo,                   /* First arg to xCompare() */
  int wrFlag,                                 /* 1 to write. 0 read-only */
  int iTable,                                 /* Root page of table to open */
  Btree *p,                                   /* The btree */
int sqlite3BtreeCursor(
}
  return rc;
  sqlite3BtreeLeave(p);
  rc = btreeCursor(p, iTable, wrFlag, pKeyInfo, pCur);
  sqlite3BtreeEnter(p);
  int rc;
){
  BtCursor *pCur                         /* Space for new cursor */
  struct KeyInfo *pKeyInfo,              /* First arg to comparison function */
  int wrFlag,                            /* 1 to write. 0 read-only */
  int iTable,                            /* Root page of table to open */
  Btree *p,                              /* The btree */
static int btreeCursorWithLock(
}
  return SQLITE_OK;
  pCur->eState = CURSOR_INVALID;
  pBt->pCursor = pCur;
  pCur->pNext = pBt->pCursor;
  }
    }
      pCur->curFlags |= BTCF_Multiple;
      pX->curFlags |= BTCF_Multiple;
    if( pX->pgnoRoot==(Pgno)iTable ){
  for(pX=pBt->pCursor; pX; pX=pX->pNext){
  ** cursors *must* have the BTCF_Multiple flag set. */
  /* If there are two or more cursors on the same btree, then all such
  pCur->curPagerFlags = wrFlag ? 0 : PAGER_GET_READONLY;
  pCur->curFlags = wrFlag ? BTCF_WriteFlag : 0;
  pCur->pBt = pBt;
  pCur->pBtree = p;
  pCur->pKeyInfo = pKeyInfo;
  pCur->iPage = -1;
  pCur->pgnoRoot = (Pgno)iTable;
  ** variables and link the cursor into the BtShared list.  */
  /* Now that no other errors can occur, finish filling in the BtCursor

  }
    }
      iTable = 0;
      assert( wrFlag==0 );
    }else if( btreePagecount(pBt)==0 ){
      return SQLITE_CORRUPT_BKPT;
    if( iTable<1 ){
  if( iTable<=1 ){
  }
    if( pBt->pTmpSpace==0 ) return SQLITE_NOMEM_BKPT;
    allocateTempSpace(pBt);
  if( wrFlag ){

  assert( wrFlag==0 || (pBt->btsFlags & BTS_READ_ONLY)==0 );
  assert( pBt->pPage1 && pBt->pPage1->aData );
  assert( wrFlag==0 || p->inTrans==TRANS_WRITE );
  assert( p->inTrans>TRANS_NONE );
  /* Assert that the caller has opened the required transaction. */

  assert( wrFlag==0 || !hasReadConflicts(p, iTable) );
          || iTable<1 );
  assert( hasSharedCacheTableLock(p, iTable, pKeyInfo!=0, (wrFlag?2:1))
  ** this lock.  The iTable<1 term disables the check for corrupt schemas. */
  ** and that no other connection has any open cursor that conflicts with 
  ** b-tree database, the connection is holding the required table locks, 
  /* The following assert statements verify that if this is a sharable 

  );
       || wrFlag==(BTREE_WRCSR|BTREE_FORDELETE) 
       || wrFlag==BTREE_WRCSR 
  assert( wrFlag==0 
  assert( sqlite3BtreeHoldsMutex(p) );

  BtCursor *pX;                          /* Looping over other all cursors */
  BtShared *pBt = p->pBt;                /* Shared b-tree handle */
){
  BtCursor *pCur                         /* Space for new cursor */
  struct KeyInfo *pKeyInfo,              /* First arg to comparison function */
  int wrFlag,                            /* 1 to write. 0 read-only */
  int iTable,                            /* Root page of table to open */
  Btree *p,                              /* The btree */
static int btreeCursor(
*/
** on pCur to initialize the memory space prior to invoking this routine.
** It is assumed that the sqlite3BtreeCursorZero() has been called
**
** will not work correctly.
** root page of a b-tree.  If it is not, then the cursor acquired
** No checking is done to make sure that page iTable really is the
**
** return a null row (2-bytes: 0x01 0x00).
** operations on this cursor can be no-ops and all READ operations can 
** rows are deleted, the FORDELETE flag is a hint that all SEEK and DELETE
** in which index entries are automatically deleted when corresponding table
** this implementation.  But in a hypothetical alternative storage engine 
** as part of a larger DELETE statement.  The FORDELETE hint is not used by
** this cursor will only be used to seek to and delete entries of an index
** is set.  If FORDELETE is set, that is a hint to the implementation that
** The BTREE_FORDELETE bit of wrFlag may optionally be set if BTREE_WRCSR
**
** 4:  There must be an active transaction.
**
** 3:  The database must be writable (not on read-only media)
**
**     the read cursors in the other database connection.
**     the changes made by this write cursor would be visible to
**     cursors open with wrFlag==0 on the same table.  Otherwise
**     but which are not in the READ_UNCOMMITTED state may not have
** 2:  Other database connections that share the same pager cache
**
** 1:  The cursor must have been opened with wrFlag containing BTREE_WRCSR
**
** for writing to be allowed:
** are also met.  These are the conditions that must be met in order
** can be used for reading or for writing if other conditions for writing
** be used for reading.  If the BTREE_WRCSR bit is set, then the cursor
** If the BTREE_WRCSR bit of wrFlag is clear, then the cursor can only
**
** the caller is assumed to have an open write transaction.
** on the database already. If a write-cursor is requested, then
** the caller already has at least a read-only transaction open
** iTable. If a read-only cursor is requested, it is assumed that
** Create a new cursor for the BTree whose root is on the page
/*

}
  return rc;
  }
    sqlite3BtreeLeave(p);
    }
      assert( CORRUPT_DB || pBt->nPage>0 );
      ** the transaction was started. Otherwise, it must be at least 1.  */
      /* pBt->nPage might be zero if the database was corrupt when 

      btreeSetNPage(pBt, pBt->pPage1);
      rc = newDatabase(pBt);
      }
        pBt->nPage = 0;
      if( iSavepoint<0 && (pBt->btsFlags & BTS_INITIALLY_EMPTY)!=0 ){
    if( rc==SQLITE_OK ){
    }
      rc = sqlite3PagerSavepoint(pBt->pPager, op, iSavepoint);
    if( rc==SQLITE_OK ){
    }
      rc = saveAllCursors(pBt, 0, 0);
    if( op==SAVEPOINT_ROLLBACK ){
    sqlite3BtreeEnter(p);
    assert( iSavepoint>=0 || (iSavepoint==-1 && op==SAVEPOINT_ROLLBACK) );
    assert( op==SAVEPOINT_RELEASE || op==SAVEPOINT_ROLLBACK );
    BtShared *pBt = p->pBt;
  if( p && p->inTrans==TRANS_WRITE ){
  int rc = SQLITE_OK;
int sqlite3BtreeSavepoint(Btree *p, int op, int iSavepoint){
*/
** transaction remains open.
** from a normal transaction rollback, as no locks are released and the
** contents of the entire transaction are rolled back. This is different
** SAVEPOINT_ROLLBACK, then iSavepoint may also be -1. In this case the 
** Normally, iSavepoint is greater than or equal to zero. However, if op is
**
** of op.
** savepoint identified by parameter iSavepoint, depending on the value 
** or SAVEPOINT_RELEASE. This function either releases or rolls back the
** The second argument to this function, op, is always SAVEPOINT_ROLLBACK
/*

}
  return rc;
  sqlite3BtreeLeave(p);
  rc = sqlite3PagerOpenSavepoint(pBt->pPager, iStatement);
  */
  ** such savepoints while the statement transaction savepoint is active.
  ** SQL statements. It is illegal to open, release or rollback any
  ** an index greater than all savepoints created explicitly using
  /* At the pager level, a statement transaction is a savepoint with
  assert( pBt->inTransaction==TRANS_WRITE );
  assert( iStatement>p->db->nSavepoint );
  assert( iStatement>0 );
  assert( (pBt->btsFlags & BTS_READ_ONLY)==0 );
  assert( p->inTrans==TRANS_WRITE );
  sqlite3BtreeEnter(p);
  BtShared *pBt = p->pBt;
  int rc;
int sqlite3BtreeBeginStmt(Btree *p, int iStatement){
*/
** using the sqlite3BtreeSavepoint() function.
** iStatement is 1. This anonymous savepoint can be released or rolled back
** are no active savepoints and no other statement-transactions open,
** including the new anonymous savepoint, open on the B-Tree. i.e. if there
** value passed as the second parameter is the total number of savepoints,
** A statement sub-transaction is implemented as an anonymous savepoint. The
**
** can be rolled back without having to rollback the entire transaction.
** error occurs within the statement, the effect of that one statement
** that are contained within a BEGIN...COMMIT block.  If a constraint
** Statement subtransactions are used around individual SQL statements
**
** if the main transaction commits or rolls back.
** before starting a subtransaction. The subtransaction is ended automatically 
** back independently of the main transaction. You must start a transaction 
** Start a statement subtransaction. The subtransaction can be rolled
/*

}
  return rc;
  sqlite3BtreeLeave(p);
  btreeEndTransaction(p);

  }
    btreeClearHasContent(pBt);
    pBt->inTransaction = TRANS_READ;
    assert( countValidCursors(pBt, 1)==0 );
    }
      releasePageOne(pPage1);
      btreeSetNPage(pBt, pPage1);
    if( btreeGetPage(pBt, 1, &pPage1, 0)==SQLITE_OK ){
    ** sure pPage1->aData is set correctly. */
    ** call btreeGetPage() on page 1 again to make
    /* The rollback may have destroyed the pPage1->aData value.  So

    }
      rc = rc2;
    if( rc2!=SQLITE_OK ){
    rc2 = sqlite3PagerRollback(pBt->pPager);
    assert( TRANS_WRITE==pBt->inTransaction );

    int rc2;
  if( p->inTrans==TRANS_WRITE ){

  btreeIntegrity(p);
  }
    if( rc2!=SQLITE_OK ) rc = rc2;
    assert( rc==SQLITE_OK || (writeOnly==0 && rc2==SQLITE_OK) );
    int rc2 = sqlite3BtreeTripAllCursors(p, tripCode, writeOnly);
  if( tripCode ){
  }
    rc = SQLITE_OK;
  }else{
    if( rc ) writeOnly = 0;
    rc = tripCode = saveAllCursors(pBt, 0, 0);
  if( tripCode==SQLITE_OK ){
  sqlite3BtreeEnter(p);
  assert( tripCode==SQLITE_ABORT_ROLLBACK || tripCode==SQLITE_OK );
  assert( writeOnly==1 || writeOnly==0 );

  MemPage *pPage1;
  BtShared *pBt = p->pBt;
  int rc;
int sqlite3BtreeRollback(Btree *p, int tripCode, int writeOnly){
*/
** are no active cursors, it also releases the read lock.
** This will release the write lock on the database file.  If there
**
** a tripped cursor will result in an error.
** tripped if writeOnly is false.  Any attempt to use
** Only write cursors are tripped if writeOnly is true but all cursors are
** If tripCode is not SQLITE_OK then cursors will be invalidated (tripped).
**
** Rollback the transaction in progress.
/*

}
  pBt->nPage = nPage;
  testcase( pBt->nPage!=nPage );
  if( nPage==0 ) sqlite3PagerPagecount(pBt->pPager, &nPage);
  testcase( nPage==0 );
  int nPage = get4byte(&pPage1->aData[28]);
static void btreeSetNPage(BtShared *pBt, MemPage *pPage1){
*/
** state of the database.  Assume pBt->pPage1 is valid.
** Set the pBt->nPage field correctly, according to the current
/*

}
  return rc;
  }
    sqlite3BtreeLeave(pBtree);
    }
      btreeReleaseAllCursorPages(p);
      }
        p->skipNext = errCode;
        p->eState = CURSOR_FAULT;
        sqlite3BtreeClearCursor(p);
      }else{
        }
          }
            break;
            (void)sqlite3BtreeTripAllCursors(pBtree, rc, 0);
          if( rc!=SQLITE_OK ){
          rc = saveCursorPosition(p);
        if( p->eState==CURSOR_VALID || p->eState==CURSOR_SKIPNEXT ){
      if( writeOnly && (p->curFlags & BTCF_WriteFlag)==0 ){
    for(p=pBtree->pBt->pCursor; p; p=p->pNext){
    sqlite3BtreeEnter(pBtree);
  if( pBtree ){
  assert( (writeOnly==0 || writeOnly==1) && BTCF_WriteFlag==1 );

  int rc = SQLITE_OK;
  BtCursor *p;
int sqlite3BtreeTripAllCursors(Btree *pBtree, int errCode, int writeOnly){
*/
** saving a cursor position, an SQLite error code.
** SQLITE_OK is returned if successful, or if an error occurs while
**
** including all read-cursors are tripped.
** saving the current position of a read-only cursor, all cursors, 
** If the writeOnly flag is true and an error is encountered while 
**
** it unsafe for read cursors to continue.
** pages may be moved or deleted from the database altogether, making
** rolled back modified the database schema. In this case b-tree root
** tripped. In general, writeOnly is false if the transaction being
** following the rollback. Or, if writeOnly is false, all cursors are 
** cursors save their current positions so that they may continue 
** flag is true, then only write-cursors need be tripped - read-only
** This routine gets called when a rollback occurs. If the writeOnly
**
** sharing the cache with pBtree.
** that belong to other database connections that happen to be
** Every cursor is a candidate to be tripped, including cursors
**
** trip write cursors and leave read cursors unchanged.
** references.  Or if the writeOnly flag is set to 1, then only
** code to errCode for every cursor on any BtShared that pBtree
** This routine sets the state to CURSOR_FAULT and the error
/*

}
  return rc;
  sqlite3BtreeLeave(p);
  }
    rc = sqlite3BtreeCommitPhaseTwo(p, 0);
  if( rc==SQLITE_OK ){
  rc = sqlite3BtreeCommitPhaseOne(p, 0);
  sqlite3BtreeEnter(p);
  int rc;
int sqlite3BtreeCommit(Btree *p){
*/
** Do both phases of a commit.
/*

}
  return SQLITE_OK;
  sqlite3BtreeLeave(p);
  btreeEndTransaction(p);

  }
    btreeClearHasContent(pBt);
    pBt->inTransaction = TRANS_READ;
    p->iDataVersion--;  /* Compensate for pPager->iDataVersion++; */
    }
      return rc;
      sqlite3BtreeLeave(p);
    if( rc!=SQLITE_OK && bCleanup==0 ){
    rc = sqlite3PagerCommitPhaseTwo(pBt->pPager);
    assert( pBt->nTransaction>0 );
    assert( pBt->inTransaction==TRANS_WRITE );
    BtShared *pBt = p->pBt;
    int rc;
  if( p->inTrans==TRANS_WRITE ){
  */
  ** transaction and set the shared state to TRANS_READ.
  /* If the handle has a write-transaction open, commit the shared-btrees 

  btreeIntegrity(p);
  sqlite3BtreeEnter(p);
  if( p->inTrans==TRANS_NONE ) return SQLITE_OK;

int sqlite3BtreeCommitPhaseTwo(Btree *p, int bCleanup){
*/
** are no active cursors, it also releases the read lock.
** This will release the write lock on the database file.  If there
**
** transitioned to the error state.
** transaction has been closed. This is quite safe, as the pager will have
** reset the b-tree objects internal state to indicate that the write
** functions return code. So, even if an error occurs in the pager layer,
** (by deleting a master journal file) and the caller will ignore this 
** transaction. In this case, the transaction has already been committed 
** is non-zero then this b-tree transaction is part of a multi-file 
** the upper layer will attempt a rollback. However, if the second argument
** finalize the underlying journal file, this function returns an error and
** Normally, if an error occurs while the pager layer is attempting to 
**
** drop locks.
** the rollback journal (which causes the transaction to commit) and
** routine has to do is delete or truncate or zero the header in the
** contents so that they are written onto the disk platter.  All this
** routine did all the work of writing information out to disk and flushing the
** be invoked prior to calling this routine.  The sqlite3BtreeCommitPhaseOne()
** sqlite3BtreeCommitPhaseOne() routine does the first phase and should
** This routine implements the second phase of a 2-phase commit.  The
**
** Commit the transaction currently in progress.
/*

}
  btreeIntegrity(p);

  }
    unlockBtreeIfUnused(pBt);
    p->inTrans = TRANS_NONE;
    ** pager if this call closed the only read or write transaction.  */
    /* Set the current transaction state to TRANS_NONE and unlock the 

    }
      }
        pBt->inTransaction = TRANS_NONE;
      if( 0==pBt->nTransaction ){
      pBt->nTransaction--;
      clearAllSharedCacheTableLocks(p);
    if( p->inTrans!=TRANS_NONE ){
    ** call below will unlock the pager.  */
    ** reaches 0, set the shared state to TRANS_NONE. The unlockBtreeIfUnused()
    ** transaction count of the shared btree. If the transaction count 
    /* If the handle had any kind of transaction open, decrement the 
  }else{
    p->inTrans = TRANS_READ;
    downgradeAllSharedCacheTableLocks(p);
    ** may still be reading from the database.  */
    ** handle, downgrade to a read-only transaction. The other statements
    /* If there are other active statements that belong to this database
  if( p->inTrans>TRANS_NONE && db->nVdbeRead>1 ){
#endif
  pBt->bDoTruncate = 0;
#ifndef SQLITE_OMIT_AUTOVACUUM

  assert( sqlite3BtreeHoldsMutex(p) );
  sqlite3 *db = p->db;
  BtShared *pBt = p->pBt;
static void btreeEndTransaction(Btree *p){
*/
** at the conclusion of a transaction.
** This function is called from both BtreeCommitPhaseTwo() and BtreeRollback()
/*

}
  return rc;
  }
    sqlite3BtreeLeave(p);
    rc = sqlite3PagerCommitPhaseOne(pBt->pPager, zMaster, 0);
#endif
    }
      sqlite3PagerTruncateImage(pBt->pPager, pBt->nPage);
    if( pBt->bDoTruncate ){
    }
      }
        return rc;
        sqlite3BtreeLeave(p);
      if( rc!=SQLITE_OK ){
      rc = autoVacuumCommit(pBt);
    if( pBt->autoVacuum ){
#ifndef SQLITE_OMIT_AUTOVACUUM
    sqlite3BtreeEnter(p);
    BtShared *pBt = p->pBt;
  if( p->inTrans==TRANS_WRITE ){
  int rc = SQLITE_OK;
int sqlite3BtreeCommitPhaseOne(Btree *p, const char *zMaster){
*/
** the write-transaction for this database file is to delete the journal.
** Once this is routine has returned, the only thing required to commit
**
** created, populated with this journal pointer and synced to disk.
** When this is called, the master journal should already have been
**
** (single database transaction).
** individual journal file, or is NULL, indicating no master journal file 
** the name of a master journal file that should be written into the
** Otherwise, sync the database file for the btree pBt. zMaster points to
**
** This call is a no-op if no write-transaction is currently active on pBt.
**
** commit process.
** committed.  See sqlite3BtreeCommitPhaseTwo() for the second phase of the
** disk and we are still holding all locks, so the transaction has not
** At the end of this call, the rollback journal still exists on the
** database are written into the database file and flushed to oxide.
** the disk.  After the journal is safely on oxide, the changes to the
** the journal.  Then the contents of the journal are flushed out to
** the database can be restored to its original state by playing back
** and populated with enough information so that if a power loss occurs
** causes a rollback journal to be created (if it does not already exist)
** This routine does the first phase of a two-phase commit.  This routine
/*

#endif
# define setChildPtrmaps(x) SQLITE_OK
#else /* ifndef SQLITE_OMIT_AUTOVACUUM */

}
  return rc;
  assert( nRef>=sqlite3PagerRefcount(pPager) );

  }
    }
      sqlite3PagerRollback(pPager);
    if( rc!=SQLITE_OK ){
    }
      pBt->nPage = nFin;
      pBt->bDoTruncate = 1;
      put4byte(&pBt->pPage1->aData[28], nFin);
      put4byte(&pBt->pPage1->aData[36], 0);
      put4byte(&pBt->pPage1->aData[32], 0);
      rc = sqlite3PagerWrite(pBt->pPage1->pDbPage);
    if( (rc==SQLITE_DONE || rc==SQLITE_OK) && nFree>0 ){
    }
      rc = incrVacuumStep(pBt, nFin, iFree, 1);
    for(iFree=nOrig; iFree>nFin && rc==SQLITE_OK; iFree--){
    }
      rc = saveAllCursors(pBt, 0, 0);
    if( nFin<nOrig ){
    if( nFin>nOrig ) return SQLITE_CORRUPT_BKPT;
    nFin = finalDbSize(pBt, nOrig, nFree);
    nFree = get4byte(&pBt->pPage1->aData[36]);

    }
      return SQLITE_CORRUPT_BKPT;
      */
      ** is encountered, this indicates corruption.
      ** is either a pointer-map page or the pending-byte page. If one
      /* It is not possible to create a database for which the final page
    if( PTRMAP_ISPAGE(pBt, nOrig) || nOrig==PENDING_BYTE_PAGE(pBt) ){
    nOrig = btreePagecount(pBt);

    Pgno nOrig;        /* Database size before freeing */
    Pgno iFree;        /* The next page to be freed */
    Pgno nFree;        /* Number of pages on the freelist initially */
    Pgno nFin;         /* Number of pages in database after autovacuuming */
  if( !pBt->incrVacuum ){
  assert(pBt->autoVacuum);
  invalidateAllOverflowCache(pBt);
  assert( sqlite3_mutex_held(pBt->mutex) );

  VVA_ONLY( int nRef = sqlite3PagerRefcount(pPager); )
  Pager *pPager = pBt->pPager;
  int rc = SQLITE_OK;
static int autoVacuumCommit(BtShared *pBt){
*/
** pages are in use.
** i.e. the database has been reorganized so that only the first *pnTrunc
** the database file should be truncated to during the commit process. 
** If SQLITE_OK is returned, then *pnTrunc is set to the number of pages
**
** is committed for an auto-vacuum database.
** This routine is called prior to sqlite3PagerCommit when a transaction
/*

}
  return rc;
  sqlite3BtreeLeave(p);
  }
    }
      rc = SQLITE_DONE;
    }else{
      }
        put4byte(&pBt->pPage1->aData[28], pBt->nPage);
        rc = sqlite3PagerWrite(pBt->pPage1->pDbPage);
      if( rc==SQLITE_OK ){
      }
        rc = incrVacuumStep(pBt, nFin, nOrig, 0);
        invalidateAllOverflowCache(pBt);
      if( rc==SQLITE_OK ){
      rc = saveAllCursors(pBt, 0, 0);
    }else if( nFree>0 ){
      rc = SQLITE_CORRUPT_BKPT;
    if( nOrig<nFin ){

    Pgno nFin = finalDbSize(pBt, nOrig, nFree);
    Pgno nFree = get4byte(&pBt->pPage1->aData[36]);
    Pgno nOrig = btreePagecount(pBt);
  }else{
    rc = SQLITE_DONE;
  if( !pBt->autoVacuum ){
  assert( pBt->inTransaction==TRANS_WRITE && p->inTrans==TRANS_WRITE );
  sqlite3BtreeEnter(p);

  BtShared *pBt = p->pBt;
  int rc;
int sqlite3BtreeIncrVacuum(Btree *p){
*/
** SQLITE_OK is returned. Otherwise an SQLite error code. 
** SQLITE_DONE is returned. If it is not finished, but no error occurred,
** If the incremental vacuum is finished after this function has run,
**
** It performs a single unit of work towards an incremental vacuum.
** A write-transaction must be opened before calling this function.
/*

}
  return nFin;

  }
    nFin--;
  while( PTRMAP_ISPAGE(pBt, nFin) || nFin==PENDING_BYTE_PAGE(pBt) ){
  }
    nFin--;
  if( nOrig>PENDING_BYTE_PAGE(pBt) && nFin<PENDING_BYTE_PAGE(pBt) ){
  nFin = nOrig - nFree - nPtrmap;
  nPtrmap = (nFree-nOrig+PTRMAP_PAGENO(pBt, nOrig)+nEntry)/nEntry;
  nEntry = pBt->usableSize/5;

  Pgno nFin;                      /* Return value */
  Pgno nPtrmap;                   /* Number of PtrMap pages to be freed */
  int nEntry;                     /* Number of entries on one ptrmap page */
static Pgno finalDbSize(BtShared *pBt, Pgno nOrig, Pgno nFree){
*/
** size of the database in pages following an auto-vacuum operation.
** nOrig pages in size containing nFree free pages. Return the expected 
** The database opened by the first argument is an auto-vacuum database
/*

}
  return SQLITE_OK;
  }
    pBt->nPage = iLastPg;
    pBt->bDoTruncate = 1;
    }while( iLastPg==PENDING_BYTE_PAGE(pBt) || PTRMAP_ISPAGE(pBt, iLastPg) );
      iLastPg--;
    do {
  if( bCommit==0 ){

  }
    }
      }
        return rc;
      if( rc!=SQLITE_OK ){
      releasePage(pLastPg);
      rc = relocatePage(pBt, pLastPg, eType, iPtrPage, iFreePg, bCommit);
      
      assert( iFreePg<iLastPg );
      }while( bCommit && iFreePg>nFin );
        releasePage(pFreePg);
        }
          return rc;
          releasePage(pLastPg);
        if( rc!=SQLITE_OK ){
        rc = allocateBtreePage(pBt, &pFreePg, &iFreePg, iNear, eMode);
        MemPage *pFreePg;
      do {
      }
        iNear = nFin;
        eMode = BTALLOC_LE;
      if( bCommit==0 ){
      */
      ** of the file is found.
      ** looping until a free-page located within the first nFin pages
      ** On the other hand, if bCommit is greater than zero, then keep
      **
      ** is swapped with the first free page pulled off the free list.
      /* If bCommit is zero, this loop runs exactly once and page pLastPg

      }
        return rc;
      if( rc!=SQLITE_OK ){
      rc = btreeGetPage(pBt, iLastPg, &pLastPg, 0);

      Pgno iNear = 0;           /* nearby parameter for allocateBtreePage() */
      u8 eMode = BTALLOC_ANY;   /* Mode parameter for allocateBtreePage() */
      MemPage *pLastPg;
      Pgno iFreePg;             /* Index of free page to move pLastPg to */
    } else {
      }
        releasePage(pFreePg);
        assert( iFreePg==iLastPg );
        }
          return rc;
        if( rc!=SQLITE_OK ){
        rc = allocateBtreePage(pBt, &pFreePg, &iFreePg, iLastPg, BTALLOC_EXACT);
        MemPage *pFreePg;
        Pgno iFreePg;
        */
        ** matter if it still contains some garbage entries.
        ** truncated to zero after this function returns, so it doesn't 
        ** if bCommit is non-zero. In that case, the free-list will be
        /* Remove the page from the files free-list. This is not required
      if( bCommit==0 ){
    if( eType==PTRMAP_FREEPAGE ){

    }
      return SQLITE_CORRUPT_BKPT;
    if( eType==PTRMAP_ROOTPAGE ){
    }
      return rc;
    if( rc!=SQLITE_OK ){
    rc = ptrmapGet(pBt, iLastPg, &eType, &iPtrPage);

    }
      return SQLITE_DONE;
    if( nFreeList==0 ){
    nFreeList = get4byte(&pBt->pPage1->aData[36]);

    Pgno iPtrPage;
    u8 eType;
  if( !PTRMAP_ISPAGE(pBt, iLastPg) && iLastPg!=PENDING_BYTE_PAGE(pBt) ){

  assert( iLastPg>nFin );
  assert( sqlite3_mutex_held(pBt->mutex) );

  int rc;
  Pgno nFreeList;           /* Number of pages still on the free-list */
static int incrVacuumStep(BtShared *pBt, Pgno nFin, Pgno iLastPg, int bCommit){
*/
** operation, or false for an incremental vacuum.
** or an error. bCommit is passed true for an auto-vacuum-on-commit 
** caller will keep calling incrVacuumStep() until it returns SQLITE_DONE 
** If the bCommit parameter is non-zero, this function assumes that the 
**
** were this function called until it returns SQLITE_DONE.
** Parameter nFin is the number of pages that this database would contain
**
** that the last page of the file currently in use is no longer in use.
** More specifically, this function attempts to re-organize the database so 
**
** occurs, return some other error code.
** calling this function again), return SQLITE_DONE. Or, if an error 
** SQLITE_OK. If there is no work to do (and therefore no point in 
** Perform a single step of an incremental-vacuum. If successful, return
/*

static int allocateBtreePage(BtShared *, MemPage **, Pgno *, Pgno, u8);
/* Forward declaration required by incrVacuumStep(). */

}
  return rc;
  }
    }
      ptrmapPut(pBt, iFreePage, eType, iPtrPage, &rc);
    if( rc==SQLITE_OK ){
    releasePage(pPtrPage);
    rc = modifyPagePointer(pPtrPage, iDbPage, iFreePage, eType);
    }
      return rc;
      releasePage(pPtrPage);
    if( rc!=SQLITE_OK ){
    rc = sqlite3PagerWrite(pPtrPage->pDbPage);
    }
      return rc;
    if( rc!=SQLITE_OK ){
    rc = btreeGetPage(pBt, iPtrPage, &pPtrPage, 0);
  if( eType!=PTRMAP_ROOTPAGE ){
  */
  ** iPtrPage.
  ** that it points at iFreePage. Also fix the pointer map entry for
  /* Fix the database pointer on page iPtrPage that pointed at iDbPage so

  }
    }
      }
        return rc;
      if( rc!=SQLITE_OK ){
      ptrmapPut(pBt, nextOvfl, PTRMAP_OVERFLOW2, iFreePage, &rc);
    if( nextOvfl!=0 ){
    Pgno nextOvfl = get4byte(pDbPage->aData);
  }else{
    }
      return rc;
    if( rc!=SQLITE_OK ){
    rc = setChildPtrmaps(pDbPage);
  if( eType==PTRMAP_BTREE || eType==PTRMAP_ROOTPAGE ){
  */
  ** the pointer map needs to be updated for the subsequent overflow page.
  ** pointer to a subsequent overflow page. If this is the case, then
  ** If pDbPage is an overflow page, then the first 4 bytes may store a
  **
  ** pages need to be changed.
  ** that point to overflow pages. The pointer map entries for all these
  /* If pDbPage was a btree-page, then it may have child pages and/or cells

  pDbPage->pgno = iFreePage;
  }
    return rc;
  if( rc!=SQLITE_OK ){
  rc = sqlite3PagerMovepage(pPager, pDbPage->pDbPage, iFreePage, isCommit);
      iDbPage, iFreePage, iPtrPage, eType));
  TRACE(("AUTOVACUUM: Moving %d to free page %d (ptr page %d type %d)\n", 
  /* Move page iDbPage from its current location to page number iFreePage */

  if( iDbPage<3 ) return SQLITE_CORRUPT_BKPT;
  assert( pDbPage->pBt==pBt );
  assert( sqlite3_mutex_held(pBt->mutex) );
      eType==PTRMAP_BTREE || eType==PTRMAP_ROOTPAGE );
  assert( eType==PTRMAP_OVERFLOW2 || eType==PTRMAP_OVERFLOW1 || 

  int rc;
  Pager *pPager = pBt->pPager;
  Pgno iDbPage = pDbPage->pgno;
  MemPage *pPtrPage;   /* The page that contains a pointer to pDbPage */
){
  int isCommit             /* isCommit flag passed to sqlite3PagerMovepage */
  Pgno iFreePage,          /* The location to move pDbPage to */
  Pgno iPtrPage,           /* Pointer map 'page-no' entry for pDbPage */
  u8 eType,                /* Pointer map 'type' entry for pDbPage */
  MemPage *pDbPage,        /* Open page to move */
  BtShared *pBt,           /* Btree */
static int relocatePage(
*/
** page.
** can be written to. The caller has already promised not to write to that
** the journal needs to be sync()ed before database page pDbPage->pgno 
** The isCommit flag indicates that there is no need to remember that
**
** database. The pDbPage reference remains valid.
** Move the open database page pDbPage to location iFreePage in the 
/*


}
  return SQLITE_OK;
  }
    }
      put4byte(&pPage->aData[pPage->hdrOffset+8], iTo);
      }
        return SQLITE_CORRUPT_PAGE(pPage);
          get4byte(&pPage->aData[pPage->hdrOffset+8])!=iFrom ){
      if( eType!=PTRMAP_BTREE || 
    if( i==nCell ){
  
    }
      }
        }
          break;
          put4byte(pCell, iTo);
        if( get4byte(pCell)==iFrom ){
      }else{
        }
          }
            break;
            put4byte(pCell+info.nSize-4, iTo);
          if( iFrom==get4byte(pCell+info.nSize-4) ){
          }
            return SQLITE_CORRUPT_PAGE(pPage);
          if( pCell+info.nSize > pPage->aData+pPage->pBt->usableSize ){
        if( info.nLocal<info.nPayload ){
        pPage->xParseCell(pPage, pCell, &info);
        CellInfo info;
      if( eType==PTRMAP_OVERFLOW1 ){
      u8 *pCell = findCell(pPage, i);
    for(i=0; i<nCell; i++){

    nCell = pPage->nCell;
    if( rc ) return rc;
    rc = pPage->isInit ? SQLITE_OK : btreeInitPage(pPage);

    int rc;
    int nCell;
    int i;
  }else{
    put4byte(pPage->aData, iTo);
    }
      return SQLITE_CORRUPT_PAGE(pPage);
    if( get4byte(pPage->aData)!=iFrom ){
    /* The pointer is always the first 4 bytes of the page in this case.  */
  if( eType==PTRMAP_OVERFLOW2 ){
  assert( sqlite3PagerIswriteable(pPage->pDbPage) );
  assert( sqlite3_mutex_held(pPage->pBt->mutex) );
static int modifyPagePointer(MemPage *pPage, Pgno iFrom, Pgno iTo, u8 eType){
*/
**                   overflow page in the list.
** PTRMAP_OVERFLOW2: pPage is an overflow-page. The pointer points at the next
**
**                   page pointed to by one of the cells on pPage.
** PTRMAP_OVERFLOW1: pPage is a btree-page. The pointer points at an overflow
**
**                   page of pPage.
** PTRMAP_BTREE:     pPage is a btree-page. The pointer points at a child 
**
** be modified, as  follows:
** that it points to iTo. Parameter eType describes the type of pointer to
** Somewhere on pPage is a pointer to page iFrom.  Modify this pointer so
/*

}
  return rc;

  }
    ptrmapPut(pBt, childPgno, PTRMAP_BTREE, pgno, &rc);
    Pgno childPgno = get4byte(&pPage->aData[pPage->hdrOffset+8]);
  if( !pPage->leaf ){

  }
    }
      ptrmapPut(pBt, childPgno, PTRMAP_BTREE, pgno, &rc);
      Pgno childPgno = get4byte(pCell);
    if( !pPage->leaf ){

    ptrmapPutOvflPtr(pPage, pPage, pCell, &rc);

    u8 *pCell = findCell(pPage, i);
  for(i=0; i<nCell; i++){

  nCell = pPage->nCell;
  if( rc!=SQLITE_OK ) return rc;
  rc = pPage->isInit ? SQLITE_OK : btreeInitPage(pPage);
  assert( sqlite3_mutex_held(pPage->pBt->mutex) );

  Pgno pgno = pPage->pgno;
  BtShared *pBt = pPage->pBt;
  int rc;                            /* Return code */
  int nCell;                         /* Number of cells in page pPage */
  int i;                             /* Counter variable */
static int setChildPtrmaps(MemPage *pPage){
*/
** map entries for the overflow pages as well.
** pPage contains cells that point to overflow pages, set the pointer
** Set the pointer-map entries for all children of page pPage. Also, if
/*

#ifndef SQLITE_OMIT_AUTOVACUUM

}
  return rc;
  sqlite3BtreeLeave(p);
  btreeIntegrity(p);

  }
    }
      rc = sqlite3PagerOpenSavepoint(pBt->pPager, p->db->nSavepoint);
      */
      ** the sub-journal is not already open, then it will be opened here.
      ** open savepoints. If the second parameter is greater than 0 and
      /* This call makes sure that the pager has the correct number of
    if( wrflag ){
    }
      *pSchemaVersion = get4byte(&pBt->pPage1->aData[40]);
    if( pSchemaVersion ){
  if( rc==SQLITE_OK ){
trans_begun:

  }
    }
      }
        }
          put4byte(&pPage1->aData[28], pBt->nPage);
        if( rc==SQLITE_OK ){
        rc = sqlite3PagerWrite(pPage1->pDbPage);
      if( pBt->nPage!=get4byte(&pPage1->aData[28]) ){
      */
      ** rollback occurs within the transaction.
      ** re-read the database size from page 1 if a savepoint or transaction
      ** this sooner rather than later means the database size can safely 
      ** client has been writing the database file), update it now. Doing
      /* If the db-size header field is incorrect (as it may be if an old

#endif
      if( wrflag>1 ) pBt->btsFlags |= BTS_EXCLUSIVE;
      pBt->btsFlags &= ~BTS_EXCLUSIVE;
      pBt->pWriter = p;
      assert( !pBt->pWriter );
#ifndef SQLITE_OMIT_SHARED_CACHE
      MemPage *pPage1 = pBt->pPage1;
    if( wrflag ){
    }
      pBt->inTransaction = p->inTrans;
    if( p->inTrans>pBt->inTransaction ){
    p->inTrans = (wrflag?TRANS_WRITE:TRANS_READ);
    }
#endif
      }
        pBt->pLock = &p->lock;
        p->lock.pNext = pBt->pLock;
        p->lock.eLock = READ_LOCK;
        assert( p->lock.pBtree==p && p->lock.iTable==1 );
      if( p->sharable ){
#ifndef SQLITE_OMIT_SHARED_CACHE
      pBt->nTransaction++;
    if( p->inTrans==TRANS_NONE ){
  if( rc==SQLITE_OK ){

  sqlite3PagerResetLockTimeout(pBt->pPager);
          btreeInvokeBusyHandler(pBt) );
  }while( (rc&0xFF)==SQLITE_BUSY && pBt->inTransaction==TRANS_NONE &&
    }
      unlockBtreeIfUnused(pBt);
    if( rc!=SQLITE_OK ){
  
    }
      }
        }
          rc = SQLITE_BUSY;
          ** code to SQLITE_BUSY. */
          ** called and SQLITE_BUSY_SNAPSHOT is returned, change the error
          /* if there was no transaction opened when this function was
        }else if( rc==SQLITE_BUSY_SNAPSHOT && pBt->inTransaction==TRANS_NONE ){
          rc = newDatabase(pBt);
        if( rc==SQLITE_OK ){
        rc = sqlite3PagerBegin(pBt->pPager,wrflag>1,sqlite3TempInMemory(p->db));
      }else{
        rc = SQLITE_READONLY;
      if( (pBt->btsFlags & BTS_READ_ONLY)!=0 ){
    if( rc==SQLITE_OK && wrflag ){

    while( pBt->pPage1==0 && SQLITE_OK==(rc = lockBtree(pBt)) );
    */
    ** pBt->pageSize to the page-size of the file on disk.
    ** file is not pBt->pageSize. In this case lockBtree() will update
    ** reading page 1 it discovers that the page-size of the database 
    ** may return SQLITE_OK but leave pBt->pPage1 set to 0 if after
    ** lockBtree() returns something other than SQLITE_OK. lockBtree()
    /* Call lockBtree() until either pBt->pPage1 is populated or
  do {
  if( pBt->nPage==0 ) pBt->btsFlags |= BTS_INITIALLY_EMPTY;
  pBt->btsFlags &= ~BTS_INITIALLY_EMPTY;

  if( SQLITE_OK!=rc ) goto trans_begun;
  rc = querySharedCacheTableLock(p, MASTER_ROOT, READ_LOCK);
  ** on page 1, the transaction cannot be opened. */
  ** page 1. So if some other shared-cache client already has a write-lock 
  /* Any read-only or read-write transaction implies a read-lock on 

#endif
  }
    }
      goto trans_begun;
      rc = SQLITE_LOCKED_SHAREDCACHE;
      sqlite3ConnectionBlocked(p->db, pBlock);
    if( pBlock ){
    }
      }
        }
          break;
          pBlock = pIter->pBtree->db;
        if( pIter->pBtree!=p ){
      for(pIter=pBt->pLock; pIter; pIter=pIter->pNext){
      BtLock *pIter;
    }else if( wrflag>1 ){
      pBlock = pBt->pWriter->db;
    ){
     || (pBt->btsFlags & BTS_PENDING)!=0
    if( (wrflag && pBt->inTransaction==TRANS_WRITE)
    */
    ** requested, return SQLITE_LOCKED.
    ** on this shared-btree structure and a second write transaction is
    /* If another database handle has already opened a write transaction 
    sqlite3 *pBlock = 0;
  {
#ifndef SQLITE_OMIT_SHARED_CACHE

  }
    goto trans_begun;
    rc = SQLITE_READONLY;
  if( (pBt->btsFlags & BTS_READ_ONLY)!=0 && wrflag ){
  /* Write transactions are not possible on a read-only database */

  }
    pBt->btsFlags &= ~BTS_READ_ONLY;
  ){
   && sqlite3PagerIsreadonly(pBt->pPager)==0 
  if( (p->db->flags & SQLITE_ResetDatabase) 

  assert( pBt->inTransaction==TRANS_WRITE || IfNotOmitAV(pBt->bDoTruncate)==0 );
  }
    goto trans_begun;
  if( p->inTrans==TRANS_WRITE || (p->inTrans==TRANS_READ && !wrflag) ){
  */
  ** is requested, this is a no-op.
  ** is already in a read-transaction and a read-transaction
  /* If the btree is already in a write-transaction, or it

  btreeIntegrity(p);
  sqlite3BtreeEnter(p);

  int rc = SQLITE_OK;
  BtShared *pBt = p->pBt;
int sqlite3BtreeBeginTrans(Btree *p, int wrflag, int *pSchemaVersion){
*/
** proceed.
** when A already has a read lock, we encourage A to give up and let B
** no progress.  By returning SQLITE_BUSY and not invoking the busy callback
** One or the other of the two processes must give way or there can be
** of A's read lock.  A tries to promote to reserved but is blocked by B.
** a reserved lock.  B tries to promote to exclusive but is blocked because
** Suppose there are two processes A and B.  A has a read lock and B has
**
** returned when there is already a read-lock in order to avoid a deadlock.
** invoke the busy handler - just return SQLITE_BUSY.  SQLITE_BUSY is 
** if there is one.  But if there was previously a read-lock, do not
** and the database was previously unlocked, then invoke the busy handler
** If an initial attempt to acquire the lock fails because of lock contention
**
**      sqlite3BtreeUpdateMeta()
**      sqlite3BtreeDelete()
**      sqlite3BtreeInsert()
**      sqlite3BtreeDropTable()
**      sqlite3BtreeClearTable()
**      sqlite3BtreeCreateIndex()
**      sqlite3BtreeCreateTable()
**
** will work unless a transaction is started first:
** changes to the database.  None of the following routines 
** A write-transaction must be started before attempting any 
**
** exclusivity flag only works for a new transaction.
** upgraded to exclusive by calling this routine a second time - the
** to access the database.  A preexisting transaction may not be
** transaction is started, meaning that no other process is allowed
** transaction.  If the second argument is 2 or more and exclusive
** is started if the second argument is nonzero, otherwise a read-
** Attempt to start a new transaction. A write-transaction
/*

}
  return rc;
  sqlite3BtreeLeave(p);
  rc = newDatabase(p->pBt);
  p->pBt->nPage = 0;
  sqlite3BtreeEnter(p);
  int rc;
int sqlite3BtreeNewDb(Btree *p){
*/
** if successful, or an SQLite error code otherwise.
** consisting of a single page and no schema objects). Return SQLITE_OK
** Initialize the first page of the database file (creating a database
/*

}
  return SQLITE_OK;
  data[31] = 1;
  pBt->nPage = 1;
#endif
  put4byte(&data[36 + 7*4], pBt->incrVacuum);
  put4byte(&data[36 + 4*4], pBt->autoVacuum);
  assert( pBt->incrVacuum==1 || pBt->incrVacuum==0 );
  assert( pBt->autoVacuum==1 || pBt->autoVacuum==0 );
#ifndef SQLITE_OMIT_AUTOVACUUM
  pBt->btsFlags |= BTS_PAGESIZE_FIXED;
  zeroPage(pP1, PTF_INTKEY|PTF_LEAF|PTF_LEAFDATA );
  memset(&data[24], 0, 100-24);
  data[23] = 32;
  data[22] = 32;
  data[21] = 64;
  data[20] = (u8)(pBt->pageSize - pBt->usableSize);
  assert( pBt->usableSize<=pBt->pageSize && pBt->usableSize+255>=pBt->pageSize);
  data[19] = 1;
  data[18] = 1;
  data[17] = (u8)((pBt->pageSize>>16)&0xff);
  data[16] = (u8)((pBt->pageSize>>8)&0xff);
  assert( sizeof(zMagicHeader)==16 );
  memcpy(data, zMagicHeader, sizeof(zMagicHeader));
  if( rc ) return rc;
  rc = sqlite3PagerWrite(pP1->pDbPage);
  data = pP1->aData;
  assert( pP1!=0 );
  pP1 = pBt->pPage1;
  }
    return SQLITE_OK;
  if( pBt->nPage>0 ){
  assert( sqlite3_mutex_held(pBt->mutex) );

  int rc;
  unsigned char *data;
  MemPage *pP1;
static int newDatabase(BtShared *pBt){
*/
** the database.
** into a new empty database by initializing the first page of
** If pBt points to an empty file then convert that empty file
/*

}
  }
    releasePageOne(pPage1);
    pBt->pPage1 = 0;
    assert( sqlite3PagerRefcount(pBt->pPager)==1 );
    assert( pPage1->aData );
    MemPage *pPage1 = pBt->pPage1;
  if( pBt->inTransaction==TRANS_NONE && pBt->pPage1!=0 ){
  assert( countValidCursors(pBt,0)==0 || pBt->inTransaction>TRANS_NONE );
  assert( sqlite3_mutex_held(pBt->mutex) );
static void unlockBtreeIfUnused(BtShared *pBt){
*/
** If there is a transaction in progress, this routine is a no-op.
**
** has the effect of releasing the read lock.
** this routine unrefs the first page of the database file which 
** of a transaction but there is a read lock on the database, then
** If there are no outstanding cursors and we are not in the middle
/*

#endif
}
  return r;
  }
     && pCur->eState!=CURSOR_FAULT ) r++; 
    if( (wrOnly==0 || (pCur->curFlags & BTCF_WriteFlag)!=0)
  for(pCur=pBt->pCursor; pCur; pCur=pCur->pNext){
  int r = 0;
  BtCursor *pCur;
static int countValidCursors(BtShared *pBt, int wrOnly){
*/
** have been tripped into the CURSOR_FAULT state are not counted.
** is capable of reading or writing to the database.  Cursors that
** For the purposes of this routine, a cursor is any cursor that
**
** false then all cursors are counted.
** Only write cursors are counted if wrOnly is true.  If wrOnly is
**
** defined.
** in assert() expressions, so it is only compiled if NDEBUG is not
** Return the number of cursors open on pBt. This is for use
/*
#ifndef NDEBUG

}
  return rc;
  pBt->pPage1 = 0;
  releasePageOne(pPage1);
page1_init_failed:

  return SQLITE_OK;
  pBt->nPage = nPage;
  pBt->pPage1 = pPage1;
  assert( pBt->maxLeaf + 23 <= MX_CELL_SIZE(pBt) );
  }
    pBt->max1bytePayload = (u8)pBt->maxLocal;
  }else{
    pBt->max1bytePayload = 127;
  if( pBt->maxLocal>127 ){
  pBt->minLeaf = (u16)((pBt->usableSize-12)*32/255 - 23);
  pBt->maxLeaf = (u16)(pBt->usableSize - 35);
  pBt->minLocal = (u16)((pBt->usableSize-12)*32/255 - 23);
  pBt->maxLocal = (u16)((pBt->usableSize-12)*64/255 - 23);
  */
  ** page pointer.
  ** 17 bytes long, 0 to N bytes of payload, and an optional 4 byte overflow
  ** So a cell consists of a 2-byte pointer, a header which is as much as
  **     4-byte overflow page pointer
  **     4-byte nData value
  **     9-byte nKey value
  **     4-byte child pointer
  **     2-byte pointer to the cell
  ** Besides the payload, the cell must store:
  ** cells can will fit on one page.  We assume a 10-byte page header.
  ** a cell.  Make sure it is small enough so that at least minFanout
  /* maxLocal is the maximum amount of payload to store locally for

  }
#endif
    pBt->incrVacuum = (get4byte(&page1[36 + 7*4])?1:0);
    pBt->autoVacuum = (get4byte(&page1[36 + 4*4])?1:0);
#ifndef SQLITE_OMIT_AUTOVACUUM
    pBt->usableSize = usableSize;
    pBt->pageSize = pageSize;
    }
      goto page1_init_failed;
    if( usableSize<480 ){
    ** reserved space size cannot exceed 32. */
    ** be less than 480. In other words, if the page size is 512, then the
    /* EVIDENCE-OF: R-28312-64704 However, the usable size is not allowed to
    }
      goto page1_init_failed;
      rc = SQLITE_CORRUPT_BKPT;
    if( sqlite3WritableSchema(pBt->db)==0 && nPage>nPageFile ){
    }
      return rc;
                                   pageSize-usableSize);
      rc = sqlite3PagerSetPagesize(pBt->pPager, &pBt->pageSize,
      freeTempSpace(pBt);
      pBt->pageSize = pageSize;
      pBt->usableSize = usableSize;
      releasePageOne(pPage1);
      */
      ** again with the correct page-size.
      ** zero and return SQLITE_OK. The caller will call this function
      ** actually pageSize. Unlock the database, leave pBt->pPage1 at
      ** of BtShared.pageSize, we have discovered that the page-size is
      /* After reading the first page of the database assuming a page size
    if( (u32)pageSize!=pBt->pageSize ){
    usableSize = pageSize - page1[20];
    ** into the database file header. */
    ** determined by the one-byte unsigned integer found at an offset of 20
    ** EVIDENCE-OF: R-37497-42412 The size of the reserved region is
    **
    ** each page to reserve for extensions. 
    ** integer at offset 20 is the number of bytes of space at the end of
    /* EVIDENCE-OF: R-59310-51205 The "reserved space" size in the 1-byte
    assert( (pageSize & 7)==0 );
    pBt->btsFlags |= BTS_PAGESIZE_FIXED;
    }
      goto page1_init_failed;
    ){
     || pageSize<=256 
     || pageSize>SQLITE_MAX_PAGE_SIZE 
    if( ((pageSize-1)&pageSize)!=0
    ** between 512 and 65536 inclusive. */
    /* EVIDENCE-OF: R-25008-21688 The size of a page is a power of two
    pageSize = (page1[16]<<8) | (page1[17]<<16);
    ** the beginning of the database file. */
    ** determined by the 2-byte integer located at an offset of 16 bytes from
    /* EVIDENCE-OF: R-51873-39618 The page size for a database file is
    }
      goto page1_init_failed;
    if( memcmp(&page1[21], "\100\040\040",3)!=0 ){
    */
    ** version 3.6.0, we require them to be fixed.
    ** The original design allowed these amounts to vary, but as of
    **
    ** fractions and the leaf payload fraction values must be 64, 32, and 32.
    /* EVIDENCE-OF: R-15465-20813 The maximum and minimum embedded payload

#endif
    }
      setDefaultSyncFlag(pBt, SQLITE_DEFAULT_SYNCHRONOUS+1);
    }else{
      rc = SQLITE_NOTADB;
      }
        }
          return SQLITE_OK;
          releasePageOne(pPage1);
        if( isOpen==0 ){
        setDefaultSyncFlag(pBt, SQLITE_DEFAULT_WAL_SYNCHRONOUS+1);
      }else{
        goto page1_init_failed;
      if( rc!=SQLITE_OK ){
      rc = sqlite3PagerOpenWal(pBt->pPager, &isOpen);
      int isOpen = 0;
    if( page1[19]==2 && (pBt->btsFlags & BTS_NO_WAL)==0 ){
    */
    ** file.
    ** may not be the latest version - there may be a newer one in the log
    ** required as the version of page 1 currently in the page1 buffer
    ** The caller detects this and calls this function again. This is
    ** return SQLITE_OK and return without populating BtShared.pPage1.
    ** in WAL mode. If the log is not already open, open it now. Then 
    /* If the write version is set to 2, this database should be accessed

    }
      goto page1_init_failed;
    if( page1[19]>2 ){
    }
      pBt->btsFlags |= BTS_READ_ONLY;
    if( page1[18]>2 ){
#else
    }
      goto page1_init_failed;
    if( page1[19]>1 ){
    }
      pBt->btsFlags |= BTS_READ_ONLY;
    if( page1[18]>1 ){
#ifdef SQLITE_OMIT_WAL

    }
      goto page1_init_failed;
    if( memcmp(page1, zMagicHeader, 16)!=0 ){
    ** 61 74 20 33 00. */
    ** with the following 16 bytes (in hex): 53 51 4c 69 74 65 20 66 6f 72 6d
    /* EVIDENCE-OF: R-43737-39999 Every valid SQLite database file begins
    rc = SQLITE_NOTADB;
    u8 *page1 = pPage1->aData;
    u32 usableSize;
    u32 pageSize;
  if( nPage>0 ){
  }
    nPage = 0;
  if( (pBt->db->flags & SQLITE_ResetDatabase)!=0 ){
  }
    nPage = nPageFile;
  if( nPage==0 || memcmp(24+(u8*)pPage1->aData, 92+(u8*)pPage1->aData,4)!=0 ){
  sqlite3PagerPagecount(pBt->pPager, (int*)&nPageFile);
  nPage = nPageHeader = get4byte(28+(u8*)pPage1->aData);
  */
  ** a valid database file. 
  /* Do some checking to help insure the file we opened really is

  if( rc!=SQLITE_OK ) return rc;
  rc = btreeGetPage(pBt, 1, &pPage1, 0);
  if( rc!=SQLITE_OK ) return rc;
  rc = sqlite3PagerSharedLock(pBt->pPager);
  assert( pBt->pPage1==0 );
  assert( sqlite3_mutex_held(pBt->mutex) );

  u32 nPageHeader;     /* Number of pages in the database according to hdr */
  u32 nPageFile = 0;   /* Number of pages in the database file */
  u32 nPage;           /* Number of pages in the database */
  MemPage *pPage1;     /* Page 1 of the database file */
  int rc;              /* Result code from subfunctions */
static int lockBtree(BtShared *pBt){
*/
** is returned if we run out of memory. 
** SQLITE_BUSY is returned if the database is locked.  SQLITE_NOMEM
** well-formed database file, then SQLITE_CORRUPT is returned.
** SQLITE_OK is returned on success.  If the file is not a
**
** also acquire a readlock on that file.
** Get a reference to pPage1 of the database file.  This will
/*


static int newDatabase(BtShared*);
/* Forward declaration */

#endif
# define setDefaultSyncFlag(pBt,safety_level)
#else
}
  }
    }
          pDb->safety_level | (db->flags & PAGER_FLAGS_MASK));
      sqlite3PagerSetFlags(pBt->pPager,
      pDb->safety_level = safety_level;
    ){
     && pDb!=&db->aDb[1] 
     && pDb->safety_level!=safety_level 
    if( pDb->bSyncSet==0 
    while( pDb->pBt==0 || pDb->pBt->pBt!=pBt ){ pDb++; }
  if( (db=pBt->db)!=0 && (pDb=db->aDb)!=0 ){
  Db *pDb;
  sqlite3 *db;
static void setDefaultSyncFlag(BtShared *pBt, u8 safety_level){
    && !defined(SQLITE_OMIT_WAL)
#if SQLITE_DEFAULT_SYNCHRONOUS!=SQLITE_DEFAULT_WAL_SYNCHRONOUS \
*/
** set it so.
** set to the value passed to this function as the second parameter,
** using "PRAGMA synchronous", and if the safety-level is not already
** If the user has not set the safety-level for this database connection
/*

}
#endif
  return rc;
  sqlite3BtreeLeave(p);
  );
    BTREE_AUTOVACUUM_INCR
    (!p->pBt->incrVacuum)?BTREE_AUTOVACUUM_FULL:
    (!p->pBt->autoVacuum)?BTREE_AUTOVACUUM_NONE:
  rc = (
  sqlite3BtreeEnter(p);
  int rc;
#else
  return BTREE_AUTOVACUUM_NONE;
#ifdef SQLITE_OMIT_AUTOVACUUM
int sqlite3BtreeGetAutoVacuum(Btree *p){
*/
** enabled 1 is returned. Otherwise 0.
** Return the value of the 'auto-vacuum' property. If auto-vacuum is 
/*

}
#endif
  return rc;
  sqlite3BtreeLeave(p);
  }
    pBt->incrVacuum = av==2 ?1:0;
    pBt->autoVacuum = av ?1:0;
  }else{
    rc = SQLITE_READONLY;
  if( (pBt->btsFlags & BTS_PAGESIZE_FIXED)!=0 && (av ?1:0)!=pBt->autoVacuum ){
  sqlite3BtreeEnter(p);

  u8 av = (u8)autoVacuum;
  int rc = SQLITE_OK;
  BtShared *pBt = p->pBt;
#else
  return SQLITE_READONLY;
#ifdef SQLITE_OMIT_AUTOVACUUM
int sqlite3BtreeSetAutoVacuum(Btree *p, int autoVacuum){
*/
** determined by the SQLITE_DEFAULT_AUTOVACUUM macro.
** is disabled. The default value for the auto-vacuum property is 
** parameter is non-zero, then auto-vacuum mode is enabled. If zero, it
** Change the 'auto-vacuum' property of the database. If the 'autoVacuum'
/*

}
  return b;
  sqlite3BtreeLeave(p);
  b = (p->pBt->btsFlags & BTS_FAST_SECURE)/BTS_SECURE_DELETE;
  }
    p->pBt->btsFlags |= BTS_SECURE_DELETE*newFlag;
    p->pBt->btsFlags &= ~BTS_FAST_SECURE;
  if( newFlag>=0 ){
  assert( BTS_FAST_SECURE==(BTS_OVERWRITE|BTS_SECURE_DELETE) );
  assert( BTS_OVERWRITE==BTS_SECURE_DELETE*2 );
  sqlite3BtreeEnter(p);
  if( p==0 ) return 0;
  int b;
int sqlite3BtreeSecureDelete(Btree *p, int newFlag){
*/
** the amount of disk I/O.
** that freelist leaf pages are written back into the database, increasing
** With BTS_SECURE_DELETE, operation is like BTS_OVERWRITE with the addition
**
** deleted content is cleared, but freelist deleted content is not.
** freelist leaf pages are not written back to the database.  Thus in-page
** With BTS_OVERWRITE set, deleted content is overwritten by zeros, but
**
** This routine acts as a query if newFlag is less than zero
**
**    newFlag==(-1)    No changes
**    newFlag==2       BTS_SECURE_DELETE cleared and BTS_OVERWRITE is set
**    newFlag==1       BTS_SECURE_DELETE set and BTS_OVERWRITE is cleared
**    newFlag==0       Both BTS_SECURE_DELETE and BTS_OVERWRITE are cleared
**
** Change the values for the BTS_SECURE_DELETE and BTS_OVERWRITE flags:
/*

}
  return n;
  sqlite3BtreeLeave(p);
  n = sqlite3PagerMaxPageCount(p->pBt->pPager, mxPage);
  sqlite3BtreeEnter(p);
  int n;
int sqlite3BtreeMaxPageCount(Btree *p, int mxPage){
*/
** Regardless of the value of mxPage, return the maximum page count.
** No changes are made if mxPage is 0 or negative.
** Set the maximum page count for a database if mxPage is positive.
/*


}
  return n;
  sqlite3BtreeLeave(p);
  n = sqlite3BtreeGetReserveNoMutex(p);
  sqlite3BtreeEnter(p);
  int n;
int sqlite3BtreeGetOptimalReserve(Btree *p){
*/
** reserve space.
** greater of the current reserved space and the maximum requested
** If SQLITE_HAS_MUTEX is defined then the number returned is the
**
** sometimes used by extensions.
** are intentually left unused.  This is the "reserved" space that is
** Return the number of bytes of space at the end of every page that
/*

}
  return n;
  n = p->pBt->pageSize - p->pBt->usableSize;
  assert( sqlite3_mutex_held(p->pBt->mutex) );
  int n;
int sqlite3BtreeGetReserveNoMutex(Btree *p){
*/
** database handle that owns *p, causing undefined behavior.
** were to be called, it might collide with some other operation on the
** database handle that owns *p is not. In this case if sqlite3BtreeEnter()
** known that the shared b-tree mutex is held, but the mutex on the 
** This is useful in one special case in the backup API code where it is
**
** held.
** may only be called if it is guaranteed that the b-tree mutex is already
** This function is similar to sqlite3BtreeGetReserve(), except that it
/*

}
  return p->pBt->pageSize;
int sqlite3BtreeGetPageSize(Btree *p){
*/
** Return the currently defined page size
/*

}
  return rc;
  sqlite3BtreeLeave(p);
  if( iFix ) pBt->btsFlags |= BTS_PAGESIZE_FIXED;
  pBt->usableSize = pBt->pageSize - (u16)nReserve;
  rc = sqlite3PagerSetPagesize(pBt->pPager, &pBt->pageSize, nReserve);
  }
    freeTempSpace(pBt);
    pBt->pageSize = (u32)pageSize;
    assert( !pBt->pCursor );
    assert( (pageSize & 7)==0 );
        ((pageSize-1)&pageSize)==0 ){
  if( pageSize>=512 && pageSize<=SQLITE_MAX_PAGE_SIZE &&
  assert( nReserve>=0 && nReserve<=255 );
  }
    nReserve = pBt->pageSize - pBt->usableSize;
  if( nReserve<0 ){
  }
    return SQLITE_READONLY;
    sqlite3BtreeLeave(p);
  if( pBt->btsFlags & BTS_PAGESIZE_FIXED ){
  sqlite3BtreeEnter(p);
  assert( nReserve>=-1 && nReserve<=255 );
  BtShared *pBt = p->pBt;
  int rc = SQLITE_OK;
int sqlite3BtreeSetPageSize(Btree *p, int pageSize, int nReserve, int iFix){
*/
** and autovacuum mode can no longer be changed.
** If the iFix!=0 then the BTS_PAGESIZE_FIXED flag is set so that the page size
**
** bytes per page is left unchanged.
** If parameter nReserve is less than zero, then the number of reserved
**
** at the beginning of a page.
** the first byte past the 1GB boundary, 0x40000000) needs to occur
** of the database file used for locking (beginning at PENDING_BYTE,
** Page sizes are constrained to be a power of two so that the region
**
** changed.
** size supplied does not meet this constraint then the page size is not
** The page size must be a power of 2 between 512 and 65536.  If the page
**
** without changing anything.
** Or, if the page size has already been fixed, return SQLITE_READONLY 
** Change the default pages size and the number of reserved bytes per page.
/*

#endif
}
  return SQLITE_OK;
  sqlite3BtreeLeave(p);
  sqlite3PagerSetFlags(pBt->pPager, pgFlags);
  sqlite3BtreeEnter(p);
  assert( sqlite3_mutex_held(p->db->mutex) );
  BtShared *pBt = p->pBt;
){
  unsigned pgFlags       /* Various PAGER_* flags */
  Btree *p,              /* The btree to set the safety level on */
int sqlite3BtreeSetPagerFlags(
#ifndef SQLITE_OMIT_PAGER_PRAGMAS
*/
** probability of damage to near zero but with a write performance reduction.
** is a very low but non-zero probability of damage.  Level 3 reduces the
** there is a high probability of damage)  Level 2 is the default.  There
** failures.  Level 1 is the same as asynchronous (no syncs() occur and
** how well the database resists damage due to OS crashes and power
** Change the way data is synced to disk in order to increase or decrease
/*

#endif /* SQLITE_MAX_MMAP_SIZE>0 */
}
  return SQLITE_OK;
  sqlite3BtreeLeave(p);
  sqlite3PagerSetMmapLimit(pBt->pPager, szMmap);
  sqlite3BtreeEnter(p);
  assert( sqlite3_mutex_held(p->db->mutex) );
  BtShared *pBt = p->pBt;
int sqlite3BtreeSetMmapLimit(Btree *p, sqlite3_int64 szMmap){
*/
** memory mapped.
** Change the limit on the amount of the database file that may be
/*
#if SQLITE_MAX_MMAP_SIZE>0

}
  return res;
  sqlite3BtreeLeave(p);
  res = sqlite3PagerSetSpillsize(pBt->pPager, mxPage);
  sqlite3BtreeEnter(p);
  assert( sqlite3_mutex_held(p->db->mutex) );
  int res;
  BtShared *pBt = p->pBt;
int sqlite3BtreeSetSpillSize(Btree *p, int mxPage){
*/
** using mxPage of 0 is a way to query the current spill size.
** as an argument, no changes are made to the spill size setting, so
** The value returned is the current spill size.  If zero is passed
**
** order to free up memory.
** the pager might attempt to "spill" pages to the journal early in
** If the number of pages exceeds this limit during a write transaction,
** Change the "spill" limit on the number of pages in the cache.
/*

}
  return SQLITE_OK;
  sqlite3BtreeLeave(p);
  sqlite3PagerSetCachesize(pBt->pPager, mxPage);
  sqlite3BtreeEnter(p);
  assert( sqlite3_mutex_held(p->db->mutex) );
  BtShared *pBt = p->pBt;
int sqlite3BtreeSetCacheSize(Btree *p, int mxPage){
*/
** dirty pages or pages still in active use.
** cache is allowed to grow larger than this limit if it contains
** pages in the cache exceeds this soft limit.  But the size of the
** Unused and unmodified pages will be recycled when the number of
** Change the "soft" limit on the number of pages in the cache.
/*

}
  return SQLITE_OK;
  sqlite3_free(p);

#endif
  if( p->pNext ) p->pNext->pPrev = p->pPrev;
  if( p->pPrev ) p->pPrev->pNext = p->pNext;
  assert( p->locked==0 );
  assert( p->wantToLock==0 );
#ifndef SQLITE_OMIT_SHARED_CACHE

  }
    sqlite3_free(pBt);
    freeTempSpace(pBt);
    sqlite3DbFree(0, pBt->pSchema);
    }
      pBt->xFreeSchema(pBt->pSchema);
    if( pBt->xFreeSchema && pBt->pSchema ){
    sqlite3PagerClose(pBt->pPager, p->db);
    assert( !pBt->pCursor );
    */
    ** Clean out and delete the BtShared object.
    **
    ** it without having to hold the mutex.
    /* The pBt is no longer on the sharing list, so we can access
  if( !p->sharable || removeFromSharingList(pBt) ){
  assert( p->wantToLock==0 && p->locked==0 );
  */
  ** up the shared-btree.
  ** structure, return now. The remainder of this procedure cleans 
  /* If there are still other outstanding references to the shared-btree

  sqlite3BtreeLeave(p);
  sqlite3BtreeRollback(p, SQLITE_OK, 0);
  */
  ** this handle.
  ** The call to sqlite3BtreeRollback() drops any table-locks held by
  /* Rollback any active transaction and free the handle structure.

  }
    }
      sqlite3BtreeCloseCursor(pTmp);
    if( pTmp->pBtree==p ){
    pCur = pCur->pNext;
    BtCursor *pTmp = pCur;
  while( pCur ){
  pCur = pBt->pCursor;
  sqlite3BtreeEnter(p);
  assert( sqlite3_mutex_held(p->db->mutex) );
  /* Close all cursors opened via this handle.  */

  BtCursor *pCur;
  BtShared *pBt = p->pBt;
int sqlite3BtreeClose(Btree *p){
*/
** Close an open database and invalidate all cursors.
/*

}
  }
    pBt->pTmpSpace = 0;
    sqlite3PageFree(pBt->pTmpSpace);
    pBt->pTmpSpace -= 4;
  if( pBt->pTmpSpace ){
static void freeTempSpace(BtShared *pBt){
*/
** Free the pBt->pTmpSpace allocation
/*

}
  }
    }
      pBt->pTmpSpace += 4;
      memset(pBt->pTmpSpace, 0, 8);
    if( pBt->pTmpSpace ){
    */
    ** left-child pointer to the beginning of a cell.
    ** beginning of pTmpSpace as an area available to prepend the
    ** Also:  Provide four bytes of initialized space before the
    **
    ** zero the first 4 bytes of temp space here.
    ** data is passed to system call write(). So to avoid this error,
    ** does cause a valgrind error when the 1 or 2 bytes of unitialized 
    ** it into a database page. This is not actually a problem, but it
    ** bytes of pTmpSpace, but that the first 4 bytes are copied from
    ** can mean that fillInCell() only initializes the first 2 or 3
    ** by the various routines that manipulate binary cells. Which
    ** a cell is less than 4 bytes in size, it is rounded up to 4 bytes
    ** inserting them into a leaf page (function fillInCell()). If
    /* One of the uses of pBt->pTmpSpace is to format cells before

    pBt->pTmpSpace = sqlite3PageMalloc( pBt->pageSize );
  if( !pBt->pTmpSpace ){
static void allocateTempSpace(BtShared *pBt){
*/
** pointer.
** MX_CELL_SIZE(pBt) bytes with a 4-byte prefix for a left-child
** Make sure pBt->pTmpSpace points to an allocation of 
/*

}
#endif
  return 1;
#else
  return removed;
  sqlite3_mutex_leave(pMaster);
  }
    removed = 1;
    }
      sqlite3_mutex_free(pBt->mutex);
    if( SQLITE_THREADSAFE ){
    }
      }
        pList->pNext = pBt->pNext;
      if( ALWAYS(pList) ){
      }
        pList=pList->pNext;
      while( ALWAYS(pList) && pList->pNext!=pBt ){
      pList = GLOBAL(BtShared*,sqlite3SharedCacheList);
    }else{
      GLOBAL(BtShared*,sqlite3SharedCacheList) = pBt->pNext;
    if( GLOBAL(BtShared*,sqlite3SharedCacheList)==pBt ){
  if( pBt->nRef<=0 ){
  pBt->nRef--;
  sqlite3_mutex_enter(pMaster);
  MUTEX_LOGIC( pMaster = sqlite3MutexAlloc(SQLITE_MUTEX_STATIC_MASTER); )
  assert( sqlite3_mutex_notheld(pBt->mutex) );

  int removed = 0;
  BtShared *pList;
  MUTEX_LOGIC( sqlite3_mutex *pMaster; )
#ifndef SQLITE_OMIT_SHARED_CACHE
static int removeFromSharingList(BtShared *pBt){
*/
** false if it is still positive.
** true if the BtShared.nRef counter reaches zero and return
** remove the BtShared structure from the sharing list.  Return
** Decrement the BtShared.nRef counter.  When it reaches zero,
/*

}
  return rc;
  assert( rc!=SQLITE_OK || sqlite3BtreeConnectionCount(*ppBtree)>0 );
  }
    sqlite3_mutex_leave(mutexOpen);
    assert( sqlite3_mutex_held(mutexOpen) );
  if( mutexOpen ){
  }
    }
      sqlite3OsFileControlHint(pFile, SQLITE_FCNTL_PDB, (void*)&pBt->db);
    if( pFile->pMethods ){
    pFile = sqlite3PagerFile(pBt->pPager);

    }
      sqlite3PagerSetCachesize(p->pBt->pPager, SQLITE_DEFAULT_CACHE_SIZE);
    if( sqlite3BtreeSchema(p, 0, 0)==0 ){
    */
    ** do not change the pager-cache size.
    ** default value. Except, when opening on an existing shared pager-cache,
    /* If the B-Tree was successfully opened, set the pager-cache size to the

    sqlite3_file *pFile;
  }else{
    *ppBtree = 0;
    sqlite3_free(p);
    sqlite3_free(pBt);
    }
      sqlite3PagerClose(pBt->pPager, 0);
    if( pBt && pBt->pPager ){
  if( rc!=SQLITE_OK ){
btree_open_out:

  *ppBtree = p;
#endif
  }
    }
      }
        break;
        }
          pSib->pNext = p;
          }
            p->pNext->pPrev = p;
          if( p->pNext ){
          p->pPrev = pSib;
          p->pNext = pSib->pNext;
          }
            pSib = pSib->pNext;
          while( pSib->pNext && (uptr)pSib->pNext->pBt<(uptr)p->pBt ){
        }else{
          pSib->pPrev = p;
          p->pPrev = 0;
          p->pNext = pSib;
        if( (uptr)p->pBt<(uptr)pSib->pBt ){
        while( pSib->pPrev ){ pSib = pSib->pPrev; }
      if( (pSib = db->aDb[i].pBt)!=0 && pSib->sharable ){
    for(i=0; i<db->nDb; i++){
    Btree *pSib;
    int i;
  if( p->sharable ){
  */
  ** The list is kept in ascending order by pBt address.
  ** Btree into the list of all sharable Btrees for the same connection.
  /* If the new Btree uses a sharable pBtShared, then link the new
#if !defined(SQLITE_OMIT_SHARED_CACHE) && !defined(SQLITE_OMIT_DISKIO)

  }
#endif
    }
      sqlite3_mutex_leave(mutexShared);
      GLOBAL(BtShared*,sqlite3SharedCacheList) = pBt;
      pBt->pNext = GLOBAL(BtShared*,sqlite3SharedCacheList);
      sqlite3_mutex_enter(mutexShared);
      }
        }
          goto btree_open_out;
          rc = SQLITE_NOMEM_BKPT;
        if( pBt->mutex==0 ){
        pBt->mutex = sqlite3MutexAlloc(SQLITE_MUTEX_FAST);
      if( SQLITE_THREADSAFE && sqlite3GlobalConfig.bCoreMutex ){
      MUTEX_LOGIC( mutexShared = sqlite3MutexAlloc(SQLITE_MUTEX_STATIC_MASTER);)
      MUTEX_LOGIC( sqlite3_mutex *mutexShared; )
    if( p->sharable ){
    pBt->nRef = 1;
    */
    /* Add the new BtShared object to the linked list sharable BtShareds.
#if !defined(SQLITE_OMIT_SHARED_CACHE) && !defined(SQLITE_OMIT_DISKIO)
   
    assert( (pBt->pageSize & 7)==0 );  /* 8-byte alignment of pageSize */
    pBt->usableSize = pBt->pageSize - nReserve;
    if( rc ) goto btree_open_out;
    rc = sqlite3PagerSetPagesize(pBt->pPager, &pBt->pageSize, nReserve);
    }
#endif
      pBt->incrVacuum = (get4byte(&zDbHeader[36 + 7*4])?1:0);
      pBt->autoVacuum = (get4byte(&zDbHeader[36 + 4*4])?1:0);
#ifndef SQLITE_OMIT_AUTOVACUUM
      pBt->btsFlags |= BTS_PAGESIZE_FIXED;
      nReserve = zDbHeader[20];
      ** into the database file header. */
      ** determined by the one-byte unsigned integer found at an offset of 20
      /* EVIDENCE-OF: R-37497-42412 The size of the reserved region is
    }else{
      nReserve = 0;
#endif
      }
        pBt->incrVacuum = (SQLITE_DEFAULT_AUTOVACUUM==2 ? 1 : 0);
        pBt->autoVacuum = (SQLITE_DEFAULT_AUTOVACUUM ? 1 : 0);
      if( zFilename && !isMemdb ){
      */
      ** regular file-name. In this case the auto-vacuum applies as per normal.
      ** SQLITE_OMIT_MEMORYDB has been defined, then ":memory:" is just a
      ** SQLITE_DEFAULT_AUTOVACUUM is true. On the other hand, if
      ** leave the autoVacuum mode at 0 (do not auto-vacuum), even if
      /* If the magic name ":memory:" will create an in-memory database, then
#ifndef SQLITE_OMIT_AUTOVACUUM
      pBt->pageSize = 0;
         || ((pBt->pageSize-1)&pBt->pageSize)!=0 ){
    if( pBt->pageSize<512 || pBt->pageSize>SQLITE_MAX_PAGE_SIZE
    pBt->pageSize = (zDbHeader[16]<<8) | (zDbHeader[17]<<16);
    ** the beginning of the database file. */
    ** determined by the 2-byte integer located at an offset of 16 bytes from
    /* EVIDENCE-OF: R-51873-39618 The page size for a database file is
#endif
    pBt->btsFlags |= BTS_OVERWRITE;
#elif defined(SQLITE_FAST_SECURE_DELETE)
    pBt->btsFlags |= BTS_SECURE_DELETE;
#if defined(SQLITE_SECURE_DELETE)
    if( sqlite3PagerIsreadonly(pBt->pPager) ) pBt->btsFlags |= BTS_READ_ONLY;
    pBt->pPage1 = 0;
    pBt->pCursor = 0;
  
    p->pBt = pBt;
    sqlite3PagerSetBusyHandler(pBt->pPager, btreeInvokeBusyHandler, pBt);
    pBt->db = db;
    pBt->openFlags = (u8)flags;
    }
      goto btree_open_out;
    if( rc!=SQLITE_OK ){
    }
      rc = sqlite3PagerReadFileheader(pBt->pPager,sizeof(zDbHeader),zDbHeader);
      sqlite3PagerSetMmapLimit(pBt->pPager, db->szMmap);
    if( rc==SQLITE_OK ){
                          sizeof(MemPage), flags, vfsFlags, pageReinit);
    rc = sqlite3PagerOpen(pVfs, &pBt->pPager, zFilename,
    }
      goto btree_open_out;
      rc = SQLITE_NOMEM_BKPT;
    if( pBt==0 ){
    pBt = sqlite3MallocZero( sizeof(*pBt) );
  
    assert( sizeof(Pgno)==4 );
    assert( sizeof(u16)==2 );
    assert( sizeof(u32)==4 );
    assert( sizeof(u64)==8 );
    assert( sizeof(i64)==8 );
    */
    ** when compiling on a different architecture.
    ** the right size.  This is to guard against size changes that result
    ** The following asserts make sure that structures used by the btree are
    /*
  if( pBt==0 ){
#endif
  }
#endif
    }
      p->sharable = 1;
      */
      ** statements to find locking problems.
      ** gives more opportunity for asserts(sqlite3_mutex_held())
      ** even when they are not.  This exercises the locking code and
      /* In debug mode, we mark all persistent databases as sharable
    else{
#ifdef SQLITE_DEBUG
    }
      sqlite3_free(zFullPathname);
      sqlite3_mutex_leave(mutexShared);
      }
        }
          break;
          pBt->nRef++;
          p->pBt = pBt;
          }
            }
              return SQLITE_CONSTRAINT;
              sqlite3_free(p);
              sqlite3_free(zFullPathname);
              sqlite3_mutex_leave(mutexOpen);
              sqlite3_mutex_leave(mutexShared);
            if( pExisting && pExisting->pBt==pBt ){
            Btree *pExisting = db->aDb[iDb].pBt;
          for(iDb=db->nDb-1; iDb>=0; iDb--){
          int iDb;
                 && sqlite3PagerVfs(pBt->pPager)==pVfs ){
        if( 0==strcmp(zFullPathname, sqlite3PagerFilename(pBt->pPager, 0))
        assert( pBt->nRef>0 );
      for(pBt=GLOBAL(BtShared*,sqlite3SharedCacheList); pBt; pBt=pBt->pNext){
#endif
      sqlite3_mutex_enter(mutexShared);
      mutexShared = sqlite3MutexAlloc(SQLITE_MUTEX_STATIC_MASTER);
      sqlite3_mutex_enter(mutexOpen);
      mutexOpen = sqlite3MutexAlloc(SQLITE_MUTEX_STATIC_OPEN);
#if SQLITE_THREADSAFE
      }
        }
          }
            return rc;
            sqlite3_free(p);
            sqlite3_free(zFullPathname);
          }else{
            rc = SQLITE_OK;
          if( rc==SQLITE_OK_SYMLINK ){
        if( rc ){
                                   nFullPathname, zFullPathname);
        rc = sqlite3OsFullPathname(pVfs, zFilename,
      }else{
        memcpy(zFullPathname, zFilename, nFilename);
      if( isMemdb ){
      }
        return SQLITE_NOMEM_BKPT;
        sqlite3_free(p);
      if( !zFullPathname ){
      p->sharable = 1;

      MUTEX_LOGIC( sqlite3_mutex *mutexShared; )
      char *zFullPathname = sqlite3Malloc(MAX(nFullPathname,nFilename));
      int nFullPathname = pVfs->mxPathname+1;
      int nFilename = sqlite3Strlen30(zFilename)+1;
    if( vfsFlags & SQLITE_OPEN_SHAREDCACHE ){
  if( isTempDb==0 && (isMemdb==0 || (vfsFlags&SQLITE_OPEN_URI)!=0) ){
  */
  ** existing BtShared object that we can share with
  ** If this Btree is a candidate for shared cache, try to find an
  /*
#if !defined(SQLITE_OMIT_SHARED_CACHE) && !defined(SQLITE_OMIT_DISKIO)

#endif
  p->lock.iTable = 1;
  p->lock.pBtree = p;
#ifndef SQLITE_OMIT_SHARED_CACHE
  p->db = db;
  p->inTrans = TRANS_NONE;
  }
    return SQLITE_NOMEM_BKPT;
  if( !p ){
  p = sqlite3MallocZero(sizeof(Btree));
  }
    vfsFlags = (vfsFlags & ~SQLITE_OPEN_MAIN_DB) | SQLITE_OPEN_TEMP_DB;
  if( (vfsFlags & SQLITE_OPEN_MAIN_DB)!=0 && (isMemdb || isTempDb) ){
  }
    flags |= BTREE_MEMORY;
  if( isMemdb ){

  assert( (flags & BTREE_SINGLE)==0 || isTempDb );
  /* A BTREE_SINGLE database is always a temporary and/or ephemeral */

  assert( (flags & BTREE_UNORDERED)==0 || (flags & BTREE_SINGLE)!=0 );
  /* Only a BTREE_SINGLE database can be BTREE_UNORDERED */

  assert( (flags&0xff)==flags );   /* flags fit in 8 bits */
  assert( sqlite3_mutex_held(db->mutex) );
  assert( pVfs!=0 );
  assert( db!=0 );

#endif
                       || (vfsFlags & SQLITE_OPEN_MEMORY)!=0;
                       || (isTempDb && sqlite3TempInMemory(db))
  const int isMemdb = (zFilename && strcmp(zFilename, ":memory:")==0)
#else
  const int isMemdb = 0;
#ifdef SQLITE_OMIT_MEMORYDB
  */
  ** false for a file-based database.
  /* Set the variable isMemdb to true for an in-memory database, or 

  const int isTempDb = zFilename==0 || zFilename[0]==0;
  /* True if opening an ephemeral, temporary database */

  unsigned char zDbHeader[100];  /* Database header content */
  u8 nReserve;                   /* Byte of unused space on each page */
  int rc = SQLITE_OK;            /* Result code from this function */
  sqlite3_mutex *mutexOpen = 0;  /* Prevents a race condition. Ticket #3537 */
  Btree *p;                      /* Handle to return */
  BtShared *pBt = 0;             /* Shared part of btree structure */
){
  int vfsFlags            /* Flags passed through to sqlite3_vfs.xOpen() */
  int flags,              /* Options */
  Btree **ppBtree,        /* Pointer to new Btree object written here */
  sqlite3 *db,            /* Associated database handle */
  const char *zFilename,  /* Name of the file containing the BTree database */
  sqlite3_vfs *pVfs,      /* VFS to use for this b-tree */
int sqlite3BtreeOpen(
*/
** to problems with locking.
** objects in the same database connection since doing so will lead
** SQLITE_CONSTRAINT error.  We cannot allow two or more BtShared
** and we are in shared cache mode, then the open will fail with an
** If the database is already opened in the same database connection
**
** BTREE_OMIT_JOURNAL and/or BTREE_MEMORY.
** The "flags" parameter is a bitmask that might contain bits like
**
** that is automatically destroyed when it is closed.
** If zFilename is ":memory:" then an in-memory database is created
**
** when sqlite3BtreeClose() is called.
** Either way, the ephemeral database will be automatically deleted 
** be exclusively in memory, or it might use a disk-based memory cache.
** then an ephemeral database is created.  The ephemeral database might
** zFilename is the name of the database file.  If zFilename is NULL
** 
** Open a database file.
/*

}
                                  sqlite3PagerFile(pBt->pPager));
  return sqlite3InvokeBusyHandler(&pBt->db->busyHandler,
  assert( sqlite3_mutex_held(pBt->db->mutex) );
  assert( pBt->db );
  BtShared *pBt = (BtShared*)pArg;
static int btreeInvokeBusyHandler(void *pArg){
*/
** Invoke the busy handler for a btree.
/*

}
  }
    }
      btreeInitPage(pPage);
      ** the call for every page that comes in for re-initing. */
      ** btreeInitPage() be called on every btree page so we make
      ** But no harm is done by this.  And it is very important that
      ** call to btreeInitPage() will likely return SQLITE_CORRUPT.
      ** or ptrmap page or a free page.  In those cases, the following
      /* pPage might not be a btree page;  it might be an overflow page
    if( sqlite3PagerPageRefcount(pData)>1 ){
    pPage->isInit = 0;
    assert( sqlite3_mutex_held(pPage->pBt->mutex) );
  if( pPage->isInit ){
  assert( sqlite3PagerPageRefcount(pData)>0 );
  pPage = (MemPage *)sqlite3PagerGetExtra(pData);
  MemPage *pPage;
static void pageReinit(DbPage *pData){
*/
** page to agree with the restored data.
** This routine needs to reset the extra data section at the end of the
**
** the transaction, for each page restored this routine is called.
** so that the cache is restored to its original state at the start of
** During a rollback, when the pager reloads information into the cache
/*


}
  return rc;
  }
    *ppPage = 0;
  }else{
    (*ppPage)->isInit = 0;
    }
      return SQLITE_CORRUPT_BKPT;
      *ppPage = 0;
      releasePage(*ppPage);
    if( sqlite3PagerPageRefcount((*ppPage)->pDbPage)>1 ){
  if( rc==SQLITE_OK ){
  int rc = btreeGetPage(pBt, pgno, ppPage, flags);
){
  int flags            /* PAGER_GET_NOCONTENT or PAGER_GET_READONLY */
  MemPage **ppPage,    /* Return the page in this parameter */
  Pgno pgno,           /* Number of the page to fetch */
  BtShared *pBt,       /* The btree */
static int btreeGetUnusedPage(
*/
**   *  Make sure the isInit flag is clear
**      release it and return an SQLITE_CURRUPT error.
**   *  If the page is already in use for some other purpose, immediately
**
** This works just like btreeGetPage() with the addition:
**
** Get an unused page.
/*

}
  sqlite3PagerUnrefPageOne(pPage->pDbPage);
  assert( sqlite3_mutex_held(pPage->pBt->mutex) );
  assert( sqlite3PagerGetData(pPage->pDbPage)==pPage->aData );
  assert( sqlite3PagerGetExtra(pPage->pDbPage) == (void*)pPage );
  assert( pPage->pDbPage!=0 );
  assert( pPage->pBt );
  assert( pPage->aData );
  assert( pPage!=0 );
static void releasePageOne(MemPage *pPage){
}
  if( pPage ) releasePageNotNull(pPage);
static void releasePage(MemPage *pPage){
}
  sqlite3PagerUnrefNotNull(pPage->pDbPage);
  assert( sqlite3_mutex_held(pPage->pBt->mutex) );
  assert( sqlite3PagerGetData(pPage->pDbPage)==pPage->aData );
  assert( sqlite3PagerGetExtra(pPage->pDbPage) == (void*)pPage );
  assert( pPage->pDbPage!=0 );
  assert( pPage->pBt );
  assert( pPage->aData );
static void releasePageNotNull(MemPage *pPage){
*/
** Page1 is a special case and must be released using releasePageOne().
**
** call to btreeGetPage.
** Release a MemPage.  This should be called once for each prior
/*

}
  return rc;
  assert( pgno!=0 || rc==SQLITE_CORRUPT );
  testcase( pgno==0 );
  }
    pCur->pPage = pCur->apPage[pCur->iPage];
    pCur->iPage--;
  if( pCur ){
getAndInitPage_error1:
  releasePage(*ppPage);
getAndInitPage_error2:

  return SQLITE_OK;
  }
    goto getAndInitPage_error2;
    rc = SQLITE_CORRUPT_PGNO(pgno);
  if( pCur && ((*ppPage)->nCell<1 || (*ppPage)->intKey!=pCur->curIntKey) ){
  ** compatible with the root page. */
  /* If obtaining a child page for a cursor, we must verify that the page is

  assert( (*ppPage)->aData==sqlite3PagerGetData(pDbPage) );
  assert( (*ppPage)->pgno==pgno );
  }
    }
      goto getAndInitPage_error2;
    if( rc!=SQLITE_OK ){
    rc = btreeInitPage(*ppPage);
    btreePageFromDbPage(pDbPage, pgno, pBt);
  if( (*ppPage)->isInit==0 ){
  *ppPage = (MemPage*)sqlite3PagerGetExtra(pDbPage);
  }
    goto getAndInitPage_error1;
  if( rc ){
  rc = sqlite3PagerGet(pBt->pPager, pgno, (DbPage**)&pDbPage, bReadOnly);
  }
    goto getAndInitPage_error1;
    rc = SQLITE_CORRUPT_BKPT;
  if( pgno>btreePagecount(pBt) ){

  assert( pCur==0 || pCur->iPage>0 );
  assert( pCur==0 || bReadOnly==pCur->curPagerFlags );
  assert( pCur==0 || ppPage==&pCur->pPage );
  assert( sqlite3_mutex_held(pBt->mutex) );
  DbPage *pDbPage;
  int rc;
){
  int bReadOnly                   /* True for a read-only page */
  BtCursor *pCur,                 /* Cursor to receive the page, or NULL */
  MemPage **ppPage,               /* Write the page pointer here */
  Pgno pgno,                      /* Number of the page to get */
  BtShared *pBt,                  /* The database file */
static int getAndInitPage(
*/
** may remain unchanged, or it may be set to an invalid value.
** If an error occurs, then *ppPage is undefined. It
**
** a read-only cursor.
** The page is fetched as read-write unless pCur is not NULL and is
**
** And if the fetch fails, this routine must decrement pCur->iPage.
** call.  Do additional sanity checking on the page in this case.
** If pCur!=0 then the page is being fetched as part of a moveToChild()
**
** Get a page from the pager and initialize it.
/*

}
  return btreePagecount(p->pBt) & 0x7fffffff;
  assert( sqlite3BtreeHoldsMutex(p) );
u32 sqlite3BtreeLastPage(Btree *p){
}
  return pBt->nPage;
  assert( (pBt->nPage & 0x80000000)==0 || CORRUPT_DB );
static Pgno btreePagecount(BtShared *pBt){
*/
** error, return ((unsigned int)-1).
** Return the size of the database file in pages. If there is any kind of
/*

}
  return 0;
  }
    return btreePageFromDbPage(pDbPage, pgno, pBt);
  if( pDbPage ){
  pDbPage = sqlite3PagerLookup(pBt->pPager, pgno);
  assert( sqlite3_mutex_held(pBt->mutex) );
  DbPage *pDbPage;
static MemPage *btreePageLookup(BtShared *pBt, Pgno pgno){
*/
** MemPage.aData elements if needed.
** already in the pager cache return NULL. Initialize the MemPage.pBt and
** Retrieve a page from the pager cache. If the requested page is not
/*

}
  return SQLITE_OK;
  *ppPage = btreePageFromDbPage(pDbPage, pgno, pBt);
  if( rc ) return rc;
  rc = sqlite3PagerGet(pBt->pPager, pgno, (DbPage**)&pDbPage, flags);
  assert( sqlite3_mutex_held(pBt->mutex) );
  assert( flags==0 || flags==PAGER_GET_NOCONTENT || flags==PAGER_GET_READONLY );

  DbPage *pDbPage;
  int rc;
){
  int flags            /* PAGER_GET_NOCONTENT or PAGER_GET_READONLY */
  MemPage **ppPage,    /* Return the page in this parameter */
  Pgno pgno,           /* Number of the page to fetch */
  BtShared *pBt,       /* The btree */
static int btreeGetPage(
*/
** read should occur at that point.
** means we have started to be concerned about content and the disk
** If in the future we call sqlite3PagerWrite() on this page, that
** to fetch the content.  Just fill in the content with zeros for now.
** about the content of the page at this time.  So do not go to the disk
** If the PAGER_GET_NOCONTENT flag is set, it means that we do not care
**
** MemPage.aData elements if needed.  See also: btreeGetUnusedPage().
** Get a page from the pager.  Initialize the MemPage.pBt and
/*

}
  return pPage; 
  assert( pPage->aData==sqlite3PagerGetData(pDbPage) );
  }
    pPage->hdrOffset = pgno==1 ? 100 : 0;
    pPage->pgno = pgno;
    pPage->pBt = pBt;
    pPage->pDbPage = pDbPage;
    pPage->aData = sqlite3PagerGetData(pDbPage);
  if( pgno!=pPage->pgno ){
  MemPage *pPage = (MemPage*)sqlite3PagerGetExtra(pDbPage);
static MemPage *btreePageFromDbPage(DbPage *pDbPage, Pgno pgno, BtShared *pBt){
*/
** the btree layer.
** Convert a DbPage obtained from the pager into a MemPage used by
/*


}
  pPage->isInit = 1;
  pPage->nCell = 0;
  pPage->maskPage = (u16)(pBt->pageSize - 1);
  assert( pBt->pageSize>=512 && pBt->pageSize<=65536 );
  pPage->nOverflow = 0;
  pPage->aDataOfst = &data[pPage->childPtrSize];
  pPage->aCellIdx = &data[first];
  pPage->aDataEnd = &data[pBt->usableSize];
  pPage->cellOffset = first;
  decodeFlags(pPage, flags);
  pPage->nFree = (u16)(pBt->usableSize - first);
  put2byte(&data[hdr+5], pBt->usableSize);
  data[hdr+7] = 0;
  memset(&data[hdr+1], 0, 4);
  first = hdr + ((flags&PTF_LEAF)==0 ? 12 : 8);
  data[hdr] = (char)flags;
  }
    memset(&data[hdr], 0, pBt->usableSize - hdr);
  if( pBt->btsFlags & BTS_FAST_SECURE ){
  assert( sqlite3_mutex_held(pBt->mutex) );
  assert( sqlite3PagerIswriteable(pPage->pDbPage) );
  assert( sqlite3PagerGetData(pPage->pDbPage) == data );
  assert( sqlite3PagerGetExtra(pPage->pDbPage) == (void*)pPage );
  assert( sqlite3PagerPagenumber(pPage->pDbPage)==pPage->pgno );

  u16 first;
  u8 hdr = pPage->hdrOffset;
  BtShared *pBt = pPage->pBt;
  unsigned char *data = pPage->aData;
static void zeroPage(MemPage *pPage, int flags){
*/
** no entries.
** Set up a raw page so that it looks like a database page holding
/*

}
  return SQLITE_OK;
  }
    return btreeCellSizeCheck(pPage);
  if( pBt->db->flags & SQLITE_CellSizeCk ){
  pPage->isInit = 1;
  pPage->nFree = -1;  /* Indicate that this value is yet uncomputed */
       || CORRUPT_DB );
       || get2byteNotZero(&data[5])==(int)pBt->usableSize
  assert( pPage->nCell>0
  ** bytes of reserved space. */
  ** offset to the cell content area will equal the page size minus the
  ** possible for a root page of a table that contains no rows) then the
  /* EVIDENCE-OF: R-24089-57979 If a page contains no cells (which is only
  testcase( pPage->nCell==MX_CELL(pBt) );
  }
    return SQLITE_CORRUPT_PAGE(pPage);
    /* To many cells for a single page.  The page must be corrupt */
  if( pPage->nCell>MX_CELL(pBt) ){
  pPage->nCell = get2byte(&data[3]);
  ** number of cells on the page. */
  /* EVIDENCE-OF: R-37002-32774 The two-byte integer at offset 3 gives the
  pPage->aDataOfst = pPage->aData + pPage->childPtrSize;
  pPage->aDataEnd = pPage->aData + pBt->usableSize;
  pPage->aCellIdx = data + pPage->childPtrSize + 8;
  pPage->cellOffset = pPage->hdrOffset + 8 + pPage->childPtrSize;
  pPage->nOverflow = 0;
  pPage->maskPage = (u16)(pBt->pageSize - 1);
  assert( pBt->pageSize>=512 && pBt->pageSize<=65536 );
  }
    return SQLITE_CORRUPT_PAGE(pPage);
  if( decodeFlags(pPage, data[0]) ){
  ** the b-tree page type. */
  /* EVIDENCE-OF: R-28594-02890 The one-byte flag at offset 0 indicating
  data = pPage->aData + pPage->hdrOffset;
  pBt = pPage->pBt;

  assert( pPage->isInit==0 );
  assert( pPage->aData == sqlite3PagerGetData(pPage->pDbPage) );
  assert( pPage == sqlite3PagerGetExtra(pPage->pDbPage) );
  assert( pPage->pgno==sqlite3PagerPagenumber(pPage->pDbPage) );
  assert( sqlite3_mutex_held(pPage->pBt->mutex) );
  assert( pPage->pBt->db!=0 );
  assert( pPage->pBt!=0 );

  BtShared *pBt;        /* The main btree structure */
  u8 *data;          /* Equal to pPage->aData */
static int btreeInitPage(MemPage *pPage){
*/
** we failed to detect any corruption.
** guarantee that the page is well-formed.  It only shows that
** SQLITE_CORRUPT.  Note that a return of SQLITE_OK does not
** not contain a well-formed database page, then return 
** Return SQLITE_OK on success.  If we see that the page does
**
** Initialize the auxiliary information for a disk block.
/*

}
  return SQLITE_OK;
  }
    }
      return SQLITE_CORRUPT_PAGE(pPage);
    if( pc+sz>usableSize ){
    testcase( pc+sz==usableSize );
    sz = pPage->xCellSize(pPage, &data[pc]);
    }
      return SQLITE_CORRUPT_PAGE(pPage);
    if( pc<iCellFirst || pc>iCellLast ){
    testcase( pc==iCellLast );
    testcase( pc==iCellFirst );
    pc = get2byteAligned(&data[cellOffset+i*2]);
  for(i=0; i<pPage->nCell; i++){
  if( !pPage->leaf ) iCellLast--;
  cellOffset = pPage->cellOffset;
  data = pPage->aData;
  iCellLast = usableSize - 4;
  usableSize = pPage->pBt->usableSize;
  iCellFirst = pPage->cellOffset + 2*pPage->nCell;

  int cellOffset;    /* Start of cell content area */
  int usableSize;    /* Maximum usable space on the page */
  u8 *data;          /* Equal to pPage->aData */
  int pc;            /* Address of a freeblock within pPage->aData[] */
  int sz;            /* Size of a cell */
  int i;             /* Index into the cell pointer array */
  int iCellLast;     /* Last possible cell or freeblock offset */
  int iCellFirst;    /* First allowable cell or freeblock offset */
static SQLITE_NOINLINE int btreeCellSizeCheck(MemPage *pPage){
*/
** PRAGMA cell_size_check=ON 
** Do additional sanity check after btreeInitPage() if
/*

}
  return SQLITE_OK;
  pPage->nFree = (u16)(nFree - iCellFirst);
  }
    return SQLITE_CORRUPT_PAGE(pPage);
  if( nFree>usableSize || nFree<iCellFirst ){
  */
  ** area, according to the page header, lies within the page.
  ** serves to verify that the offset to the start of the cell-content
  ** of the page, then the page must be corrupted. This check also
  ** the cell-content area. If this is greater than the usable-size
  ** of the cell-content area plus the number of free bytes within
  /* At this point, nFree contains the sum of the offset to the start

  }
    }
      return SQLITE_CORRUPT_PAGE(pPage);
      /* Last freeblock extends past page end */
    if( pc+size>(unsigned int)usableSize ){
    }
      return SQLITE_CORRUPT_PAGE(pPage);
      /* Freeblock not in ascending order */
    if( next>0 ){
    }
      pc = next;
      if( next<=pc+size+3 ) break;
      nFree = nFree + size;
      size = get2byte(&data[pc+2]);
      next = get2byte(&data[pc]);
      }
        return SQLITE_CORRUPT_PAGE(pPage);
        /* Freeblock off the end of the page */
      if( pc>iCellLast ){
    while( 1 ){
    }
      return SQLITE_CORRUPT_PAGE(pPage); 
      */
      ** always be at least one cell before the first freeblock.
      /* EVIDENCE-OF: R-55530-52930 In a well-formed b-tree page, there will
    if( pc<top ){
    u32 next, size;
  if( pc>0 ){
  nFree = data[hdr+7] + top;  /* Init nFree to non-freeblock free space */
  pc = get2byte(&data[hdr+1]);
  ** freeblocks. */
  ** start of the first freeblock on the page, or is zero if there are no
  ** EVIDENCE-OF: R-23588-34450 The two-byte integer at offset 1 gives the
  /* Compute the total free space on the page

  iCellLast = usableSize - 4;
  iCellFirst = hdr + 8 + pPage->childPtrSize + 2*pPage->nCell;
  top = get2byteNotZero(&data[hdr+5]);
  ** interpreted as 65536. */
  ** the start of the cell content area. A zero value for this integer is
  /* EVIDENCE-OF: R-58015-48175 The two-byte integer at offset 5 designates
  data = pPage->aData;
  hdr = pPage->hdrOffset;
  usableSize = pPage->pBt->usableSize;

  assert( pPage->nFree<0 );
  assert( pPage->isInit==1 );
  assert( pPage->aData == sqlite3PagerGetData(pPage->pDbPage) );
  assert( pPage == sqlite3PagerGetExtra(pPage->pDbPage) );
  assert( pPage->pgno==sqlite3PagerPagenumber(pPage->pDbPage) );
  assert( sqlite3_mutex_held(pPage->pBt->mutex) );
  assert( pPage->pBt->db!=0 );
  assert( pPage->pBt!=0 );

  int iCellLast;     /* Last possible cell or freeblock offset */
  int iCellFirst;    /* First allowable cell or freeblock offset */
  int top;           /* First byte of the cell content area */
  int nFree;         /* Number of unused bytes on the page */
  int usableSize;    /* Amount of usable space on each page */
  u8 *data;          /* Equal to pPage->aData */
  u8 hdr;            /* Offset to beginning of page header */
  int pc;            /* Address of a freeblock within pPage->aData[] */
static int btreeComputeFreeSpace(MemPage *pPage){
*/
** in the pPage->nFree field.
** Compute the amount of freespace on the page.  In other words, fill
/*

}
  return SQLITE_OK;
  pPage->max1bytePayload = pBt->max1bytePayload;
  }
    return SQLITE_CORRUPT_PAGE(pPage);
    ** an error. */
    /* EVIDENCE-OF: R-47608-56469 Any other value for the b-tree page type is
  }else{
    pPage->minLocal = pBt->minLocal;
    pPage->maxLocal = pBt->maxLocal;
    pPage->xParseCell = btreeParseCellPtrIndex;
    pPage->intKeyLeaf = 0;
    pPage->intKey = 0;
    assert( (PTF_ZERODATA|PTF_LEAF)==10 );
    ** leaf index b-tree page. */
    /* EVIDENCE-OF: R-59615-42828 A value of 10 (0x0a) means the page is a
    assert( (PTF_ZERODATA)==2 );
    ** interior index b-tree page. */
    /* EVIDENCE-OF: R-43316-37308 A value of 2 (0x02) means the page is an
  }else if( flagByte==PTF_ZERODATA ){
    pPage->minLocal = pBt->minLeaf;
    pPage->maxLocal = pBt->maxLeaf;
    }
      pPage->xParseCell = btreeParseCellPtrNoPayload;
      pPage->xCellSize = cellSizePtrNoPayload;
      pPage->intKeyLeaf = 0;
    }else{
      pPage->xParseCell = btreeParseCellPtr;
      pPage->intKeyLeaf = 1;
    if( pPage->leaf ){
    pPage->intKey = 1;
    assert( (PTF_LEAFDATA|PTF_INTKEY|PTF_LEAF)==13 );
    ** leaf table b-tree page. */
    /* EVIDENCE-OF: R-26900-09176 A value of 13 (0x0d) means the page is a
    assert( (PTF_LEAFDATA|PTF_INTKEY)==5 );
    ** interior table b-tree page. */
    /* EVIDENCE-OF: R-07291-35328 A value of 5 (0x05) means the page is an
  if( flagByte==(PTF_LEAFDATA | PTF_INTKEY) ){
  pBt = pPage->pBt;
  pPage->xCellSize = cellSizePtr;
  pPage->childPtrSize = 4-4*pPage->leaf;
  flagByte &= ~PTF_LEAF;
  pPage->leaf = (u8)(flagByte>>3);  assert( PTF_LEAF == 1<<3 );
  assert( sqlite3_mutex_held(pPage->pBt->mutex) );
  assert( pPage->hdrOffset==(pPage->pgno==1 ? 100 : 0) );

  BtShared *pBt;     /* A copy of pPage->pBt */
static int decodeFlags(MemPage *pPage, int flagByte){
*/
**         PTF_LEAFDATA | PTF_INTKEY | PTF_LEAF
**         PTF_LEAFDATA | PTF_INTKEY
**         PTF_ZERODATA | PTF_LEAF
**         PTF_ZERODATA
**
** indicates a corrupt database files:
** Only the following combinations are supported.  Anything different
**
** and initialize fields of the MemPage structure accordingly.
** Decode the flags byte (the first byte of the header) for a page
/*

}
  return SQLITE_OK;
  pPage->nFree += iOrigSize;
  put2byte(&data[iStart+2], iSize);
  put2byte(&data[iStart], iFreeBlk);
  }
    memset(&data[iStart], 0, iSize);
    ** option is enabled */
    /* Overwrite deleted information with zeros when the secure_delete
  if( pPage->pBt->btsFlags & BTS_FAST_SECURE ){
  }
    put2byte(&data[iPtr], iStart);
    /* Insert the new freeblock into the freelist */
  }else{
    put2byte(&data[hdr+5], iEnd);
    put2byte(&data[hdr+1], iFreeBlk);
    if( iPtr!=hdr+1 ) return SQLITE_CORRUPT_PAGE(pPage);
    if( iStart<x ) return SQLITE_CORRUPT_PAGE(pPage);
    ** freelist entry */
    ** so just extend the cell content area rather than create another
    /* The new freeblock is at the beginning of the cell content area,
  if( iStart<=x ){
  x = get2byte(&data[hdr+5]);
  }
    data[hdr+7] -= nFrag;
    if( nFrag>data[hdr+7] ) return SQLITE_CORRUPT_PAGE(pPage);
    }
      }
        iStart = iPtr;
        iSize = iEnd - iPtr;
        nFrag += iStart - iPtrEnd;
        if( iPtrEnd>iStart ) return SQLITE_CORRUPT_PAGE(pPage);
      if( iPtrEnd+3>=iStart ){
      int iPtrEnd = iPtr + get2byte(&data[iPtr+2]);
    if( iPtr>hdr+1 ){
    */
    ** coalesced onto the end of iPtr.
    ** pointer in the page header) then check to see if iStart should be
    /* If iPtr is another freeblock (that is, if iPtr is not the freelist
  
    }
      iFreeBlk = get2byte(&data[iFreeBlk]);
      iSize = iEnd - iStart;
      }
        return SQLITE_CORRUPT_PAGE(pPage);
      if( NEVER(iEnd > pPage->pBt->usableSize) ){
      iEnd = iFreeBlk + get2byte(&data[iFreeBlk+2]);
      if( iEnd>iFreeBlk ) return SQLITE_CORRUPT_PAGE(pPage);
      nFrag = iFreeBlk - iEnd;
    if( iFreeBlk && iEnd+3>=iFreeBlk ){
    */
    ** Check to see if iFreeBlk should be coalesced onto the end of iStart.
    **
    **    iPtr:       The address of a pointer to iFreeBlk
    **    iFreeBlk:   First freeblock after iStart, or zero if none
    /* At this point:
  
    assert( iFreeBlk>iPtr || iFreeBlk==0 );
    }
      return SQLITE_CORRUPT_PAGE(pPage);
    if( iFreeBlk>pPage->pBt->usableSize-4 ){ /* TH3: corrupt081.100 */
    }
      iPtr = iFreeBlk;
      }
        return SQLITE_CORRUPT_PAGE(pPage);
        if( iFreeBlk==0 ) break; /* TH3: corrupt082.100 */
      if( iFreeBlk<iPtr+4 ){
    while( (iFreeBlk = get2byte(&data[iPtr]))<iStart ){
  }else{
    iFreeBlk = 0;  /* Shortcut for the case when the freelist is empty */
  if( data[iPtr+1]==0 && data[iPtr]==0 ){
  iPtr = hdr + 1;
  hdr = pPage->hdrOffset;
  */
  ** spot on the list where iStart should be inserted.
  /* The list of freeblocks must be in ascending order.  Find the 

  assert( iStart<=pPage->pBt->usableSize-4 );
  assert( iSize>=4 );   /* Minimum cell size is 4 */
  assert( sqlite3_mutex_held(pPage->pBt->mutex) );
  assert( CORRUPT_DB || iEnd <= pPage->pBt->usableSize );
  assert( CORRUPT_DB || iStart>=pPage->hdrOffset+6+pPage->childPtrSize );
  assert( sqlite3PagerIswriteable(pPage->pDbPage) );
  assert( pPage->pBt!=0 );

  unsigned char *data = pPage->aData;   /* Page content */
  u32 iEnd = iStart + iSize;            /* First byte past the iStart buffer */
  u16 x;                                /* Offset to cell content area */
  u16 iOrigSize = iSize;                /* Original value of iSize */
  u8 nFrag = 0;                         /* Reduction in fragmentation */
  u8 hdr;                               /* Page header size.  0 or 100 */
  u16 iFreeBlk;                         /* Address of the next freeblock */
  u16 iPtr;                             /* Address of ptr to next freeblock */
static int freeSpace(MemPage *pPage, u16 iStart, u16 iSize){
*/
** routine and return SQLITE_CORRUPT if any problems are found.
** at the end of the page.  So do additional corruption checks inside this
** does it detect cells or freeblocks that encrouch into the reserved bytes
** that routine will not detect overlap between cells or freeblocks.  Nor
** Even though the freeblock list was checked by btreeComputeFreeSpace(),
**
** Adjacent freeblocks are coalesced.
**
** and the size of the block is iSize bytes.
** The first byte of the new free block is pPage->aData[iStart]
** Return a section of the pPage->aData to the freelist.
/*

}
  return SQLITE_OK;
  *pIdx = top;
  assert( top+nByte <= (int)pPage->pBt->usableSize );
  put2byte(&data[hdr+5], top);
  top -= nByte;
  */
  ** The assert() below verifies the previous sentence.
  ** is no way that the allocation can extend off the end of the page.
  ** validated the freelist.  Given that the freelist is valid, there
  ** and the cell content area.  The btreeComputeFreeSpace() call has already
  /* Allocate memory from the gap in between the cell pointer array


  }
    assert( gap+2+nByte<=top );
    top = get2byteNotZero(&data[hdr+5]);
    if( rc ) return rc;
    rc = defragmentPage(pPage, MIN(4, pPage->nFree - (2+nByte)));
    assert( pPage->nFree>=0 );
    assert( pPage->nCell>0 || CORRUPT_DB );
  if( gap+2+nByte>top ){
  testcase( gap+2+nByte==top );
  */
  ** to see if defragmentation is necessary.
  /* The request could not be fulfilled using a freelist slot.  Check

  }
    }
      return rc;
    }else if( rc ){
      }
        return SQLITE_OK;
      }else{
        return SQLITE_CORRUPT_PAGE(pPage);
      if( NEVER(g2<=gap) ){
      *pIdx = g2 = (int)(pSpace-data);
      assert( pSpace+nByte<=data+pPage->pBt->usableSize );
      int g2;
    if( pSpace ){
    u8 *pSpace = pageFindSlot(pPage, nByte, &rc);
  if( (data[hdr+2] || data[hdr+1]) && gap+2<=top ){
  testcase( gap==top );
  testcase( gap+1==top );
  testcase( gap+2==top );
  */
  ** freelist looking for a slot big enough to satisfy the request.
  ** and if the freelist is not empty, then search the
  /* If there is enough space between gap and top for one more cell pointer,

  }
    }
      return SQLITE_CORRUPT_PAGE(pPage);
    }else{
      top = 65536;
    if( top==0 && pPage->pBt->usableSize==65536 ){
  if( gap>top ){
  assert( top<=(int)pPage->pBt->usableSize ); /* by btreeComputeFreeSpace() */
  top = get2byte(&data[hdr+5]);
  ** integer, so a value of 0 is used in its place. */
  ** However, that integer is too large to be stored in a 2-byte unsigned
  ** then the cell content offset of an empty page wants to be 65536.
  ** and the reserved space is zero (the usual value for reserved space)
  /* EVIDENCE-OF: R-29356-02391 If the database uses a 65536-byte page size
  assert( gap<=65536 );
  gap = pPage->cellOffset + 2*pPage->nCell;
  assert( pPage->cellOffset == hdr + 12 - 4*pPage->leaf );

  assert( nByte < (int)(pPage->pBt->usableSize-8) );
  assert( pPage->nOverflow==0 );
  assert( pPage->nFree>=nByte );
  assert( nByte>=0 );  /* Minimum cell size is 4 */
  assert( sqlite3_mutex_held(pPage->pBt->mutex) );
  assert( pPage->pBt );
  assert( sqlite3PagerIswriteable(pPage->pDbPage) );
  
  int gap;        /* First byte of gap between cell pointers and cell content */
  int rc = SQLITE_OK;                  /* Integer return code */
  int top;                             /* First byte of cell content area */
  u8 * const data = pPage->aData;      /* Local cache of pPage->aData */
  const int hdr = pPage->hdrOffset;    /* Local cache of pPage->hdrOffset */
static int allocateSpace(MemPage *pPage, int nByte, int *pIdx){
*/
** also end up needing a new cell pointer.
** allocation is being made in order to insert a new cell, so we will
** the first two bytes past the cell pointer area since presumably this
** all the space together, however.  This routine will avoid using
** allocation.  This routine might need to defragment in order to bring
** The caller guarantees that there is sufficient space to make the
**
** an error code (usually SQLITE_CORRUPT).
** of the first byte of allocated space. Return either SQLITE_OK or
** as the first argument. Write into *pIdx the index into pPage->aData[]
** Allocate nByte bytes of space from within the B-Tree page passed
/*

}
  return 0;
  }
    *pRc = SQLITE_CORRUPT_PAGE(pPg);
    /* The free slot chain extends off the end of the page */
  if( pc>maxPC+nByte-4 ){
  }
    }
      return 0;
      }
        *pRc = SQLITE_CORRUPT_PAGE(pPg);
        /* The next slot in the chain is not past the end of the current slot */
      if( pc ){
    if( pc<=iAddr+size ){
    pc = get2byte(&aData[pc]);
    iAddr = pc;
    }
      return &aData[pc + x];
      }
        put2byte(&aData[pc+2], x);
        ** for the portion used by the new allocation. */
        /* The slot remains on the free-list. Reduce its size to account
      }else{
        return 0;
        *pRc = SQLITE_CORRUPT_PAGE(pPg);
        /* This slot extends off the end of the usable part of the page */
      }else if( x+pc > maxPC ){
        aData[hdr+7] += (u8)x;
        memcpy(&aData[iAddr], &aData[pc], 2);
        ** fragmented bytes within the page. */
        /* Remove the slot from the free-list. Update the number of

        if( aData[hdr+7]>57 ) return 0;
        ** number of bytes in fragments may not exceed 60. */
        /* EVIDENCE-OF: R-11498-58022 In a well-formed b-tree page, the total
      if( x<4 ){
      testcase( x==3 );
      testcase( x==4 );
    if( (x = size - nByte)>=0 ){
    size = get2byte(&aData[pc+2]);
    ** in bytes, including the 4-byte header. */
    ** freeblock form a big-endian integer which is the size of the freeblock
    /* EVIDENCE-OF: R-22710-53328 The third and fourth bytes of each
  while( pc<=maxPC ){
  assert( pc>0 );

  int size;                                  /* Size of the free slot */
  int maxPC = pPg->pBt->usableSize - nByte;  /* Max address for a usable slot */
  int x;                                     /* Excess size of the slot */
  int pc = get2byte(&aData[iAddr]);          /* Address of a free slot */
  int iAddr = hdr + 1;                       /* Address of ptr to pc */
  u8 * const aData = pPg->aData;             /* Page data */
  const int hdr = pPg->hdrOffset;            /* Offset to page header */
static u8 *pageFindSlot(MemPage *pPg, int nByte, int *pRc){
*/
** causes the fragmentation count to exceed 60.
** will be ignored if adding the extra space to the fragmentation count
** Slots on the free list that are between 1 and 3 bytes larger than nByte
**
** detected then *pRc is set to SQLITE_CORRUPT and NULL is returned.
** This function may detect corruption within pPg.  If corruption is
**
** If no suitable space can be found on the free-list, return NULL.
**
** from the free-list.
** size. If one can be found, return a pointer to the space and remove it
** Search the free-list on page pPg for space to store a cell nByte bytes in
/*

}
  return SQLITE_OK;
  assert( sqlite3PagerIswriteable(pPage->pDbPage) );
  memset(&data[iCellFirst], 0, cbrk-iCellFirst);
  data[hdr+2] = 0;
  data[hdr+1] = 0;
  put2byte(&data[hdr+5], cbrk);
  assert( cbrk>=iCellFirst );
  }
    return SQLITE_CORRUPT_PAGE(pPage);
  if( data[hdr+7]+cbrk-iCellFirst!=pPage->nFree ){
  assert( pPage->nFree>=0 );
 defragment_out:

  data[hdr+7] = 0;
  }
    memcpy(&data[cbrk], &src[pc], size);
    }
      src = temp;
      memcpy(&temp[x], &data[x], (cbrk+size) - x);
      x = get2byte(&data[hdr+5]);
      temp = sqlite3PagerTempSpace(pPage->pBt->pPager);
      if( cbrk==pc ) continue;
      int x;
    if( temp==0 ){
    put2byte(pAddr, cbrk);
    testcase( pc+size==usableSize );
    testcase( cbrk+size==usableSize );
    assert( cbrk+size<=usableSize && cbrk>=iCellFirst );
    }
      return SQLITE_CORRUPT_PAGE(pPage);
    if( cbrk<iCellFirst || pc+size>usableSize ){
    cbrk -= size;
    size = pPage->xCellSize(pPage, &src[pc]);
    assert( pc>=iCellFirst && pc<=iCellLast );
    }
      return SQLITE_CORRUPT_PAGE(pPage);
    if( pc<iCellFirst || pc>iCellLast ){
    */
    ** if PRAGMA cell_size_check=ON.
    /* These conditions have already been verified in btreeInitPage()
    testcase( pc==iCellLast );
    testcase( pc==iCellFirst );
    pc = get2byte(pAddr);
    pAddr = &data[cellOffset + i*2];
    u8 *pAddr;     /* The i-th cell pointer */
  for(i=0; i<nCell; i++){
  iCellLast = usableSize - 4;
  cbrk = usableSize;

  }
    }
      }
        goto defragment_out;
        }
          else if( pc<iFree2 ){ put2byte(pAddr, pc+sz2); }
          if( pc<iFree ){ put2byte(pAddr, pc+sz); }
          pc = get2byte(pAddr);
        for(pAddr=&data[cellOffset]; pAddr<pEnd; pAddr+=2){
        memmove(&data[cbrk], &data[top], iFree-top);
        assert( cbrk+(iFree-top) <= usableSize );
        cbrk = top+sz;

        }
          return SQLITE_CORRUPT_PAGE(pPage);
        }else if( NEVER(iFree+sz>usableSize) ){
          sz += sz2;
          memmove(&data[iFree+sz+sz2], &data[iFree+sz], iFree2-(iFree+sz));
          if( iFree2+sz2 > usableSize ) return SQLITE_CORRUPT_PAGE(pPage);
          sz2 = get2byte(&data[iFree2+2]);
          if( iFree+sz>iFree2 ) return SQLITE_CORRUPT_PAGE(pPage);
        if( iFree2 ){
        }
          return SQLITE_CORRUPT_PAGE(pPage);
        if( top>=iFree ){
        int top = get2byte(&data[hdr+5]);
        int sz = get2byte(&data[iFree+2]);
        int sz2 = 0;
        u8 *pAddr;
        u8 *pEnd = &data[cellOffset + nCell*2];
      if( 0==iFree2 || (data[iFree2]==0 && data[iFree2+1]==0) ){
      if( iFree2>usableSize-4 ) return SQLITE_CORRUPT_PAGE(pPage);
      int iFree2 = get2byte(&data[iFree]);
    if( iFree ){
    if( iFree>usableSize-4 ) return SQLITE_CORRUPT_PAGE(pPage);
    int iFree = get2byte(&data[hdr+1]);
  if( (int)data[hdr+7]<=nMaxFrag ){
  ** reconstruct the entire page.  */
  ** offsets to each pointer in the cell-pointer array than it is to 
  ** two (or one) blocks of cells using memmove() and add the required
  ** or fewer fragmented bytes. In this case it is faster to move the
  /* This block handles pages with two or fewer free blocks and nMaxFrag

  usableSize = pPage->pBt->usableSize;
  iCellFirst = cellOffset + 2*nCell;
  assert( nCell==get2byte(&data[hdr+3]) || CORRUPT_DB );
  nCell = pPage->nCell;
  cellOffset = pPage->cellOffset;
  hdr = pPage->hdrOffset;
  src = data = pPage->aData;
  temp = 0;
  assert( sqlite3_mutex_held(pPage->pBt->mutex) );
  assert( pPage->nOverflow==0 );
  assert( pPage->pBt->usableSize <= SQLITE_MAX_PAGE_SIZE );
  assert( pPage->pBt!=0 );
  assert( sqlite3PagerIswriteable(pPage->pDbPage) );

  int iCellLast;             /* Last possible cell index */
  int iCellFirst;            /* First allowable cell index */
  unsigned char *src;        /* Source of content */
  unsigned char *temp;       /* Temp area for cell content */
  unsigned char *data;       /* The page data */
  int nCell;                 /* Number of cells on the page */
  int cbrk;                  /* Offset to the cell content area */
  int cellOffset;            /* Offset to the cell pointer array */
  int usableSize;            /* Number of usable bytes on a page */
  int size;                  /* Size of a cell */
  int hdr;                   /* Offset to the page header */
  int pc;                    /* Address of the i-th cell */
  int i;                     /* Loop counter */
static int defragmentPage(MemPage *pPage, int nMaxFrag){
*/
** cells are packed tightly at the end of the page.
** unused bytes are contained in the unallocated space region, and all
** b-tree page so that there are no freeblocks or fragment bytes, all
** EVIDENCE-OF: R-44582-60138 SQLite may from time to time reorganize a
**
** present in the page after this routine returns.
** Parameter nMaxFrag is the maximum amount of fragmented space that may be
**
** page so that there are no free-blocks on the free-block list.
** Defragment the page given. This routine reorganizes cells within the
/*


#endif
}
  }
    ptrmapPut(pPage->pBt, ovfl, PTRMAP_OVERFLOW1, pPage->pgno, pRC);
    ovfl = get4byte(&pCell[info.nSize-4]);
    }
      return;
      *pRC = SQLITE_CORRUPT_BKPT;
      testcase( pSrc!=pPage );
    if( SQLITE_WITHIN(pSrc->aDataEnd, pCell, pCell+info.nLocal) ){
    Pgno ovfl;
  if( info.nLocal<info.nPayload ){
  pPage->xParseCell(pPage, pCell, &info);
  assert( pCell!=0 );
  if( *pRC ) return;
  CellInfo info;
static void ptrmapPutOvflPtr(MemPage *pPage, MemPage *pSrc, u8 *pCell,int *pRC){
*/
** the overflow page that will be valid after pCell has been moved to pPage.
** pointer to an overflow page, insert an entry into the pointer-map for
** of pPage.  (pSrc and pPager are often the same.)  If pCell contains a
** The cell pCell is currently part of page pSrc but will ultimately be part
/*
#ifndef SQLITE_OMIT_AUTOVACUUM

#endif
}
  return pPage->xCellSize(pPage, findCell(pPage, iCell));
static u16 cellSize(MemPage *pPage, int iCell){
** only. */
/* This variation on cellSizePtr() is used inside of assert() statements
#ifdef SQLITE_DEBUG


}
  return (u16)(pIter - pCell);
  assert( debuginfo.nSize==(u16)(pIter - pCell) || CORRUPT_DB );
  while( (*pIter++)&0x80 && pIter<pEnd );
  pEnd = pIter + 9;
  assert( pPage->childPtrSize==4 );

#endif
  UNUSED_PARAMETER(pPage);
#else
  pPage->xParseCell(pPage, pCell, &debuginfo);
  CellInfo debuginfo;
  ** this function verifies that this invariant is not violated. */
  ** cell. If SQLITE_DEBUG is defined, an assert() at the bottom of
  ** the (CellInfo.nSize) value found by doing a full parse of the
  /* The value returned by this function should always be the same as
#ifdef SQLITE_DEBUG

  u8 *pEnd;              /* End mark for a varint */
  u8 *pIter = pCell + 4; /* For looping over bytes of pCell */
static u16 cellSizePtrNoPayload(MemPage *pPage, u8 *pCell){
}
  return (u16)nSize;
  assert( nSize==debuginfo.nSize || CORRUPT_DB );
  }
    nSize += 4 + (u16)(pIter - pCell);
    }
      nSize = minLocal;
    if( nSize>pPage->maxLocal ){
    testcase( nSize==pPage->maxLocal+1 );
    testcase( nSize==pPage->maxLocal );
    nSize = minLocal + (nSize - minLocal) % (pPage->pBt->usableSize - 4);
    int minLocal = pPage->minLocal;
  }else{
    if( nSize<4 ) nSize = 4;
    nSize += (u32)(pIter - pCell);
  if( nSize<=pPage->maxLocal ){
  testcase( nSize==pPage->maxLocal+1 );
  testcase( nSize==pPage->maxLocal );
  }
    while( (*pIter++)&0x80 && pIter<pEnd );
    pEnd = &pIter[9];
    ** past the end of the key value. */
    ** integer. The following block moves pIter to point at the first byte
    /* pIter now points at the 64-bit integer key value, a variable length 
  if( pPage->intKey ){
  pIter++;
  }
    }while( *(pIter)>=0x80 && pIter<pEnd );
      nSize = (nSize<<7) | (*++pIter & 0x7f);
    do{
    nSize &= 0x7f;
    pEnd = &pIter[8];
  if( nSize>=0x80 ){
  nSize = *pIter;

#endif
  pPage->xParseCell(pPage, pCell, &debuginfo);
  CellInfo debuginfo;
  ** this function verifies that this invariant is not violated. */
  ** cell. If SQLITE_DEBUG is defined, an assert() at the bottom of
  ** the (CellInfo.nSize) value found by doing a full parse of the
  /* The value returned by this function should always be the same as
#ifdef SQLITE_DEBUG

  u32 nSize;                               /* Size value to return */
  u8 *pEnd;                                /* End mark for a varint */
  u8 *pIter = pCell + pPage->childPtrSize; /* For looping over bytes of pCell */
static u16 cellSizePtr(MemPage *pPage, u8 *pCell){
*/
** cellSizePtr()             =>   all index nodes & table leaf nodes
** cellSizePtrNoPayload()    =>   table internal nodes
**
** the space used by the cell pointer.
** data header and the local payload, but not any overflow page or
** data area of the btree-page.  The return number includes the cell
** Compute the total number of bytes that a Cell needs in the cell
**
** method.
** The following routines are implementations of the MemPage.xCellSize
/*

}
  pPage->xParseCell(pPage, findCell(pPage, iCell), pInfo);
){
  CellInfo *pInfo         /* Fill in this structure */
  int iCell,              /* The cell index.  First cell is 0 */
  MemPage *pPage,         /* Page containing the cell */
static void btreeParseCell(
}
  }
    btreeParseCellAdjustSizeForOverflow(pPage, pCell, pInfo);
  }else{
    pInfo->nLocal = (u16)nPayload;
    if( pInfo->nSize<4 ) pInfo->nSize = 4;
    pInfo->nSize = nPayload + (u16)(pIter - pCell);
    */
    ** on the local page.  No overflow is required.
    /* This is the (easy) common case where the entire payload fits
  if( nPayload<=pPage->maxLocal ){
  testcase( nPayload==pPage->maxLocal+1 );
  testcase( nPayload==pPage->maxLocal );
  pInfo->pPayload = pIter;
  pInfo->nPayload = nPayload;
  pInfo->nKey = nPayload;
  pIter++;
  }
    }while( *(pIter)>=0x80 && pIter<pEnd );
      nPayload = (nPayload<<7) | (*++pIter & 0x7f);
    do{
    nPayload &= 0x7f;
    u8 *pEnd = &pIter[8];
  if( nPayload>=0x80 ){
  nPayload = *pIter;
  pIter = pCell + pPage->childPtrSize;
  assert( pPage->intKeyLeaf==0 );
  assert( pPage->leaf==0 || pPage->leaf==1 );
  assert( sqlite3_mutex_held(pPage->pBt->mutex) );

  u32 nPayload;           /* Number of bytes of cell payload */
  u8 *pIter;              /* For scanning through pCell */
){
  CellInfo *pInfo         /* Fill in this structure */
  u8 *pCell,              /* Pointer to the cell text. */
  MemPage *pPage,         /* Page containing the cell */
static void btreeParseCellPtrIndex(
}
  }
    btreeParseCellAdjustSizeForOverflow(pPage, pCell, pInfo);
  }else{
    pInfo->nLocal = (u16)nPayload;
    if( pInfo->nSize<4 ) pInfo->nSize = 4;
    pInfo->nSize = nPayload + (u16)(pIter - pCell);
    */
    ** on the local page.  No overflow is required.
    /* This is the (easy) common case where the entire payload fits
  if( nPayload<=pPage->maxLocal ){
  testcase( nPayload==pPage->maxLocal+1 );
  testcase( nPayload==pPage->maxLocal );
  pInfo->pPayload = pIter;
  pInfo->nPayload = nPayload;
  pInfo->nKey = *(i64*)&iKey;

  pIter++;
  }
    }
      }
        break;
        iKey = (iKey<<8) | *++pIter;
      if( pIter>=pEnd ){
      if( (*pIter)<0x80 ) break;
      iKey = (iKey<<7) | (*++pIter & 0x7f);
    while(1){
    iKey &= 0x7f;
    u8 *pEnd = &pIter[7];
  if( iKey>=0x80 ){
  iKey = *pIter;
  */
  ** The code is inlined to avoid a function call.
  **
  **     pIter += getVarint(pIter, (u64*)&pInfo->nKey);
  **
  /* The next block of code is equivalent to:

  pIter++;
  }
    }while( (*pIter)>=0x80 && pIter<pEnd );
      nPayload = (nPayload<<7) | (*++pIter & 0x7f);
    do{
    nPayload &= 0x7f;
    u8 *pEnd = &pIter[8];
  if( nPayload>=0x80 ){
  nPayload = *pIter;
  */
  ** The code is inlined to avoid a function call.
  **
  **     pIter += getVarint32(pIter, nPayload);
  **
  /* The next block of code is equivalent to:

  pIter = pCell;
  assert( pPage->childPtrSize==0 );
  assert( pPage->intKeyLeaf );
  assert( pPage->leaf==0 || pPage->leaf==1 );
  assert( sqlite3_mutex_held(pPage->pBt->mutex) );

  u64 iKey;               /* Extracted Key value */
  u32 nPayload;           /* Number of bytes of cell payload */
  u8 *pIter;              /* For scanning through pCell */
){
  CellInfo *pInfo         /* Fill in this structure */
  u8 *pCell,              /* Pointer to the cell text. */
  MemPage *pPage,         /* Page containing the cell */
static void btreeParseCellPtr(
}
  return;
  pInfo->pPayload = 0;
  pInfo->nLocal = 0;
  pInfo->nPayload = 0;
  pInfo->nSize = 4 + getVarint(&pCell[4], (u64*)&pInfo->nKey);
#endif
  UNUSED_PARAMETER(pPage);
#ifndef SQLITE_DEBUG
  assert( pPage->childPtrSize==4 );
  assert( pPage->leaf==0 );
  assert( sqlite3_mutex_held(pPage->pBt->mutex) );
){
  CellInfo *pInfo         /* Fill in this structure */
  u8 *pCell,              /* Pointer to the cell text. */
  MemPage *pPage,         /* Page containing the cell */
static void btreeParseCellPtrNoPayload(
*/
** by pointer.
** all MemPage types and that references the cell by index rather than
** There is also a wrapper function btreeParseCell() that works for
**
** btreeParseCellPtrIndex()   =>   index btree nodes
** btreeParseCellNoPayload()  =>   table btree internal nodes
** btreeParseCellPtr()        =>   table btree leaf nodes
**
** Parse a cell content block and fill in the CellInfo structure.
**
** method.
** The following routines are implementations of the MemPage.xParseCell()
/*

}
  pInfo->nSize = (u16)(&pInfo->pPayload[pInfo->nLocal] - pCell) + 4;
  }
    pInfo->nLocal = (u16)minLocal;
  }else{
    pInfo->nLocal = (u16)surplus;
  if( surplus <= maxLocal ){
  testcase( surplus==maxLocal+1 );
  testcase( surplus==maxLocal );
  surplus = minLocal + (pInfo->nPayload - minLocal)%(pPage->pBt->usableSize-4);
  maxLocal = pPage->maxLocal;
  minLocal = pPage->minLocal;

  int surplus;   /* Overflow payload available for local storage */
  int maxLocal;  /* Maximum amount of payload held locally */
  int minLocal;  /* Minimum amount of payload held locally */
  */
  ** way will result in an incompatible file format.
  ** Warning:  changing the way overflow payload is distributed in any
  **
  ** in between minLocal and maxLocal.
  ** space on overflow pages while keeping the amount of local storage
  ** overflow pages.  The strategy is to minimize the amount of unused
  ** to decide how much to store locally and how much to spill onto
  /* If the payload will not fit completely on the local page, we have
){
  CellInfo *pInfo         /* Fill in this structure */
  u8 *pCell,              /* Pointer to the cell text. */
  MemPage *pPage,         /* Page containing the cell */
static SQLITE_NOINLINE void btreeParseCellAdjustSizeForOverflow(
*/
** structure.
** on a single B-tree page.  Make necessary adjustments to the CellInfo
** btreeParseCellPtrIndex() for the case when the cell does not fit entirely
** This is common tail processing for btreeParseCellPtr() and
/*


  ((P)->aDataOfst + ((P)->maskPage & get2byteAligned(&(P)->aCellIdx[2*(I)])))
#define findCellPastPtr(P,I) \
  ((P)->aData + ((P)->maskPage & get2byteAligned(&(P)->aCellIdx[2*(I)])))
#define findCell(P,I) \
*/
** This routine works only for pages that do not contain overflow cells.
**
** 4-byte child pointer found on interior pages, if there is one.
** findCellPastPtr() does the same except it skips past the initial
**
** to the cell content.
** the page, 1 means the second cell, and so forth) return a pointer
** Given a btree page and a cell index (0 means the first cell on
/*

#endif
  #define ptrmapPutOvflPtr(x, y, z, rc)
  #define ptrmapGet(w,x,y,z) SQLITE_OK
  #define ptrmapPut(w,x,y,z,rc)
#else /* if defined SQLITE_OMIT_AUTOVACUUM */

}
  return SQLITE_OK;
  if( *pEType<1 || *pEType>5 ) return SQLITE_CORRUPT_PGNO(iPtrmap);
  sqlite3PagerUnref(pDbPage);

  if( pPgno ) *pPgno = get4byte(&pPtrmap[offset+1]);
  *pEType = pPtrmap[offset];
  assert( pEType!=0 );
  assert( offset <= (int)pBt->usableSize-5 );
  }
    return SQLITE_CORRUPT_BKPT;
    sqlite3PagerUnref(pDbPage);
  if( offset<0 ){
  offset = PTRMAP_PTROFFSET(iPtrmap, key);

  pPtrmap = (u8 *)sqlite3PagerGetData(pDbPage);
  }
    return rc;
  if( rc!=0 ){
  rc = sqlite3PagerGet(pBt->pPager, iPtrmap, &pDbPage, 0);
  iPtrmap = PTRMAP_PAGENO(pBt, key);

  assert( sqlite3_mutex_held(pBt->mutex) );

  int rc;
  int offset;        /* Offset of entry in pointer map */
  u8 *pPtrmap;       /* Pointer map page data */
  int iPtrmap;       /* Pointer map page index */
  DbPage *pDbPage;   /* The pointer map page */
static int ptrmapGet(BtShared *pBt, Pgno key, u8 *pEType, Pgno *pPgno){
*/
** An error code is returned if something goes wrong, otherwise SQLITE_OK.
** the type and parent page number to *pEType and *pPgno respectively.
** This routine retrieves the pointer map entry for page 'key', writing
**
** Read an entry from the pointer map.
/*

}
  sqlite3PagerUnref(pDbPage);
ptrmap_exit:

  }
    }
      put4byte(&pPtrmap[offset+1], parent);
      pPtrmap[offset] = eType;
    if( rc==SQLITE_OK ){
    *pRC= rc = sqlite3PagerWrite(pDbPage);
    TRACE(("PTRMAP_UPDATE: %d->(%d,%d)\n", key, eType, parent));
  if( eType!=pPtrmap[offset] || get4byte(&pPtrmap[offset+1])!=parent ){

  pPtrmap = (u8 *)sqlite3PagerGetData(pDbPage);
  assert( offset <= (int)pBt->usableSize-5 );
  }
    goto ptrmap_exit;
    *pRC = SQLITE_CORRUPT_BKPT;
  if( offset<0 ){
  offset = PTRMAP_PTROFFSET(iPtrmap, key);
  }
    goto ptrmap_exit;
    *pRC = SQLITE_CORRUPT_BKPT;
    ** as a btree page. */
    ** If that byte is set, it means this page is also being used
    /* The first byte of the extra data is the MemPage.isInit byte.
  if( ((char*)sqlite3PagerGetExtra(pDbPage))[0]!=0 ){
  }
    return;
    *pRC = rc;
  if( rc!=SQLITE_OK ){
  rc = sqlite3PagerGet(pBt->pPager, iPtrmap, &pDbPage, 0);
  iPtrmap = PTRMAP_PAGENO(pBt, key);
  }
    return;
    *pRC = SQLITE_CORRUPT_BKPT;
  if( key==0 ){
  assert( pBt->autoVacuum );

  assert( 0==PTRMAP_ISPAGE(pBt, PENDING_BYTE_PAGE(pBt)) );
  /* The master-journal page number must never be used as a pointer map page */
  assert( sqlite3_mutex_held(pBt->mutex) );

  if( *pRC ) return;

  int rc;           /* Return code from subfunctions */
  int offset;       /* Offset in pointer map page */
  Pgno iPtrmap;     /* The pointer map page number */
  u8 *pPtrmap;      /* The pointer map data */
  DbPage *pDbPage;  /* The pointer map page */
static void ptrmapPut(BtShared *pBt, Pgno key, u8 eType, Pgno parent, int *pRC){
*/
** into *pRC.
** a no-op.  If an error occurs, the appropriate error code is written
** If *pRC is initially non-zero (non-SQLITE_OK) then this routine is
**
** so that it maps to type 'eType' and parent page number 'pgno'.
** This routine updates the pointer map entry for page number 'key'
**
** Write an entry into the pointer map.
/*

}
  return ret;
  }
    ret++;
  if( ret==PENDING_BYTE_PAGE(pBt) ){
  ret = (iPtrMap*nPagesPerMapPage) + 2; 
  iPtrMap = (pgno-2)/nPagesPerMapPage;
  nPagesPerMapPage = (pBt->usableSize/5)+1;
  if( pgno<2 ) return 0;
  assert( sqlite3_mutex_held(pBt->mutex) );
  Pgno iPtrMap, ret;
  int nPagesPerMapPage;
static Pgno ptrmapPageno(BtShared *pBt, Pgno pgno){
*/
** requires that ptrmapPageno(*,1)!=1.
** no pointer map associated with page 1.  The integrity_check logic
** Return 0 (not a valid page) for pgno==1 since there is
**
** input page number.
** number for the pointer-map page that contains the entry for the
** Given a page number of a regular database page, return the page
/*
#ifndef SQLITE_OMIT_AUTOVACUUM


}
  pCur->hints = x;
  assert( x==BTREE_SEEK_EQ || x==BTREE_BULKLOAD || x==0 );
void sqlite3BtreeCursorHintFlags(BtCursor *pCur, unsigned x){
*/
** Provide flag hints to the cursor.
/*

#endif
}
  /* Used only by system that substitute their own storage engine */
void sqlite3BtreeCursorHint(BtCursor *pCur, int eHintType, ...){
*/
** parameter.  See the definitions of the BTREE_HINT_* macros for details.
** and number of the varargs parameters) is determined by the eHintType
** Provide hints to the cursor.  The particular hint given (and the type
/*
#ifdef SQLITE_ENABLE_CURSOR_HINTS

}
  return SQLITE_OK;
  }
    *pDifferentRow = 0;
  }else{
    *pDifferentRow = 1;
  if( pCur->eState!=CURSOR_VALID ){
  }
    return rc;
    *pDifferentRow = 1;
  if( rc ){
  rc = restoreCursorPosition(pCur);
  assert( pCur->eState!=CURSOR_VALID );
  assert( pCur!=0 );

  int rc;
int sqlite3BtreeCursorRestore(BtCursor *pCur, int *pDifferentRow){
*/
** TRUE from sqlite3BtreeCursorHasMoved().
** This routine should only be called for a cursor that just returned
**
** nearby row.
** was pointing to has been deleted, forcing the cursor to point to some
** pointing at exactly the same row.  *pDifferntRow is the row the cursor
** On success, the *pDifferentRow parameter is false if the cursor is left
**
** a row having been deleted out from under the cursor).  
** has been moved by some outside activity (such as a btree rebalance or
** This routine restores a cursor back to its original position after it
/*

}
  return (BtCursor*)&fakeCursor;
  assert( offsetof(BtCursor, eState)==0 );
  static u8 fakeCursor = CURSOR_VALID;
BtCursor *sqlite3BtreeFakeValidCursor(void){
*/
** cursor returned must not be used with any other Btree interface.
** false to the sqlite3BtreeCursorHasMoved() routine above.  The fake
** Return a pointer to a fake BtCursor object that will always answer
/*

}
  return CURSOR_VALID != *(u8*)pCur;
  assert( sizeof(pCur->eState)==1 );
  assert( offsetof(BtCursor, eState)==0 );
       || pCur==sqlite3BtreeFakeValidCursor() );
  assert( EIGHT_BYTE_ALIGNMENT(pCur)
int sqlite3BtreeCursorHasMoved(BtCursor *pCur){
*/
** back to where it ought to be if this routine returns true.
** Use the separate sqlite3BtreeCursorRestore() routine to restore a cursor
**
** Calling this routine with a NULL cursor pointer returns false.
**
** is rebalanced.
** from under them, for example.  Cursor might also move if a btree
** Cursors can move when the row they are pointing at is deleted out
** it was last placed, or has been invalidated for any other reason.
** Determine whether or not a cursor has moved from the position where
/*

         SQLITE_OK)
         btreeRestoreCursorPosition(p) : \
  (p->eState>=CURSOR_REQUIRESEEK ? \
#define restoreCursorPosition(p) \

}
  return rc;
  }
    }
      pCur->eState = CURSOR_SKIPNEXT;
    if( pCur->skipNext && pCur->eState==CURSOR_VALID ){
    if( skipNext ) pCur->skipNext = skipNext;
    assert( pCur->eState==CURSOR_VALID || pCur->eState==CURSOR_INVALID );
    pCur->pKey = 0;
    sqlite3_free(pCur->pKey);
  if( rc==SQLITE_OK ){
  }
    rc = btreeMoveto(pCur, pCur->pKey, pCur->nKey, 0, &skipNext);
  }else{
    rc = SQLITE_IOERR;
  if( sqlite3FaultSim(410) ){
  pCur->eState = CURSOR_INVALID;
  }
    return pCur->skipNext;
  if( pCur->eState==CURSOR_FAULT ){
  assert( pCur->eState>=CURSOR_REQUIRESEEK );
  assert( cursorOwnsBtShared(pCur) );
  int skipNext = 0;
  int rc;
static int btreeRestoreCursorPosition(BtCursor *pCur){
*/
** saveCursorPosition().
** at most one effective restoreCursorPosition() call after each 
** saved position info stored by saveCursorPosition(), so there can be
** when saveCursorPosition() was called. Note that this call deletes the 
** Restore the cursor to the position it was in (or as close to as possible)
/*

}
  return rc;
  }
    sqlite3DbFree(pCur->pKeyInfo->db, pIdxKey);
  if( pIdxKey ){
moveto_done:
  rc = sqlite3BtreeMovetoUnpacked(pCur, pIdxKey, nKey, bias, pRes);
  }
    pIdxKey = 0;
  }else{
    }
      goto moveto_done;
      rc = SQLITE_CORRUPT_BKPT;
    if( pIdxKey->nField==0 || pIdxKey->nField>pKeyInfo->nAllField ){
    sqlite3VdbeRecordUnpack(pKeyInfo, (int)nKey, pKey, pIdxKey);
    if( pIdxKey==0 ) return SQLITE_NOMEM_BKPT;
    pIdxKey = sqlite3VdbeAllocUnpackedRecord(pKeyInfo);
    assert( nKey==(i64)(int)nKey );
    KeyInfo *pKeyInfo = pCur->pKeyInfo;
  if( pKey ){

  UnpackedRecord *pIdxKey;   /* Unpacked index key */
  int rc;                    /* Status code */
){
  int *pRes           /* Write search results here */
  int bias,           /* Bias search to the high end */
  i64 nKey,           /* Integer key for tables.  Size of pKey for indices */
  const void *pKey,   /* Packed key if the btree is an index */
  BtCursor *pCur,     /* Cursor open on the btree to be searched */
static int btreeMoveto(
*/
** record and then call BtreeMovetoUnpacked() to do the work.
** such as is generated by the OP_MakeRecord opcode.  Unpack the
** In this version of BtreeMoveto, pKey is a packed index record
/*

}
  pCur->eState = CURSOR_INVALID;
  pCur->pKey = 0;
  sqlite3_free(pCur->pKey);
  assert( cursorHoldsMutex(pCur) );
void sqlite3BtreeClearCursor(BtCursor *pCur){
*/
** Clear the current cursor position.
/*

}
  return SQLITE_OK;
  }while( p );
    p = p->pNext;
    }
      }
        btreeReleaseAllCursorPages(p);
        testcase( p->iPage>=0 );
      }else{
        }
          return rc;
        if( SQLITE_OK!=rc ){
        int rc = saveCursorPosition(p);
      if( p->eState==CURSOR_VALID || p->eState==CURSOR_SKIPNEXT ){
    if( p!=pExcept && (0==iRoot || p->pgnoRoot==iRoot) ){
  do{
){
  BtCursor *pExcept    /* Do not save this cursor */
  Pgno iRoot,          /* Only save cursor with this iRoot. Save all if zero */
  BtCursor *p,         /* The first cursor that needs saving */
static int SQLITE_NOINLINE saveCursorsOnList(
*/
** broken out from its caller to avoid unnecessary stack pointer movement.
** The common case is that no cursors need to be saved, so this routine is
** the cursors if and when a cursor is found that actually requires saving.
/* This helper routine to saveAllCursors does the actual work of saving

}
  return SQLITE_OK;
  if( pExcept ) pExcept->curFlags &= ~BTCF_Multiple;
  if( p ) return saveCursorsOnList(p, iRoot, pExcept);
  }
    if( p!=pExcept && (0==iRoot || p->pgnoRoot==iRoot) ) break;
  for(p=pBt->pCursor; p; p=p->pNext){
  assert( pExcept==0 || pExcept->pBt==pBt );
  assert( sqlite3_mutex_held(pBt->mutex) );
  BtCursor *p;
static int saveAllCursors(BtShared *pBt, Pgno iRoot, BtCursor *pExcept){
*/
** event that cursors are in need to being saved.
** need to be saved.  It calls out to saveCursorsOnList() in the (unusual)
** Implementation note:  This routine merely checks to see if any cursors
**
** pointless call to this routine.
** then the BTCF_Multiple flag on pExpect is cleared, to avoid another
** If pExpect!=NULL and if no other cursors are found on the same root-page,
**
** the uncommon case when pExpect has the BTCF_Multiple flag set.
** routine enforces that rule.  This routine only needs to be called in
** cursors should have their BTCF_Multiple flag set.  The btreeCursor()
** If there are two or more cursors on the same btree, then all such 
**
** table, for example in BtreeDelete() or BtreeInsert().
** routine is called just before cursor pExcept is used to modify the
** moved back to the same spot after the btree has been modified.  This
** the location in the btree is remembered in such a way that it can be
** the table with root-page iRoot.  "Saving the cursor position" means that
** Save the positions of all cursors (except pExcept) that are open on
/*

static int SQLITE_NOINLINE saveCursorsOnList(BtCursor*,Pgno,BtCursor*);
/* Forward reference */

}
  return rc;
  pCur->curFlags &= ~(BTCF_ValidNKey|BTCF_ValidOvfl|BTCF_AtLast);

  }
    pCur->eState = CURSOR_REQUIRESEEK;
    btreeReleaseAllCursorPages(pCur);
  if( rc==SQLITE_OK ){
  rc = saveCursorKey(pCur);

  }
    pCur->skipNext = 0;
  }else{
    pCur->eState = CURSOR_VALID;
  if( pCur->eState==CURSOR_SKIPNEXT ){
  }
    return SQLITE_CONSTRAINT_PINNED;
  if( pCur->curFlags & BTCF_Pinned ){

  assert( cursorHoldsMutex(pCur) );
  assert( 0==pCur->pKey );
  assert( CURSOR_VALID==pCur->eState || CURSOR_SKIPNEXT==pCur->eState );

  int rc;
static int saveCursorPosition(BtCursor *pCur){
*/
** prior to calling this routine.  
** The caller must ensure that the cursor is valid (has eState==CURSOR_VALID)
**
** and BtCursor.pKey. The cursor's state is set to CURSOR_REQUIRESEEK.
** Save the current cursor position in the variables BtCursor.nKey 
/*

}
  return rc;
  assert( !pCur->curIntKey || !pCur->pKey );
  }
    }
      rc = SQLITE_NOMEM_BKPT;
    }else{
      }
        sqlite3_free(pKey);
      }else{
        pCur->pKey = pKey;
        memset(((u8*)pKey)+pCur->nKey, 0, 9+8);
      if( rc==SQLITE_OK ){
      rc = sqlite3BtreePayload(pCur, 0, (int)pCur->nKey, pKey);
    if( pKey ){
    pKey = sqlite3Malloc( pCur->nKey + 9 + 8 );
    pCur->nKey = sqlite3BtreePayloadSize(pCur);
    void *pKey;
    ** below. */
    ** position is restored. Hence the 17 bytes of padding allocated 
    ** up to the size of 1 varint plus 1 8-byte value when the cursor 
    ** the sqlite3VdbeRecordUnpack() function may overread the buffer by
    ** that the current key is corrupt. In that case, it is possible that
    /* For an index btree, save the complete key content. It is possible
  }else{
    pCur->nKey = sqlite3BtreeIntegerKey(pCur);
    /* Only the rowid is required for a table btree */
  if( pCur->curIntKey ){

  assert( cursorHoldsMutex(pCur) );
  assert( 0==pCur->pKey );
  assert( CURSOR_VALID==pCur->eState );
  int rc = SQLITE_OK;
static int saveCursorKey(BtCursor *pCur){
*/
** the key.
** set to point to a malloced buffer pCur->nKey bytes in size containing 
** NULL. If the cursor is open on a non-intkey table, then pCur->pKey is 
** (the rowid) is stored in pCur->nKey and pCur->pKey is left set to
** If the cursor is open on an intkey table, then the integer key
**
** code otherwise.
** pCur->pKey. SQLITE_OK is returned if successful or an SQLite error 
** function saves the current cursor key in variables pCur->nKey and
** when this function is called (i.e. have eState==CURSOR_VALID). This
** The cursor passed as the only argument must point to a valid entry
/*

}
  }
    pCur->iPage = -1;
    releasePageNotNull(pCur->pPage);
    }
      releasePageNotNull(pCur->apPage[i]);
    for(i=0; i<pCur->iPage; i++){
  if( pCur->iPage>=0 ){
  int i;
static void btreeReleaseAllCursorPages(BtCursor *pCur){
*/
** Release all of the apPage[] pages for a cursor.
/*

}
  pBt->pHasContent = 0;
  sqlite3BitvecDestroy(pBt->pHasContent);
static void btreeClearHasContent(BtShared *pBt){
*/
** invoked at the conclusion of each write-transaction.
** Clear (destroy) the BtShared.pHasContent bitvec. This should be
/*

}
  return p && (pgno>sqlite3BitvecSize(p) || sqlite3BitvecTestNotNull(p, pgno));
  Bitvec *p = pBt->pHasContent;
static int btreeGetHasContent(BtShared *pBt, Pgno pgno){
*/
** page from the pager layer with the 'no-content' flag set. True otherwise.
** free-list for reuse. It returns false if it is safe to retrieve the
** This function is called when a free-list leaf page is removed from the
**
** Query the BtShared.pHasContent vector.
/*

}
  return rc;
  }
    rc = sqlite3BitvecSet(pBt->pHasContent, pgno);
  if( rc==SQLITE_OK && pgno<=sqlite3BitvecSize(pBt->pHasContent) ){
  }
    }
      rc = SQLITE_NOMEM_BKPT;
    if( !pBt->pHasContent ){
    pBt->pHasContent = sqlite3BitvecCreate(pBt->nPage);
    assert( pgno<=pBt->nPage );
  if( !pBt->pHasContent ){
  int rc = SQLITE_OK;
static int btreeSetHasContent(BtShared *pBt, Pgno pgno){
*/
** at the end of every transaction.
** set in BtShared.pHasContent. The contents of the bitvec are cleared
** optimization 2 above is omitted if the corresponding bit is already
** set in the bitvec. Whenever a leaf page is extracted from the free-list,
** moved to become a free-list leaf page, the corresponding bit is
** The solution is the BtShared.pHasContent bitvec. Whenever a page is 
**
** to restore the database to its original configuration.
** may be lost. In the event of a rollback, it may not be possible
** is extracted from the free-list and reused, then the original data
** it is moved to the free-list and it is also not journalled when it
** transaction, a problem comes up. If the page is not journalled when
** a page is moved to the free-list and then reused within the same
** performance boost to bulk delete or insert operations. However, if
** By themselves, these optimizations work fine and provide a handy
**
**      be, if it is not at all meaningful?).
**      from the database or written to the journal file (why should it
**   2) When a free-list leaf page is reused, its content is not read
**
**      why bother journalling it?).
**      such a page is not even journalled (as it will not be modified,
**      (as free-list leaf pages contain no meaningful data). Sometimes
**      a free-list leaf page, the page is not written to the database
**   1) When all data is deleted from a page and the page becomes
**
** free-list leaf pages:
** bug caused by the interaction of two useful IO optimizations surrounding
** The BtShared.pHasContent bitvec exists to work around an obscure
**
** page.
** when a page that previously contained data becomes a free-list leaf 
** Set bit pgno of the BtShared.pHasContent bitvec. This is called 
/*

#endif /* SQLITE_OMIT_INCRBLOB */
  #define invalidateIncrblobCursors(w,x,y,z)
  /* Stub function when INCRBLOB is omitted */
#else

}
  }
    }
      }
        p->eState = CURSOR_INVALID;
      if( p->pgnoRoot==pgnoRoot && (isClearTable || p->info.nKey==iRow) ){
      pBtree->hasIncrblobCur = 1;
    if( (p->curFlags & BTCF_Incrblob)!=0 ){
  for(p=pBtree->pBt->pCursor; p; p=p->pNext){
  pBtree->hasIncrblobCur = 0;
  assert( sqlite3BtreeHoldsMutex(pBtree) );
  if( pBtree->hasIncrblobCur==0 ) return;
  BtCursor *p;
){
  int isClearTable        /* True if all rows are being deleted */
  i64 iRow,               /* The rowid that might be changing */
  Pgno pgnoRoot,          /* The table that might be changing */
  Btree *pBtree,          /* The database file to check */
static void invalidateIncrblobCursors(
*/
** only those incrblob cursors open on that specific row.
** rowid iRow is being replaced or deleted. In this case invalidate
** Otherwise, if argument isClearTable is false, then the row with
**
** cursors open on any row within the table with root-page pgnoRoot.
** table is about to be deleted. In this case invalidate all incrblob
** If argument isClearTable is true, then the entire contents of the
**
** row or one of the rows being modified.
** to invalidate any incrblob cursors that are open on the
** This function is called before modifying the contents of a table
/*
#ifndef SQLITE_OMIT_INCRBLOB

}
  }
    invalidateOverflowCache(p);
  for(p=pBt->pCursor; p; p=p->pNext){
  assert( sqlite3_mutex_held(pBt->mutex) );
  BtCursor *p;
static void invalidateAllOverflowCache(BtShared *pBt){
*/
** on the shared btree structure pBt.
** Invalidate the overflow page-list cache for all cursors opened
/*

#define invalidateOverflowCache(pCur) (pCur->curFlags &= ~BTCF_ValidOvfl)
*/
** on the shared btree structure pBt.
** Invalidate the overflow cache of the cursor passed as the first argument.
/*

#endif
}
  return (p->pBtree->db==p->pBt->db);
  assert( cursorHoldsMutex(p) );
static int cursorOwnsBtShared(BtCursor *p){
*/
** does keep the database connection pointers up-to-date.
** statements only and for the purpose of double-checking that the btree code
** a connection that has already closed.  This routine is used inside assert()
** btreeInitPage() to reference an stale connection pointer that references a
** connection pointers get out-of-sync, it is possible for routines like
** database connetion. This is important in shared-cache mode. If the database 
/* Verify that the cursor and the BtShared agree about what is the current

}
  return sqlite3_mutex_held(p->pBt->mutex);
static int cursorHoldsMutex(BtCursor *p){
#ifdef SQLITE_DEBUG
*/
** Verify that the cursor holds the mutex on its BtShared
**
***** This routine is used inside of assert() only ****
/*

static void releasePageNotNull(MemPage *pPage);  /* Forward reference */
static void releasePageOne(MemPage *pPage);      /* Forward reference */
static void releasePage(MemPage *pPage);         /* Forward reference */

#endif /* SQLITE_OMIT_SHARED_CACHE */

}
  }
    }
      pLock->eLock = READ_LOCK;
      assert( pLock->eLock==READ_LOCK || pLock->pBtree==p );
    for(pLock=pBt->pLock; pLock; pLock=pLock->pNext){
    pBt->btsFlags &= ~(BTS_EXCLUSIVE|BTS_PENDING);
    pBt->pWriter = 0;
    BtLock *pLock;
  if( pBt->pWriter==p ){
  BtShared *pBt = p->pBt;
static void downgradeAllSharedCacheTableLocks(Btree *p){
*/
** This function changes all write-locks held by Btree p into read-locks.
/*

}
  }
    pBt->btsFlags &= ~BTS_PENDING;
    */
    ** be zero already. So this next line is harmless in that case.
    ** If there is not currently a writer, then BTS_PENDING must
    **
    ** set the BTS_PENDING flag to 0.
    ** than the writer must be about to drop to zero. In this case
    ** that writer, then the number of locks held by connections other
    ** transaction. If there currently exists a writer, and p is not
    /* This function is called when Btree p is concluding its 
  }else if( pBt->nTransaction==2 ){
    pBt->btsFlags &= ~(BTS_EXCLUSIVE|BTS_PENDING);
    pBt->pWriter = 0;
  if( pBt->pWriter==p ){
  assert( (pBt->btsFlags & BTS_PENDING)==0 || pBt->pWriter );

  }
    }
      ppIter = &pLock->pNext;
    }else{
      }
        sqlite3_free(pLock);
      if( pLock->iTable!=1 ){
      assert( pLock->iTable!=1 || pLock==&p->lock );
      *ppIter = pLock->pNext;
    if( pLock->pBtree==p ){
    assert( pLock->pBtree->inTrans>=pLock->eLock );
    assert( (pBt->btsFlags & BTS_EXCLUSIVE)==0 || pBt->pWriter==pLock->pBtree );
    BtLock *pLock = *ppIter;
  while( *ppIter ){

  assert( p->inTrans>0 );
  assert( p->sharable || 0==*ppIter );
  assert( sqlite3BtreeHoldsMutex(p) );

  BtLock **ppIter = &pBt->pLock;
  BtShared *pBt = p->pBt;
static void clearAllSharedCacheTableLocks(Btree *p){
*/
** may be incorrectly cleared.
** transaction. If it does not, then the BTS_PENDING flag
** This function assumes that Btree p has an open read or write 
**
** the setSharedCacheTableLock() procedure) held by Btree object p.
** Release all the table locks (locks obtained via calls to
/*
#ifndef SQLITE_OMIT_SHARED_CACHE

#endif /* !SQLITE_OMIT_SHARED_CACHE */
}
  return SQLITE_OK;

  }
    pLock->eLock = eLock;
  if( eLock>pLock->eLock ){
  assert( WRITE_LOCK>READ_LOCK );
  */
  ** and a read-lock requested, we don't incorrectly downgrade the lock.
  ** and the requested lock. This means if a write-lock was already held
  /* Set the BtLock.eLock variable to the maximum of the current lock

  }
    pBt->pLock = pLock;
    pLock->pNext = pBt->pLock;
    pLock->pBtree = p;
    pLock->iTable = iTable;
    }
      return SQLITE_NOMEM_BKPT;
    if( !pLock ){
    pLock = (BtLock *)sqlite3MallocZero(sizeof(BtLock));
  if( !pLock ){
  */
  ** with table iTable, allocate one and link it into the list.
  /* If the above search did not find a BtLock struct associating Btree p

  }
    }
      break;
      pLock = pIter;
    if( pIter->iTable==iTable && pIter->pBtree==p ){
  for(pIter=pBt->pLock; pIter; pIter=pIter->pNext){
  /* First search the list for an existing lock on this table. */

  assert( SQLITE_OK==querySharedCacheTableLock(p, iTable, eLock) );
  assert( p->sharable );
  ** has been determined that no other b-tree holds a conflicting lock.  */
  /* This function should only be called on a sharable b-tree after it 

  assert( 0==(p->db->flags&SQLITE_ReadUncommit) || eLock==WRITE_LOCK );
  ** table, and that lock is obtained in BtreeBeginTrans().  */
  ** by a connection in read-uncommitted mode is on the sqlite_master 
  ** obtain a read-lock using this function. The only read-lock obtained
  /* A connection with the read-uncommitted flag set will never try to

  assert( p->db!=0 );
  assert( eLock==READ_LOCK || eLock==WRITE_LOCK );
  assert( sqlite3BtreeHoldsMutex(p) );

  BtLock *pIter;
  BtLock *pLock = 0;
  BtShared *pBt = p->pBt;
static int setSharedCacheTableLock(Btree *p, Pgno iTable, u8 eLock){
*/
** is returned if a malloc attempt fails.
** SQLITE_OK is returned if the lock is added successfully. SQLITE_NOMEM 
**
**       already been called and returned SQLITE_OK).
**       with the requested lock (i.e. querySharedCacheTableLock() has
**   (b) No other Btree objects hold a lock that conflicts
**
**       database (one with the BtShared.sharable flag set), and
**   (a) The specified Btree object p is connected to a sharable
**
** This function assumes the following:
**
** WRITE_LOCK.
** by Btree handle p. Parameter eLock must be either READ_LOCK or 
** Add a lock on the table with root-page iTable to the shared-btree used
/*
#ifndef SQLITE_OMIT_SHARED_CACHE

#endif /* !SQLITE_OMIT_SHARED_CACHE */
}
  return SQLITE_OK;
  }
    }
      return SQLITE_LOCKED_SHAREDCACHE;
      }
        pBt->btsFlags |= BTS_PENDING;
        assert( p==pBt->pWriter );
      if( eLock==WRITE_LOCK ){
      sqlite3ConnectionBlocked(p->db, pIter->pBtree->db);
    if( pIter->pBtree!=p && pIter->iTable==iTab && pIter->eLock!=eLock ){
    assert( eLock==READ_LOCK || pIter->pBtree==p || pIter->eLock==READ_LOCK);
    assert( pIter->eLock==READ_LOCK || pIter->eLock==WRITE_LOCK );
    */
    ** only be a single writer).
    ** may hold a WRITE_LOCK on any table in this file (since there can
    ** since we know that if eLock==WRITE_LOCK, then no other connection
    **
    **   (eLock==WRITE_LOCK || pIter->eLock==WRITE_LOCK)
    **
    ** statement is a simplification of:
    /* The condition (pIter->eLock!=eLock) in the following if(...) 
  for(pIter=pBt->pLock; pIter; pIter=pIter->pNext){

  }
    return SQLITE_LOCKED_SHAREDCACHE;
    sqlite3ConnectionBlocked(p->db, pBt->pWriter->db);
  if( pBt->pWriter!=p && (pBt->btsFlags & BTS_EXCLUSIVE)!=0 ){
  */
  ** requested lock may not be obtained.
  /* If some other connection is holding an exclusive lock, the

  }
    return SQLITE_OK;
  if( !p->sharable ){
  /* This routine is a no-op if the shared-cache is not enabled */
  
  assert( eLock==READ_LOCK || pBt->inTransaction==TRANS_WRITE );
  assert( eLock==READ_LOCK || (p==pBt->pWriter && p->inTrans==TRANS_WRITE) );
  */
  ** must be an open write transaction on the file itself.
  ** transaction on this file. And, obviously, for this to be so there 
  /* If requesting a write-lock, then the Btree must have an open write
  
  assert( !(p->db->flags&SQLITE_ReadUncommit)||eLock==WRITE_LOCK||iTab==1 );
  assert( p->db!=0 );
  assert( eLock==READ_LOCK || eLock==WRITE_LOCK );
  assert( sqlite3BtreeHoldsMutex(p) );

  BtLock *pIter;
  BtShared *pBt = p->pBt;
static int querySharedCacheTableLock(Btree *p, Pgno iTab, u8 eLock){
*/
** setSharedCacheTableLock()), or SQLITE_LOCKED if not.
** SQLITE_OK if the lock may be obtained (by calling
** (READ_LOCK or WRITE_LOCK) on the table with root-page iTab. Return
** Query to see if Btree handle p may obtain a lock of type eLock 
/*

#endif    /* #ifdef SQLITE_DEBUG */
}
  return 0;
  }
    }
      return 1;
    ){
     && 0==(p->pBtree->db->flags & SQLITE_ReadUncommit)
     && p->pBtree!=pBtree
    if( p->pgnoRoot==iRoot 
  for(p=pBtree->pBt->pCursor; p; p=p->pNext){
  BtCursor *p;
static int hasReadConflicts(Btree *pBtree, Pgno iRoot){
*/
**    assert( !hasReadConflicts(pBtree, iRoot) );
**
** rooted at page iRoot, one should call:
** For example, before writing to any part of the table or index
**
** have a read cursor.
** read-uncommitted flag set, then it is OK for the other object to
** the iRoot table.  Except, if the other Btree object has the
** shares the same BtShared object is currently reading or writing
** It is illegal for pBtree to write if some other Btree object that
**
** simultaneously reading that same table or index.
** table or index rooted at iRoot because other shared connections are
** Return true if it would be illegal for pBtree to write into the
**
**** This function may be used as part of assert() statements only. ****
/*
#ifdef SQLITE_DEBUG

#endif /* SQLITE_DEBUG */
}
  return 0;
  /* Failed to find the required lock. */

  }
    }
      return 1;
    ){
     && pLock->eLock>=eLockType 
     && (pLock->iTable==iTab || (pLock->eLock==WRITE_LOCK && pLock->iTable==1))
    if( pLock->pBtree==pBtree 
  for(pLock=pBtree->pBt->pLock; pLock; pLock=pLock->pNext){
  ** read-lock on iTab will suffice. Return 1 if any of these are found.  */
  ** write-lock on the schema table, or (if the client is reading) a
  /* Search for the required lock. Either a write-lock on root-page iTab, a 

  }
    iTab = iRoot;
  }else{
    }
      }
        iTab = pIdx->pTable->tnum;
        }
          return 1;
          ** useful in that case. */
          ** be imposter tables.  So just return true.  The assert is not
          /* Two or more indexes share the same root page.  There must
        if( iTab ){
      if( pIdx->tnum==(int)iRoot ){
      Index *pIdx = (Index *)sqliteHashData(p);
    for(p=sqliteHashFirst(&pSchema->idxHash); p; p=sqliteHashNext(p)){
    HashElem *p;
  if( isIndex ){
  ** table.  */
  ** written. For index b-trees, it is the root page of the associated
  ** b-trees, this is just the root page of the b-tree being read or
  /* Figure out the root-page that the lock should be held on. For table

  }
    return 1;
  if( isIndex && (!pSchema || (pSchema->schemaFlags&DB_SchemaLoaded)==0) ){
  */
  ** This case does not come up very often anyhow.
  ** the correct locks are held.  So do not bother - just return true.
  ** not loaded, then it is too difficult to actually check to see if
  /* If the client is reading  or writing an index and the schema is

  }
    return 1;
  ){
   || (eLockType==READ_LOCK && (pBtree->db->flags & SQLITE_ReadUncommit))
  if( (pBtree->sharable==0)
  */
  ** Return true immediately.
  ** and has the read-uncommitted flag set, then no lock is required. 
  /* If this database is not shareable, or if the client is reading

  BtLock *pLock;
  Pgno iTab = 0;
  Schema *pSchema = (Schema *)pBtree->pBt->pSchema;
){
  int eLockType          /* Required lock type (READ_LOCK or WRITE_LOCK) */
  int isIndex,           /* True if iRoot is the root of an index b-tree */
  Pgno iRoot,            /* Root page of b-tree */
  Btree *pBtree,         /* Handle that must hold lock */
static int hasSharedCacheTableLock(
*/
** acceptable.
** hold a write-lock on the schema table (root page 1). This is also
** Instead of a lock on the table/index rooted at page iRoot, the caller may
**
** function has to search through the database schema.
** the table corresponding to the index being written, this
** as this module treats each table as a separate structure. To determine
** the corresponding table. This makes things a bit more complicated,
** caller should have first obtained a lock specifying the root page of
** When writing to an index that resides in a sharable database, the 
**
**    assert( hasSharedCacheTableLock(pBtree, iRoot, 0, WRITE_LOCK) );
**
** Btree connection pBtree:
** For example, when writing to a table with root-page iRoot via 
**
** table with root page iRoot.   Return 1 if it does and 0 if not.
** Check to see if pBtree holds the required locks to read or write to the 
**
**** This function is only used as part of an assert() statement. ***
/*
#ifdef SQLITE_DEBUG

#ifndef SQLITE_OMIT_SHARED_CACHE

#endif
# define SQLITE_CORRUPT_PAGE(pMemPage) SQLITE_CORRUPT_PGNO(pMemPage->pgno)
#else
# define SQLITE_CORRUPT_PAGE(pMemPage) corruptPageError(__LINE__, pMemPage)
}
  return SQLITE_CORRUPT_BKPT;
  sqlite3_free(zMsg);
  }
    sqlite3ReportError(SQLITE_CORRUPT, lineno, zMsg);
  if( zMsg ){
  sqlite3EndBenignMalloc();
  );
      (int)p->pgno, sqlite3PagerFilename(p->pBt->pPager, 0)
  zMsg = sqlite3_mprintf("database corruption page %d of %s",
  sqlite3BeginBenignMalloc();
  char *zMsg;
int corruptPageError(int lineno, MemPage *p){
#ifdef SQLITE_DEBUG
*/
** with the page number and filename associated with the (MemPage*).
** normally produced as a side-effect of SQLITE_CORRUPT_BKPT is augmented
** SQLITE_CORRUPT_BKPT. Or, if SQLITE_DEBUG is set, then the log message
** If SQLITE_DEBUG is not defined, then this macro is equivalent to
**
** (MemPage*) as an argument. The (MemPage*) must not be NULL.
** Implementation of the SQLITE_CORRUPT_PAGE() macro. Takes a single
/*

#endif
  #define hasReadConflicts(a, b) 0
  #define hasSharedCacheTableLock(a,b,c,d) 1
  #define downgradeAllSharedCacheTableLocks(a)
  #define clearAllSharedCacheTableLocks(a)
  #define setSharedCacheTableLock(a,b,c) SQLITE_OK
  #define querySharedCacheTableLock(a,b,c) SQLITE_OK
  */
  ** So define the lock related functions as no-ops.
  ** of each BtShared structure and so this locking is not necessary. 
  ** shared-cache feature disabled, then there is only ever one user
  ** shared-cache table level locks. If the library is compiled with the
  ** manipulate entries in the BtShared.pLock linked list used to store
  ** and clearAllSharedCacheTableLocks()
  ** The functions querySharedCacheTableLock(), setSharedCacheTableLock(),
  /*
#ifdef SQLITE_OMIT_SHARED_CACHE



#endif
}
  return SQLITE_OK;
  sqlite3GlobalConfig.sharedCacheEnabled = enable;
int sqlite3_enable_shared_cache(int enable){
*/
** sqlite3_open(), sqlite3_open16(), or sqlite3_open_v2().
** The shared cache setting effects only future calls to
** This routine has no effect on existing database connections.
**
** Enable or disable the shared pager and schema features.
/*
#ifndef SQLITE_OMIT_SHARED_CACHE

#endif /* SQLITE_OMIT_SHARED_CACHE */
#endif
static BtShared *SQLITE_WSD sqlite3SharedCacheList = 0;
#else
BtShared *SQLITE_WSD sqlite3SharedCacheList = 0;
#ifdef SQLITE_TEST
*/
** Access to this variable is protected by SQLITE_MUTEX_STATIC_MASTER.
**
** test builds.
** but the test harness needs to access it so we make it global for 
** in shared cache.  This variable has file scope during normal builds,
** A list of BtShared objects that are eligible for participation
/*
#ifndef SQLITE_OMIT_SHARED_CACHE

#endif
#define IfNotOmitAV(expr) 0
#else
#define IfNotOmitAV(expr) (expr)
#ifndef SQLITE_OMIT_AUTOVACUUM
*/
**   bIncrVacuum = IfNotOmitAV(pBtShared->incrVacuum);
**
** defined, or 0 if it is. For example:
** Macro IfNotOmitAV(x) returns (x) if SQLITE_OMIT_AUTOVACUUM is not 
/*

#define BTALLOC_LE    2           /* Allocate any page <= the parameter */
#define BTALLOC_EXACT 1           /* Allocate exact page if possible */
#define BTALLOC_ANY   0           /* Allocate any page */
*/
** Values passed as the 5th argument to allocateBtreePage()
/*

#define get2byteNotZero(X)  (((((int)get2byte(X))-1)&0xffff)+1)
*/
** This routine makes the necessary adjustment to 65536.
** is empty, the offset should be 65536, but the 2-byte value stores zero.
** from the header of a btree page.  If the page size is 65536 and the page
** This routine is used to extract the "offset to cell content area" value
**
** But if the value is zero, make it 65536.
** Extract a 2-byte big-endian integer from an array of unsigned bytes.
/*

#endif
# define TRACE(X)
#else
# define TRACE(X)  if(sqlite3BtreeTrace){printf X;fflush(stdout);}
int sqlite3BtreeTrace=1;  /* True to enable tracing */
#if 0
*/
** macro.
** Set this global variable to 1 to enable tracing using the TRACE
/*

static const char zMagicHeader[] = SQLITE_FILE_HEADER;
*/
** SQLite database.
** The header string that appears at the beginning of every
/*

#include "btreeInt.h"
*/
** Including a description of file format and an overview of operation.
** See the header comment on "btreeInt.h" for additional information.
** This file implements an external (disk-based) database using BTrees.
*************************************************************************
**
**    May you share freely, never taking more than you give.
**    May you find forgiveness for yourself and forgive others.
**    May you do good and not evil.
**
** a legal notice, here is a blessing:
** The author disclaims copyright to this source code.  In place of
**
** 2004 April 6
/*

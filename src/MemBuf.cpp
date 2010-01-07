// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// MemBuf.cpp: R/C++ interface class library -- Easier R embedding into C++
//
// Copyright (C) 2009 - 2010 Dirk Eddelbuettel
//
// This file is part of RInside.
//
// RInside is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// RInside is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with RInside.  If not, see <http://www.gnu.org/licenses/>.

#include <iostream>
#include <cstdlib>
#include <cstring>

#include "MemBuf.h"

extern bool verbose;
extern const char *programName;

MemBuf::~MemBuf() {
    if (verbose) std::cout << "MemBuf::dtor BEGIN" << std::endl;
    free(p_m);
    if (verbose) std::cout << "MemBuf::dtor END" << std::endl;
}

MemBuf::MemBuf(int sizebytes) {
    if (verbose) std::cout << "MemBuf::ctor BEGIN" << std::endl;
    p_m = (membuf_st*) malloc(sizebytes+sizeof(struct membuf_st));

    if (p_m == NULL) {
	std::cerr << programName << ": MemBuf ctor failed! Exiting!!!\n\n" << std::endl;
	exit(1);
	//return NULL; /* unreached */
    }

    p_m->size = sizebytes;
    p_m->count = 0;
    p_m->buf = (unsigned char *)p_m+sizeof(struct membuf_st);
    
    if (verbose) std::cout << "MemBuf::ctor END" << std::endl;
}

void MemBuf::resize() {		// Use power of 2 resizing 
    membuf_t lb = p_m;
    lb = p_m = (membuf_st*) realloc(lb, lb->size*2+sizeof(struct membuf_st));
    if (lb == NULL) {
	std::cerr << programName << ": MemBuff::resize() failed! Exiting!!!\n\n" << std::endl;
	exit(1);
    }
    lb->size =  lb->size * 2;
    lb->buf = (unsigned char *)lb+sizeof(struct membuf_st);
}

void MemBuf::rewind(){
    p_m->count = 0;
}

void MemBuf::add(char *buf){
    membuf_t mb = p_m;
    int buflen = strlen(buf);

    while ((buflen + (mb->count)) >= mb->size) {
	resize();
	mb = p_m;
    }

    memcpy(mb->buf + mb->count, buf, buflen);
    mb->buf[mb->count+buflen] = '\0';
    mb->count += buflen;
}


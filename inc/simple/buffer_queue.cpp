#include <deque>
#include <algorithm>

#include "buffer.h"

struct	buffer_queue::Data {
    bool				buffer_can_merge;
    size_t		        buffer_capacity;
    std::deque<buffer*>	used_list;
    std::deque<buffer*>	free_list;

    Data(bool bCanMergeBuffer, size_t nMaxBufferSize)
        :	buffer_can_merge(bCanMergeBuffer), buffer_capacity(nMaxBufferSize) {
    }

    ~Data() {
        std::deque<buffer*>::iterator	it		= used_list.begin();
        std::deque<buffer*>::iterator	it_end	= used_list.end();
        for(; it != it_end; ++it) {
            delete	*it;
        }

        it		= free_list.begin();
        it_end	= free_list.end();
        for(; it != it_end; ++it) {
            delete	*it;
        }
    }

    void	pack() {
        while(free_list.size() > 10) {
            buffer*	pBuffer	= free_list.front();
            free_list.pop_front();
            delete	pBuffer;
        }
    }
};

buffer_queue::buffer_queue(bool bCanMergeBuffer, size_t nMaxBufferSize) {
    m_pData		= new Data(bCanMergeBuffer, nMaxBufferSize);
}

buffer_queue::~buffer_queue() {
    delete		m_pData;
}

void buffer_queue::clear() {
    std::deque<buffer*>::iterator	it		= m_pData->used_list.begin();
    std::deque<buffer*>::iterator	it_end	= m_pData->used_list.end();
    for(; it != it_end; ++it) {
        (*it)->clear();
        m_pData->free_list.push_back(*it);
    }
    m_pData->used_list.clear();
}

buffer* buffer_queue::create() {
    buffer* pBuffer;
    if(m_pData->free_list.empty()) {
        pBuffer	= new buffer(m_pData->buffer_capacity);
    } else {
        pBuffer	= m_pData->free_list.front();
        m_pData->free_list.pop_front();
    }

    pBuffer->clear();
    return	pBuffer;
}

void buffer_queue::destroy(buffer* buf) {
    if(0 == buf) {
        return;
    }

    if(std::find(m_pData->used_list.begin(),  m_pData->used_list.end(), buf) != m_pData->used_list.end()) {
        return;
    }

    if(std::find(m_pData->free_list.begin(),  m_pData->free_list.end(), buf) != m_pData->free_list.end()) {
        return;
    }

    buf->clear();
    m_pData->free_list.push_back(buf);
    m_pData->pack();
}

bool buffer_queue::push(buffer* buf) {
    assert(buf->good());
    if(buf->failure()) {
        buf->clear();
        m_pData->free_list.push_back(buf);
        return	false;
    }

    buf->rewind();
    if(m_pData->used_list.empty()) {
        m_pData->used_list.push_back(buf);
        return	true;
    }

    buffer*	pBacker	= m_pData->used_list.back();
    if(		m_pData->buffer_can_merge
            &&	0 == pBacker->pos()
            &&	pBacker->size() + buf->size() <= pBacker->capacity()
      ) {
        //	merge to the last one.
        pBacker->read(0, pBacker->size());
        pBacker->write(buf->data(), buf->size());
        pBacker->rewind();

        buf->clear();
        m_pData->free_list.push_back(buf);
    } else {
        m_pData->used_list.push_back(buf);
    }

    return	true;
}

bool buffer_queue::get(buffer*& buf) {
    while(!m_pData->used_list.empty()) {
        buffer*	pBuffer	= m_pData->used_list.front();
        bool	buf_used= false;
        if(!m_pData->buffer_can_merge) {
            buf_used	= (pBuffer->pos() > 0);
        } else {
            buf_used	= (pBuffer->pos() >= pBuffer->size());
        }
        if(buf_used) {
            m_pData->used_list.pop_front();
            pBuffer->clear();
            m_pData->free_list.push_back(pBuffer);
            continue;
        }

        buf	= pBuffer;
        break;
    }

    return	!m_pData->used_list.empty();
}

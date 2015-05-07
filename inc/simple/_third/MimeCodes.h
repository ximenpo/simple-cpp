/*
 * MimeCodes.h - MIME Encoding and decoding filters, using STL-like iterators.
 */

#ifndef MIME_CODES_H
#define MIME_CODES_H


/******************************************************************************
 * MimeCoder -  Abstract base class for MIME filters.
 ******************************************************************************/
template <class InIter, class OutIter>
class MimeCoder
{
public:
    virtual OutIter Filter( OutIter out, InIter inBeg, InIter inEnd ) = 0;
    virtual OutIter Finish( OutIter out ) = 0;
};

/******************************************************************************
 * Base64
 ******************************************************************************/
template <class InIter, class OutIter>
class Base64Encoder : public MimeCoder<InIter, OutIter>
{
public:
    Base64Encoder() : its3Len(0), itsLinePos(0) {}
    virtual OutIter Filter( OutIter out, InIter inBeg, InIter inEnd );
    virtual OutIter Finish( OutIter out );
private:
    int             itsLinePos;
    unsigned char   itsCurr3[3];
    int             its3Len;
    void EncodeCurr3( OutIter& out );
};

template <class InIter, class OutIter>
class Base64Decoder : public MimeCoder<InIter, OutIter>
{
public:
    Base64Decoder() : its4Len(0), itsEnded(0) {}
    virtual OutIter Filter( OutIter out, InIter inBeg, InIter inEnd );
    virtual OutIter Finish( OutIter out );
private:
    int             itsEnded;
    unsigned char   itsCurr4[4];
    int             its4Len;
    int             itsErrNum;
    void DecodeCurr4( OutIter& out );
};

template <class InIter, class OutIter>
OutIter base64_encode( InIter inBeg, InIter inEnd, OutIter out )
{
	Base64Encoder<InIter, OutIter> e;
	return e.Finish( e.Filter(out, inBeg, inEnd) );
}

template <class InIter, class OutIter>
OutIter base64_decode( InIter inBeg, InIter inEnd, OutIter out )
{
	Base64Decoder<InIter, OutIter> d;
	return d.Finish( d.Filter(out, inBeg, inEnd) );
}

/******************************************************************************
 * Quoted-Printable
 ******************************************************************************/
template <class InIter, class OutIter>
class QpEncoder : public MimeCoder<InIter, OutIter>
{
public:
    QpEncoder() : itsLinePos(0), itsPrevCh('x') {}
    virtual OutIter Filter( OutIter out, InIter inBeg, InIter inEnd );
    virtual OutIter Finish( OutIter out );
private:
    int             itsLinePos;
    unsigned char   itsPrevCh;
};

template <class InIter, class OutIter>
class QpDecoder : public MimeCoder<InIter, OutIter>
{
public:
    QpDecoder() : itsHexLen(0) {}
    virtual OutIter Filter( OutIter out, InIter inBeg, InIter inEnd );
    virtual OutIter Finish( OutIter out );
private:
    int             itsHexLen;
    unsigned char   itsHex[2];
};

template <class InIter, class OutIter>
OutIter qp_encode( InIter inBeg, InIter inEnd, OutIter out )
{
	QpEncoder<InIter, OutIter> e;
	return e.Finish( e.Filter(out, inBeg, inEnd) );
}

template <class InIter, class OutIter>
OutIter qp_decode( InIter inBeg, InIter inEnd, OutIter out )
{
	QpDecoder<InIter, OutIter> d;
	return d.Finish( d.Filter(out, inBeg, inEnd) );
}


#endif // MIME_CODES_H

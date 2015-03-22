#ifndef __MIME_H__
#define __MIME_H__

namespace mime {

/*
 * MimeCoder -  Abstract base class for MIME filters.
 */
template <class InputIterator, class OutputIterator>
class MimeCoder {
public:
  virtual OutputIterator filter(InputIterator first, InputIterator last, OutputIterator result, bool fin =false) = 0;
  virtual OutputIterator finish(OutputIterator& result) = 0;
};

/*
 * Base64
 */
template <class InputIterator, class OutputIterator>
class Base64Encoder : public MimeCoder<InputIterator, OutputIterator> {
public:
  Base64Encoder() : len(0), linepos(0) {}
  virtual OutputIterator filter(InputIterator first, InputIterator last, OutputIterator result, bool fin =false );
  virtual OutputIterator finish(OutputIterator& result );
private:
  int             linepos;
  unsigned char   curr[3];
  int             len;
  void encodeCurr(OutputIterator& result);
};

template <class InputIterator, class OutputIterator>
class Base64Decoder : public MimeCoder<InputIterator, OutputIterator> {
public:
  Base64Decoder() : len(0), ended(false) {}
  virtual OutputIterator filter(InputIterator first, InputIterator last, OutputIterator result, bool fin =false );
  virtual OutputIterator finish(OutputIterator& result);
private:
  bool            ended;
  unsigned char   curr[4];
  int             len;
  int             err;
  void decodeCurr(OutputIterator& result);
};

/*
 * Quoted-Printable
 */
template <class InputIterator, class OutputIterator>
class QpEncoder : public MimeCoder<InputIterator, OutputIterator> {
public:
  QpEncoder() : linepos(0), prevCh('x') {}
  virtual OutputIterator filter(InputIterator first, InputIterator last, OutputIterator result, bool fin =false);
  virtual OutputIterator finish(OutputIterator& result);
private:
  int             linepos;
  unsigned char   prevCh;
};

template <class InputIterator, class OutputIterator>
class QpDecoder : public MimeCoder<InputIterator, OutputIterator> {
public:
  QpDecoder() : hexlen(0) {}
  virtual OutputIterator filter(InputIterator first, InputIterator last, OutputIterator result, bool fin =false );
  virtual OutputIterator finish(OutputIterator& result);
private:
  int             hexlen;
  unsigned char   hex2[2];
};

}

#include "mime.cc"

#endif

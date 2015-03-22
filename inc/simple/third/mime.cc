namespace mime {

static const int cLineLen = 72;

/*
 * Base64Encoder
 */
static const char cBase64Codes[] =
  "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

template <class InputIterator, class OutputIterator>
OutputIterator Base64Encoder<InputIterator, OutputIterator>::filter(InputIterator first, InputIterator last, OutputIterator result, bool fin) {
  for(;;) {
    for(; linepos < cLineLen; linepos += 4) {
      for (; len < 3; len++) {
        if ( first == last ) {
          if ( fin )
            finish(result);
            return result;
        }
        curr[len] = *first;
        ++first;
      }
      encodeCurr(result);
      len = 0;
    }
    *result++ = 13;
    *result++ = 10;
    linepos = 0;
  } // for (;;)
}

template <class InputIterator, class OutputIterator>
OutputIterator Base64Encoder<InputIterator, OutputIterator>::finish(OutputIterator& result) {
  if ( len )
    encodeCurr(result);
  len = 0;
  linepos = 0;
  return result;
}

template <class InputIterator, class OutputIterator>
void Base64Encoder<InputIterator, OutputIterator>::encodeCurr(OutputIterator& result) {
  if ( len < 3 )
    curr[len] = 0;
  *result++ = cBase64Codes[ curr[0] >> 2 ];
  *result++ = cBase64Codes[ ((curr[0] & 0x03) << 4) |
                            ((curr[1] & 0xF0) >> 4) ];
  if ( len == 1 )
    *result++ = '=';
  else
    *result++ = cBase64Codes[ ((curr[1] & 0x0F) << 2) |
                              ((curr[2] & 0xC0) >> 6) ];
  if ( len < 3 )
    *result++ = '=';
  else
    *result++ = cBase64Codes[ curr[2] & 0x3F ];
}

/*
 * Base64Decoder
 */
#define XX 127

static const unsigned char cIndex64[256] = {
    XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
    XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
    XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,62, XX,XX,XX,63,
    52,53,54,55, 56,57,58,59, 60,61,XX,XX, XX,XX,XX,XX,
    XX, 0, 1, 2,  3, 4, 5, 6,  7, 8, 9,10, 11,12,13,14,
    15,16,17,18, 19,20,21,22, 23,24,25,XX, XX,XX,XX,XX,
    XX,26,27,28, 29,30,31,32, 33,34,35,36, 37,38,39,40,
    41,42,43,44, 45,46,47,48, 49,50,51,XX, XX,XX,XX,XX,
    XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
    XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
    XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
    XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
    XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
    XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
    XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
    XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
};

template <class InputIterator, class OutputIterator>
OutputIterator Base64Decoder<InputIterator, OutputIterator>::filter(InputIterator first, InputIterator last, OutputIterator result, bool fin) {
  unsigned char c;
  err = 0;
  for (;;) {
    while ( len < 4) {
      if ( first == last ) {
        if ( fin )
        finish(result);
        return result;
      }
      c = *first;
      if ((cIndex64[c] != XX) || (c == '='))
        curr[len++] = c;
      else
      if ((c != 13) && (c != 10))
        ++err; // error
      ++first;
    }
    decodeCurr(result);
    len = 0;
  }
}

template <class InputIterator, class OutputIterator>
OutputIterator Base64Decoder<InputIterator, OutputIterator>::finish(OutputIterator& result) {
  len = 0;
  if ( ended )
    return result;
  else {
    ended = false;
    return result;
  }
}

template <class InputIterator, class OutputIterator>
void Base64Decoder<InputIterator, OutputIterator>::decodeCurr(OutputIterator& result) {
  if ( ended ) {
    ++err;
    ended = false;
  }
  for (int i = 0; i < 2; i++)
    if ( curr[i] == '=') {
      ++err;
      return;
    } else
      curr[i] = cIndex64[curr[i]];
  *result++ = (curr[0] << 2) | ((curr[1] & 0x30) >> 4);
  if ( curr[2] == '=') {
    if ( curr[3] == '=')
      ended = true;
    else
      ++err;
  } else {
    curr[2] = cIndex64[curr[2]];
    *result++ = ((curr[1] & 0x0F) << 4) | ((curr[2] & 0x3C) >> 2);
    if ( curr[3] == '=' )
      ended = true;
    else
      *result++ = ((curr[2] & 0x03) << 6) | cIndex64[curr[3]];
  }
}

/*
 * QpEncoder
 */
static const char cBasisHex[] = "0123456789ABCDEF";

template <class InputIterator, class OutputIterator>
OutputIterator QpEncoder<InputIterator, OutputIterator>::filter(InputIterator first, InputIterator last, OutputIterator result, bool fin) {
  unsigned char c;
  for (; first != last; ++first ) {
    c = *first;
    if (c == '\n') {
      if ( prevCh == ' ' || prevCh == '\t') {
        *result++ = '='; // soft & hard lines
        *result++ = c;
      }
      *result++ = c;
      linepos = 0;
      prevCh = c;
    } else
    if ( (c < 32 && c != '\t') || (c == '=') || (c >= 127)
          || ( linepos == 0 && c == '.') ) {
      *result++ = '=';
      *result++ = cBasisHex[c >> 4];
      *result++ = cBasisHex[c & 0xF];
      linepos += 3;
      prevCh = 'A';
    } else { // printable characters
      *result++ = prevCh = c;
      ++linepos;
    }

    if ( linepos > cLineLen) {
      *result++ = '=';
      *result++ = prevCh = '\n';
      linepos = 0;
    }
  }
  if ( fin )
    finish(result);
  return result;
}

template <class InputIterator, class OutputIterator>
OutputIterator QpEncoder<InputIterator, OutputIterator>::finish(OutputIterator& result) {
  if ( linepos ) {
    *result++ = '=';
    *result++ = '\n';
  }
  linepos = 0;
  prevCh = 'x';
  return result;
}

/*
 * QpDecoder
 */
static const unsigned char cIndexHex[256] = {
  XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
  XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
  XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
   0, 1, 2, 3,  4, 5, 6, 7,  8, 9,XX,XX, XX,XX,XX,XX,
  XX,10,11,12, 13,14,15,XX, XX,XX,XX,XX, XX,XX,XX,XX,
  XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
  XX,10,11,12, 13,14,15,XX, XX,XX,XX,XX, XX,XX,XX,XX,
  XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
  XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
  XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
  XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
  XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
  XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
  XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
  XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
  XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
};

template <class InputIterator, class OutputIterator>
OutputIterator QpDecoder<InputIterator, OutputIterator>::filter(InputIterator first, InputIterator last, OutputIterator result, bool fin) {
  unsigned char c, c1, c2;
  int errn = 0;

  for (; first != last; ++first) {
    if ( hexlen ) {
      if (*first == '\n')
        hexlen = 0;
      else {
        hex2[hexlen-1] = *first;
        if ( hexlen++ == 2) {
          if (XX == (c1 = cIndexHex[hex2[0]]))
            ++errn;
          if (XX == (c2 = cIndexHex[hex2[1]]))
            ++errn;
          c = (c1 << 4) | c2;
          if (c != '\r')
            *result++ = c;
          hexlen = 0;
        }
      }
    } else
    if (*first == '=')
      hexlen = 1;
    else
      *result++ = *first;
  }

  if ( fin )
    finish(result);
  return result;
}

template <class InputIterator, class OutputIterator>
OutputIterator QpDecoder<InputIterator, OutputIterator>::finish(OutputIterator& result) {
  if ( hexlen ) { // error
    hexlen = 0;
    return result;
  }
  return result;
}

#undef XX

}

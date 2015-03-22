/*
MIT License

Combination Library version 1.5

Copyright (c) 2007 Wong Shao Voon

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
*/

#include <string>

#ifndef __FINDTOTALCOMB_H__
#define __FINDTOTALCOMB_H__

#if	defined(__GNUC__)
#	include	"bigint/BigIntegerUtils.hh"
#endif

#define FACTMAX  100

template <typename T>
class CFindTotalComb {
public:
    // Constructor
    CFindTotalComb() : m_Data((long)0) {};

    // Copy Constructor
    CFindTotalComb( const CFindTotalComb<T> &ftc ) {
        this->m_Data = ftc.m_Data;
    };

    // Constructor
    CFindTotalComb( T& Data ) {
        m_Data = Data;
    };

    CFindTotalComb( int nData ) {
        m_Data = nData;
    };

    // Destructor
    ~CFindTotalComb() {};

    // FindTotalComb function
    CFindTotalComb<T> &FindTotalComb( unsigned int Set, unsigned int Comb );

    T GetResult();

    // Overloaded operators
    ///////////////////////////////////////////
    const CFindTotalComb &operator=( const CFindTotalComb<T> &ftc );

    bool operator==( const CFindTotalComb<T> &ftc );

    bool operator>( const CFindTotalComb<T> &ftc );

    bool operator<( const CFindTotalComb<T> &ftc );

    const CFindTotalComb &operator+=( const CFindTotalComb<T> &ftc );

    const CFindTotalComb &operator-=( const CFindTotalComb<T> &ftc );

    static void Init();

    static void Destroy();

    static bool IsInit() {
        return m_bInit;
    }

    static bool GetFactorial( unsigned int fact, T& biResults );

    static bool ComputeFactorial( unsigned int fact, T& biResults );

private:

    T FindFactorial( unsigned int Factor );

    // Range Excluding Min
    T FindRange( const T &Min, const T &Max );

private:
    T m_Data;

    static std::string* m_strFactArray;

    static bool m_bInit;
};

template<typename T>
bool CFindTotalComb<T>::m_bInit=false;

template<typename T>
std::string* CFindTotalComb<T>::m_strFactArray = NULL;

template< typename T >
CFindTotalComb<T> &CFindTotalComb<T>::FindTotalComb( unsigned int Set, unsigned int Comb ) {
    if( Comb > Set )
        throw std::string("Comb > Set");

    if( Comb == Set ) {
        m_Data = 1;
        return *this;
    }

    unsigned int AComb64 = Set - Comb;
    unsigned int Comb64 = Comb;
    unsigned int Set64 = Set;

    if( AComb64 >= Comb64 ) {
        m_Data =
            FindRange( AComb64, Set64 ) / FindFactorial( Comb64 );
    } else {
        m_Data =
            FindRange( Comb64, Set64 ) / FindFactorial( AComb64 );
    }

    return *this;
}


template< typename T >
T CFindTotalComb<T>::GetResult() {
    return m_Data;
}

template< typename T >
const CFindTotalComb<T> &CFindTotalComb<T>::operator=( const CFindTotalComb &ftc ) {
    this->m_Data = ftc.m_Data;

    return *this;
}

template< typename T >
bool CFindTotalComb<T>::operator==( const CFindTotalComb &ftc ) {
    return ( this->m_Data == ftc.m_Data );
}

template< typename T >
bool CFindTotalComb<T>::operator>( const CFindTotalComb &ftc ) {
    return ( this->m_Data > ftc.m_Data );
}

template< typename T >
bool CFindTotalComb<T>::operator<( const CFindTotalComb &ftc ) {
    return ( this->m_Data < ftc.m_Data );
}

template< typename T >
const CFindTotalComb<T> &CFindTotalComb<T>::operator+=( const CFindTotalComb &ftc ) {
    this->m_Data += ftc.m_Data;

    return *this;
}

template< typename T >
const CFindTotalComb<T> &CFindTotalComb<T>::operator-=( const CFindTotalComb &ftc ) {
    this->m_Data -= ftc.m_Data;

    return *this;
}

/*
template< typename T >
T CFindTotalComb<T>::FindFactorial( const T Factor )
{
	T Ans = 1;
	for( T i=2; i<=Factor; ++i )
	{
		Ans *= i;
	}

	return Ans;
}
*/
template< typename T >
T CFindTotalComb<T>::FindFactorial( unsigned int Factor ) {
    T biResults;
    GetFactorial( Factor, biResults );

    return biResults;
}


// Range Excluding Min
template< typename T >
T CFindTotalComb<T>::FindRange( const T &Min, const T &Max ) {
    if(Min == Max)
        return T(1);

    T Ans = Min+1;

    for( T i=Min+2; i<=Max; ++i ) {
        Ans *= i;
    }

    return Ans;
}

template<typename T>
void CFindTotalComb<T>::Init() {
    if( m_bInit )
        return;

    m_strFactArray = new std::string[FACTMAX+1];

    m_strFactArray[0] = "1";
    m_strFactArray[1] = "1";
    m_strFactArray[2] = "2";
    m_strFactArray[3] = "6";
    m_strFactArray[4] = "24";
    m_strFactArray[5] = "120";
    m_strFactArray[6] = "720";
    m_strFactArray[7] = "5040";
    m_strFactArray[8] = "40320";
    m_strFactArray[9] = "362880";
    m_strFactArray[10] = "3628800";
    m_strFactArray[11] = "39916800";
    m_strFactArray[12] = "479001600";
    m_strFactArray[13] = "6227020800";
    m_strFactArray[14] = "87178291200";
    m_strFactArray[15] = "1307674368000";
    m_strFactArray[16] = "20922789888000";
    m_strFactArray[17] = "355687428096000";
    m_strFactArray[18] = "6402373705728000";
    m_strFactArray[19] = "121645100408832000";
    m_strFactArray[20] = "2432902008176640000";
    m_strFactArray[21] = "51090942171709440000";
    m_strFactArray[22] = "1124000727777607680000";
    m_strFactArray[23] = "25852016738884976640000";
    m_strFactArray[24] = "620448401733239439360000";
    m_strFactArray[25] = "15511210043330985984000000";
    m_strFactArray[26] = "403291461126605635584000000";
    m_strFactArray[27] = "10888869450418352160768000000";
    m_strFactArray[28] = "304888344611713860501504000000";
    m_strFactArray[29] = "8841761993739701954543616000000";
    m_strFactArray[30] = "265252859812191058636308480000000";
    m_strFactArray[31] = "8222838654177922817725562880000000";
    m_strFactArray[32] = "263130836933693530167218012160000000";
    m_strFactArray[33] = "8683317618811886495518194401280000000";
    m_strFactArray[34] = "295232799039604140847618609643520000000";
    m_strFactArray[35] = "10333147966386144929666651337523200000000";
    m_strFactArray[36] = "371993326789901217467999448150835200000000";
    m_strFactArray[37] = "13763753091226345046315979581580902400000000";
    m_strFactArray[38] = "523022617466601111760007224100074291200000000";
    m_strFactArray[39] = "20397882081197443358640281739902897356800000000";
    m_strFactArray[40] = "815915283247897734345611269596115894272000000000";
    m_strFactArray[41] = "33452526613163807108170062053440751665152000000000";
    m_strFactArray[42] = "1405006117752879898543142606244511569936384000000000";
    m_strFactArray[43] = "60415263063373835637355132068513997507264512000000000";
    m_strFactArray[44] = "2658271574788448768043625811014615890319638528000000000";
    m_strFactArray[45] = "119622220865480194561963161495657715064383733760000000000";
    m_strFactArray[46] = "5502622159812088949850305428800254892961651752960000000000";
    m_strFactArray[47] = "258623241511168180642964355153611979969197632389120000000000";
    m_strFactArray[48] = "12413915592536072670862289047373375038521486354677760000000000";
    m_strFactArray[49] = "608281864034267560872252163321295376887552831379210240000000000";
    m_strFactArray[50] = "30414093201713378043612608166064768844377641568960512000000000000";
    m_strFactArray[51] = "1551118753287382280224243016469303211063259720016986112000000000000";
    m_strFactArray[52] = "80658175170943878571660636856403766975289505440883277824000000000000";
    m_strFactArray[53] = "4274883284060025564298013753389399649690343788366813724672000000000000";
    m_strFactArray[54] = "230843697339241380472092742683027581083278564571807941132288000000000000";
    m_strFactArray[55] = "12696403353658275925965100847566516959580321051449436762275840000000000000";
    m_strFactArray[56] = "710998587804863451854045647463724949736497978881168458687447040000000000000";
    m_strFactArray[57] = "40526919504877216755680601905432322134980384796226602145184481280000000000000";
    m_strFactArray[58] = "2350561331282878571829474910515074683828862318181142924420699914240000000000000";
    m_strFactArray[59] = "138683118545689835737939019720389406345902876772687432540821294940160000000000000";
    m_strFactArray[60] = "8320987112741390144276341183223364380754172606361245952449277696409600000000000000";
    m_strFactArray[61] = "507580213877224798800856812176625227226004528988036003099405939480985600000000000000";
    m_strFactArray[62] = "31469973260387937525653122354950764088012280797258232192163168247821107200000000000000";
    m_strFactArray[63] = "1982608315404440064116146708361898137544773690227268628106279599612729753600000000000000";
    m_strFactArray[64] = "126886932185884164103433389335161480802865516174545192198801894375214704230400000000000000";
    m_strFactArray[65] = "8247650592082470666723170306785496252186258551345437492922123134388955774976000000000000000";
    m_strFactArray[66] = "544344939077443064003729240247842752644293064388798874532860126869671081148416000000000000000";
    m_strFactArray[67] = "36471110918188685288249859096605464427167635314049524593701628500267962436943872000000000000000";
    m_strFactArray[68] = "2480035542436830599600990418569171581047399201355367672371710738018221445712183296000000000000000";
    m_strFactArray[69] = "171122452428141311372468338881272839092270544893520369393648040923257279754140647424000000000000000";
    m_strFactArray[70] = "11978571669969891796072783721689098736458938142546425857555362864628009582789845319680000000000000000";
    m_strFactArray[71] = "850478588567862317521167644239926010288584608120796235886430763388588680378079017697280000000000000000";
    m_strFactArray[72] = "61234458376886086861524070385274672740778091784697328983823014963978384987221689274204160000000000000000";
    m_strFactArray[73] = "4470115461512684340891257138125051110076800700282905015819080092370422104067183317016903680000000000000000";
    m_strFactArray[74] = "330788544151938641225953028221253782145683251820934971170611926835411235700971565459250872320000000000000000";
    m_strFactArray[75] = "24809140811395398091946477116594033660926243886570122837795894512655842677572867409443815424000000000000000000";
    m_strFactArray[76] = "1885494701666050254987932260861146558230394535379329335672487982961844043495537923117729972224000000000000000000";
    m_strFactArray[77] = "145183092028285869634070784086308284983740379224208358846781574688061991349156420080065207861248000000000000000000";
    m_strFactArray[78] = "11324281178206297831457521158732046228731749579488251990048962825668835325234200766245086213177344000000000000000000";
    m_strFactArray[79] = "894618213078297528685144171539831652069808216779571907213868063227837990693501860533361810841010176000000000000000000";
    m_strFactArray[80] = "71569457046263802294811533723186532165584657342365752577109445058227039255480148842668944867280814080000000000000000000";
    m_strFactArray[81] = "5797126020747367985879734231578109105412357244731625958745865049716390179693892056256184534249745940480000000000000000000";
    m_strFactArray[82] = "475364333701284174842138206989404946643813294067993328617160934076743994734899148613007131808479167119360000000000000000000";
    m_strFactArray[83] = "39455239697206586511897471180120610571436503407643446275224357528369751562996629334879591940103770870906880000000000000000000";
    m_strFactArray[84] = "3314240134565353266999387579130131288000666286242049487118846032383059131291716864129885722968716753156177920000000000000000000";
    m_strFactArray[85] = "281710411438055027694947944226061159480056634330574206405101912752560026159795933451040286452340924018275123200000000000000000000";
    m_strFactArray[86] = "24227095383672732381765523203441259715284870552429381750838764496720162249742450276789464634901319465571660595200000000000000000000";
    m_strFactArray[87] = "2107757298379527717213600518699389595229783738061356212322972511214654115727593174080683423236414793504734471782400000000000000000000";
    m_strFactArray[88] = "185482642257398439114796845645546284380220968949399346684421580986889562184028199319100141244804501828416633516851200000000000000000000";
    m_strFactArray[89] = "16507955160908461081216919262453619309839666236496541854913520707833171034378509739399912570787600662729080382999756800000000000000000000";
    m_strFactArray[90] = "1485715964481761497309522733620825737885569961284688766942216863704985393094065876545992131370884059645617234469978112000000000000000000000";
    m_strFactArray[91] = "135200152767840296255166568759495142147586866476906677791741734597153670771559994765685283954750449427751168336768008192000000000000000000000";
    m_strFactArray[92] = "12438414054641307255475324325873553077577991715875414356840239582938137710983519518443046123837041347353107486982656753664000000000000000000000";
    m_strFactArray[93] = "1156772507081641574759205162306240436214753229576413535186142281213246807121467315215203289516844845303838996289387078090752000000000000000000000";
    m_strFactArray[94] = "108736615665674308027365285256786601004186803580182872307497374434045199869417927630229109214583415458560865651202385340530688000000000000000000000";
    m_strFactArray[95] = "10329978488239059262599702099394727095397746340117372869212250571234293987594703124871765375385424468563282236864226607350415360000000000000000000000";
    m_strFactArray[96] = "991677934870949689209571401541893801158183648651267795444376054838492222809091499987689476037000748982075094738965754305639874560000000000000000000000";
    m_strFactArray[97] = "96192759682482119853328425949563698712343813919172976158104477319333745612481875498805879175589072651261284189679678167647067832320000000000000000000000";
    m_strFactArray[98] = "9426890448883247745626185743057242473809693764078951663494238777294707070023223798882976159207729119823605850588608460429412647567360000000000000000000000";
    m_strFactArray[99] = "933262154439441526816992388562667004907159682643816214685929638952175999932299156089414639761565182862536979208272237582511852109168640000000000000000000000";
    m_strFactArray[100] = "93326215443944152681699238856266700490715968264381621468592963895217599993229915608941463976156518286253697920827223758251185210916864000000000000000000000000";


    m_bInit = true;
}

template<typename T>
void CFindTotalComb<T>::Destroy() {
    if( m_strFactArray ) {
        delete [] m_strFactArray;
        m_strFactArray = NULL;
    }

    m_bInit = false;
}

template<typename T>
bool CFindTotalComb<T>::GetFactorial( unsigned int fact, T& biResults ) {
    if( !m_bInit ) {
        return ComputeFactorial( fact, biResults );
    }

    if( fact > FACTMAX ) {
        return ComputeFactorial( fact, biResults );
    }

    //biResults = T(m_strFactArray[fact].c_str());
    biResults = stringToBigInteger(m_strFactArray[fact]);

    return true;
}

template<typename T>
bool CFindTotalComb<T>::ComputeFactorial( unsigned int fact, T& biResults ) {
    biResults = 1;

    for( T i=2; i<=fact; ++i ) {
        biResults = biResults * i;
    }

    return true;
}

#endif // __FINDTOTALCOMB_H__
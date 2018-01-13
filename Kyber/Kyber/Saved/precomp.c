#include "params.h"
#include <stdint.h>

/* Precomputed constants for the forward NTT and inverse NTT.
 * Computed using Pari/GP as follows:
 *
brv=[0,128,64,192,32,160,96,224,16,144,80,208,48,176,112,240, \
     8,136,72,200,40,168,104,232,24,152,88,216,56,184,120,248, \
     4,132,68,196,36,164,100,228,20,148,84,212,52,180,116,244, \
     12,140,76,204,44,172,108,236,28,156,92,220,60,188,124,252, \
     2,130,66,194,34,162,98,226,18,146,82,210,50,178,114,242, \
     10,138,74,202,42,170,106,234,26,154,90,218,58,186,122,250, \
     6,134,70,198,38,166,102,230,22,150,86,214,54,182,118,246, \
     14,142,78,206,46,174,110,238,30,158,94,222,62,190,126,254, \
     1,129,65,193,33,161,97,225,17,145,81,209,49,177,113,241, \
     9,137,73,201,41,169,105,233,25,153,89,217,57,185,121,249, \
     5,133,69,197,37,165,101,229,21,149,85,213,53,181,117,245, \
     13,141,77,205,45,173,109,237,29,157,93,221,61,189,125,253, \
     3,131,67,195,35,163,99,227,19,147,83,211,51,179,115,243, \
     11,139,75,203,43,171,107,235,27,155,91,219,59,187,123,251, \
     7,135,71,199,39,167,103,231,23,151,87,215,55,183,119,247, \
     15,143,79,207,47,175,111,239,31,159,95,223,63,191,127,255];

q = 7681;
n = 256;
mont = Mod(2^18,q);

g=0; for(i=2,q-1,if(znorder(Mod(i,q)) == 2*n, g=Mod(i,q); break))

zetas = lift(vector(n, i, g^(brv[i])*mont))
omegas_inv_bitrev_montgomery = lift(vector(n/2, i, (g^2)^(-brv[2*(i-1)+1])*mont))
psis_inv_montgomery = lift(vector(n, i, g^(-(i-1))/n*mont))

*/

const uint16_t zetas[KYBER_N] =
{
	0x03DEU, 0x1D03U, 0x0A4AU, 0x1AA3U, 0x0242U, 0x0CD1U, 0x085FU, 0x0447U,
	0x01E4U, 0x18DAU, 0x0D08U, 0x1506U, 0x17C6U, 0x0EEFU, 0x036DU, 0x1618U,
	0x0DFFU, 0x1B62U, 0x190EU, 0x0107U, 0x0505U, 0x0123U, 0x1BE7U, 0x1CAAU,
	0x062DU, 0x140EU, 0x1440U, 0x172CU, 0x0FCAU, 0x168FU, 0x09A4U, 0x0003U,
	0x025EU, 0x02D9U, 0x1507U, 0x03C2U, 0x0CA8U, 0x1D7CU, 0x1409U, 0x1DE5U,
	0x1729U, 0x1365U, 0x099DU, 0x0281U, 0x0630U, 0x0A6AU, 0x0476U, 0x009DU,
	0x1CEFU, 0x1466U, 0x15E2U, 0x1416U, 0x17FCU, 0x156DU, 0x1343U, 0x0617U,
	0x0825U, 0x14A4U, 0x0808U, 0x0DD2U, 0x1C65U, 0x0DCFU, 0x1C16U, 0x07A5U,
	0x0D89U, 0x1A88U, 0x0602U, 0x1238U, 0x07E7U, 0x1DDBU, 0x0E4CU, 0x1DF9U,
	0x069EU, 0x1AF9U, 0x0F9BU, 0x0D93U, 0x1733U, 0x0743U, 0x1AFEU, 0x1152U,
	0x03FBU, 0x05D4U, 0x1BAFU, 0x1299U, 0x0291U, 0x12FBU, 0x16A6U, 0x0A50U,
	0x069DU, 0x0A2FU, 0x0ADEU, 0x1518U, 0x1942U, 0x03F2U, 0x03BDU, 0x0F0BU,
	0x0849U, 0x18F8U, 0x1C97U, 0x0D27U, 0x0E4BU, 0x0D2FU, 0x191EU, 0x1D9FU,
	0x060DU, 0x16E0U, 0x12A5U, 0x17C4U, 0x15A8U, 0x0672U, 0x0F9DU, 0x1126U,
	0x1A42U, 0x0B63U, 0x1095U, 0x0A4BU, 0x17F0U, 0x1DFCU, 0x1669U, 0x0650U,
	0x0D81U, 0x0C3CU, 0x1C1CU, 0x125EU, 0x185FU, 0x0353U, 0x084AU, 0x0BC1U,
	0x1DBDU, 0x1C7FU, 0x07D7U, 0x0143U, 0x13F8U, 0x0E84U, 0x08F1U, 0x192AU,
	0x1B35U, 0x0A99U, 0x1BD6U, 0x0D49U, 0x03C3U, 0x19C4U, 0x025FU, 0x13A3U,
	0x1BA6U, 0x1184U, 0x1731U, 0x03B0U, 0x0B2CU, 0x0A78U, 0x13B9U, 0x06F1U,
	0x16DAU, 0x0D3BU, 0x1957U, 0x1A79U, 0x12CCU, 0x1274U, 0x1BA5U, 0x00BAU,
	0x1AC0U, 0x1A89U, 0x0D87U, 0x16F5U, 0x0496U, 0x1BCCU, 0x0C05U, 0x1739U,
	0x19BFU, 0x024EU, 0x19F3U, 0x0539U, 0x1794U, 0x0F97U, 0x068BU, 0x0805U,
	0x17A7U, 0x048AU, 0x068FU, 0x0F2BU, 0x10D7U, 0x083AU, 0x1813U, 0x1186U,
	0x18E6U, 0x138EU, 0x11E0U, 0x10C0U, 0x143CU, 0x1006U, 0x011AU, 0x17E7U,
	0x1D13U, 0x18BAU, 0x0C70U, 0x136BU, 0x09E2U, 0x14CDU, 0x104BU, 0x1C11U,
	0x1437U, 0x1617U, 0x076AU, 0x017EU, 0x1C2BU, 0x002BU, 0x174DU, 0x17B9U,
	0x06C2U, 0x014CU, 0x0629U, 0x0CE8U, 0x0919U, 0x06A3U, 0x1806U, 0x094BU,
	0x13F9U, 0x014DU, 0x0DAEU, 0x11A5U, 0x05C8U, 0x0494U, 0x15BFU, 0x028BU,
	0x039DU, 0x11DDU, 0x0257U, 0x0557U, 0x100DU, 0x0747U, 0x1B11U, 0x0645U,
	0x0F1AU, 0x0811U, 0x0FD0U, 0x0347U, 0x1684U, 0x098FU, 0x07E6U, 0x0D11U,
	0x07C6U, 0x0FE3U, 0x07F4U, 0x0815U, 0x0DEFU, 0x1CCBU, 0x0940U, 0x0153U,
	0x1B23U, 0x086FU, 0x028EU, 0x1C9FU, 0x0AD0U, 0x1A14U, 0x03DBU, 0x08A6U
};

const uint16_t omegas_inv_bitrev_montgomery[KYBER_N / 2] =
{
	0x03DEU, 0x00FEU, 0x035EU, 0x13B7U, 0x19BAU, 0x15A2U, 0x1130U, 0x1BBFU,
	0x07E9U, 0x1A94U, 0x0F12U, 0x063BU, 0x08FBU, 0x10F9U, 0x0527U, 0x1C1DU,
	0x1DFEU, 0x145DU, 0x0772U, 0x0E37U, 0x06D5U, 0x09C1U, 0x09F3U, 0x17D4U,
	0x0157U, 0x021AU, 0x1CDEU, 0x18FCU, 0x1CFAU, 0x04F3U, 0x029FU, 0x1002U,
	0x165CU, 0x01EBU, 0x1032U, 0x019CU, 0x102FU, 0x15F9U, 0x095DU, 0x15DCU,
	0x17EAU, 0x0ABEU, 0x0894U, 0x0605U, 0x09EBU, 0x081FU, 0x099BU, 0x0112U,
	0x1D64U, 0x198BU, 0x1397U, 0x17D1U, 0x1B80U, 0x1464U, 0x0A9CU, 0x06D8U,
	0x001CU, 0x09F8U, 0x0085U, 0x1159U, 0x1A3FU, 0x08FAU, 0x1B28U, 0x1BA3U,
	0x1240U, 0x15B7U, 0x1AAEU, 0x05A2U, 0x0BA3U, 0x01E5U, 0x11C5U, 0x1080U,
	0x17B1U, 0x0798U, 0x0005U, 0x0611U, 0x13B6U, 0x0D6CU, 0x129EU, 0x03BFU,
	0x0CDBU, 0x0E64U, 0x178FU, 0x0859U, 0x063DU, 0x0B5CU, 0x0721U, 0x17F4U,
	0x0062U, 0x04E3U, 0x10D2U, 0x0FB6U, 0x10DAU, 0x016AU, 0x0509U, 0x15B8U,
	0x0EF6U, 0x1A44U, 0x1A0FU, 0x04BFU, 0x08E9U, 0x1323U, 0x13D2U, 0x1764U,
	0x13B1U, 0x075BU, 0x0B06U, 0x1B70U, 0x0B68U, 0x0252U, 0x182DU, 0x1A06U,
	0x0CAFU, 0x0303U, 0x16BEU, 0x06CEU, 0x106EU, 0x0E66U, 0x0308U, 0x1763U,
	0x0008U, 0x0FB5U, 0x0026U, 0x161AU, 0x0BC9U, 0x17FFU, 0x0379U, 0x1078U
};

const uint16_t psis_inv_montgomery[KYBER_N] = 
{
	0x0400U, 0x136CU, 0x1693U, 0x1AFBU, 0x134FU, 0x1048U, 0x013BU, 0x15CCU,
	0x005AU, 0x01F1U, 0x0463U, 0x008EU, 0x1266U, 0x1597U, 0x098BU, 0x1307U,
	0x02BAU, 0x09B9U, 0x095AU, 0x0FA3U, 0x02ACU, 0x08C1U, 0x0956U, 0x1C38U,
	0x13D0U, 0x0810U, 0x1285U, 0x0697U, 0x1AB9U, 0x01E2U, 0x1D11U, 0x04D3U,
	0x084EU, 0x1286U, 0x0AF2U, 0x1670U, 0x188FU, 0x1345U, 0x02BBU, 0x0E14U,
	0x0511U, 0x084FU, 0x16E1U, 0x06A9U, 0x0F1CU, 0x0EC3U, 0x112DU, 0x0881U,
	0x009FU, 0x026EU, 0x08C0U, 0x04FBU, 0x0280U, 0x1B24U, 0x11DCU, 0x149DU,
	0x00D1U, 0x0A2DU, 0x0485U, 0x1CA0U, 0x16B9U, 0x0C77U, 0x067EU, 0x07D9U,
	0x1300U, 0x023EU, 0x09B7U, 0x00A4U, 0x1835U, 0x1154U, 0x1C59U, 0x0D86U,
	0x10ACU, 0x10B9U, 0x0D56U, 0x11A3U, 0x0C62U, 0x050AU, 0x07D3U, 0x16DFU,
	0x1BF4U, 0x17AEU, 0x1D6BU, 0x06C4U, 0x0CB1U, 0x1314U, 0x1D58U, 0x0E06U,
	0x0419U, 0x084BU, 0x0575U, 0x17CDU, 0x0E6BU, 0x17F2U, 0x198DU, 0x0F6AU,
	0x1872U, 0x0CFAU, 0x06FCU, 0x1091U, 0x1324U, 0x0905U, 0x0578U, 0x06DDU,
	0x0190U, 0x01F6U, 0x1A2AU, 0x0922U, 0x0BC3U, 0x029CU, 0x1D14U, 0x11E4U,
	0x1974U, 0x1A8BU, 0x0B8FU, 0x1028U, 0x0BE0U, 0x08E7U, 0x18D3U, 0x0F67U,
	0x0B61U, 0x19D5U, 0x1CF8U, 0x0FF4U, 0x196CU, 0x15B4U, 0x0FD6U, 0x0EC6U,
	0x003DU, 0x19A7U, 0x08A4U, 0x030BU, 0x0278U, 0x164DU, 0x1623U, 0x133BU,
	0x132FU, 0x1AEDU, 0x1257U, 0x1044U, 0x0DD0U, 0x08EFU, 0x1DAAU, 0x0B20U,
	0x0CC3U, 0x0777U, 0x0C38U, 0x0222U, 0x07C7U, 0x009CU, 0x135EU, 0x159BU,
	0x1AF7U, 0x01E3U, 0x036BU, 0x008AU, 0x00FAU, 0x08BAU, 0x08DAU, 0x1C36U,
	0x0B1AU, 0x10A2U, 0x032CU, 0x1A2FU, 0x00E8U, 0x1457U, 0x19FAU, 0x0A19U,
	0x076CU, 0x1851U, 0x1344U, 0x1C61U, 0x125DU, 0x0C65U, 0x121BU, 0x18F9U,
	0x00E3U, 0x1C91U, 0x1166U, 0x10BCU, 0x1A67U, 0x04C8U, 0x1467U, 0x05A7U,
	0x018BU, 0x0A30U, 0x1196U, 0x0FC5U, 0x00BDU, 0x0D14U, 0x0036U, 0x192BU,
	0x08A2U, 0x1856U, 0x0B0AU, 0x06F4U, 0x0BBAU, 0x176BU, 0x07A3U, 0x17D6U,
	0x179DU, 0x0F62U, 0x139BU, 0x1141U, 0x059AU, 0x04EEU, 0x05E3U, 0x16D7U,
	0x171DU, 0x1BF5U, 0x1C09U, 0x1922U, 0x1D71U, 0x02E5U, 0x10FCU, 0x051DU,
	0x0091U, 0x0176U, 0x08BCU, 0x1190U, 0x13A4U, 0x1A73U, 0x1B0BU, 0x1CFDU,
	0x07BAU, 0x03FFU, 0x0F11U, 0x1ADCU, 0x044EU, 0x1D1BU, 0x1260U, 0x1976U,
	0x0540U, 0x02FDU, 0x0180U, 0x1649U, 0x04B7U, 0x065EU, 0x127EU, 0x061BU,
	0x1AB7U, 0x172DU, 0x07A2U, 0x137BU, 0x1BE6U, 0x16B6U, 0x1D67U, 0x1359U,
	0x1DD5U, 0x1263U, 0x0886U, 0x1666U, 0x0B02U, 0x021DU, 0x1001U, 0x1609U
};


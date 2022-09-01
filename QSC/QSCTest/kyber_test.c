#include "kyber_test.h"
#include "katparser.h"
#include "nistrng.h"
#include "testutils.h"
#include "../QSC/consoleutils.h"
#include "../QSC/csp.h"
#include "../QSC/intutils.h"
#include "../QSC/kyber.h"
#include "../QSC/memutils.h"

bool qsctest_kyber_ciphertext_integrity()
{
	uint8_t ct[QSC_KYBER_CIPHERTEXT_SIZE] = { 0 };
	uint8_t pk[QSC_KYBER_PUBLICKEY_SIZE] = { 0 };
	uint8_t seed[QSCTEST_NIST_RNG_SEED_SIZE] = { 0 };
	uint8_t sk[QSC_KYBER_PRIVATEKEY_SIZE] = { 0 };
	uint8_t ssk1[QSC_KYBER_SHAREDSECRET_SIZE] = { 0 };
	uint8_t ssk2[QSC_KYBER_SHAREDSECRET_SIZE] = { 0 };
	bool res;

	res = true;
	qsctest_hex_to_bin("061550234D158C5EC95595FE04EF7A25767F2E24CC2BC479D09D86DC9ABCFDE7056A8C266F9EF97ED08541DBD2E1FFA1", seed, sizeof(seed));
	
	qsctest_nistrng_prng_initialize(seed, NULL, 0);

	/* generate public and secret keys */
	qsc_kyber_generate_keypair(pk, sk, qsctest_nistrng_prng_generate);

	/* derive a shared-secret key and creates a response (in: pk | out: ct and ss2) */
	qsc_kyber_encapsulate(ssk2, ct, pk, qsctest_nistrng_prng_generate);

	/* change some bytes in the ciphertext */
	if (qsc_csp_generate(ct, 32) != true)
	{
		qsc_consoleutils_print_line("Failure! kyber cipher-text: the random generator has failed -KCT0");
		res = false;
	}

	/* invalid ciphertext, auth should fail */
	if (qsc_kyber_decapsulate(ssk1, ct, sk) == true)
	{
		qsc_consoleutils_print_line("Failure! kyber cipher-text: decapsulation has failed -KCT1");
		res = false;
	}

	/* fail if equal */
	if (qsc_intutils_are_equal8(ssk1, ssk2, QSC_KYBER_SHAREDSECRET_SIZE) == true)
	{
		qsc_consoleutils_print_line("Failure! kyber cipher-text: invalid shared secret -KCT2");
		res = false;
	}

	return res;
}

bool qsctest_kyber_kat_test()
{
	uint8_t ct[QSC_KYBER_CIPHERTEXT_SIZE] = { 0 };
	uint8_t kct[QSC_KYBER_CIPHERTEXT_SIZE] = { 0 };
	uint8_t kpk[QSC_KYBER_PUBLICKEY_SIZE] = { 0 };
	uint8_t ksk[QSC_KYBER_PRIVATEKEY_SIZE] = { 0 };
	uint8_t kss[QSC_KYBER_SHAREDSECRET_SIZE] = { 0 };
	uint8_t pk[QSC_KYBER_PUBLICKEY_SIZE] = { 0 };
	uint8_t seed[QSCTEST_NIST_RNG_SEED_SIZE] = { 0 };
	uint8_t sk[QSC_KYBER_PRIVATEKEY_SIZE] = { 0 };
	uint8_t ss1[QSC_KYBER_SHAREDSECRET_SIZE] = { 0 };
	uint8_t ss2[QSC_KYBER_SHAREDSECRET_SIZE] = { 0 };
	size_t ctlen;
	size_t pklen;
	size_t seedlen;
	size_t sklen;
	size_t sslen;
	bool ret;

	ctlen = 0;
	pklen = 0;
	seedlen = 0;
	sklen = 0;
	sslen = 0;
	ret = true;

#if defined(QSC_KYBER_S3Q3329N256K3)
	char path[] = "NPQCR3/kyber-2400.rsp";
#elif defined(QSC_KYBER_S5Q3329N256K4)
	char path[] = "NPQCR3/kyber-3168.rsp";
#elif defined(QSC_KYBER_S6Q3329N256K5)
	qsctest_hex_to_bin("061550234D158C5EC95595FE04EF7A25767F2E24CC2BC479D09D86DC9ABCFDE7056A8C266F9EF97ED08541DBD2E1FFA1", seed, sizeof(seed));
	qsctest_hex_to_bin("8DBB0F30FB24A87ABFBD58B0BD085CC4C80BA5042B2DD67CEBBE1D09493129C7C691B857B7331BFC82BD8F337F351F4B661B716179D61DE37375C5DAA6471FE8"
		"1F98CF422054035DBFD8446CA709DFCAE9FF003997AE811F5FF4F3FAD97484AD1AC00054119AFE07924EC12A46B2E1DE9BAADAD43D3C309B85A2CA2EB78B7182"
		"46E0F8AB4B9573D783D0C911D3DD8220AA2CBCDE3DE55531E417CF1F52F30A83EB4C0F0C7FA1E99B703B15CCDB0711A57B2119972F7D7884DA47422473831087"
		"A337904608DB7CFBAACF3EA0CB26F9A0B3EF6B1E43A24BF7672ACAAA6D2283B0DCEB3C18122654A0F41EE3D5EA014DEA906AFBC58C37EA28D6C4CBCBB8CF58A5"
		"AD2B6ED1712FC68131A85C3020C7F6620F139E485518378289D4A21C0755ADB34859B3E2642CEBC5D338F19EFFD1E0663072F6076315920D830AE2FB78DC95D8"
		"151114C54478C7726852D4E68939C131BA375418C17E8EC9BE828C8C47D99E3802E0BF8C4AC6F4827B7C3F89C15C211453AC4DFD0633695E1DE3E04ACC0CDEC6"
		"701F164249F90866E45DA8FDEDDC641ABF7D0A11CF23D950923675E3272EAF74051CA24985236748FC6122068ED25F6F585EB913A492E0E4C7C22D7200DE3B16"
		"496542BDE5C1079C0F303720554BE3E9ACE57EAD60351AB9EC87280F7408FDB5F8871B829C4F6BB08CC506ED74AFDBB29B8BF2DF75DC697EC9C6E7C9FA75B486"
		"A412DD003D7FFCC31D784DF7620286909FA1C2F5F3273D94351638EDB0F8292015261C0D9530BD88DC98A4B10AA886434E1E6575314269C811A4E7EEACAEE342"
		"7442B943601B418750922CF6B66D91D4850A7E1B188787542F4F29ACA019CF08FB347C65B8EE38935EDCA4F01CF197852BEB5DFD3D45FFFED8BEA6F7A8573D15"
		"CB1B27F9D1C846C5552A4B4043888232D5D92F8AC3142314F5DA5FD8923746BDF0B8D612BC7F372213A6DBF5D8C363AE5F762034F59CB7BFC197176F3A6170D7"
		"8A6AA9E3B330E249EACF75580E7A06BEAE88E348D1031AFEB703A869A96EB187418A60717AE69D778D221C6D3A5052CA39CBDABA5BEC34EBF9CC0B8E11E7807B"
		"3DF47BF335F20E7A016F8861E60D284F8137681BF23645AE84B56C5D271AE02BAB075D14A0B637EB867288DFF115914CB8DDD1DC3803BFE96FDAC8F7D11ED045"
		"0A5FE11AC9D84AA1C81D2694F3870AB6FAA53E90C1562D33F73AC9EDE955287C558C2968C280C5596EDF35175B5E1BD5C4934DAB73D9AA8EBDE3505C82E152ED"
		"4C7F290E44CF84D0D8DE385BDB837BCC0F9DFAA5558BD6C278576577083274A4AB09420A9386A0301BA40F400CCA7FA74B7361E50C66F911D1349C85D8951B62"
		"15C20E9BD8929AD793AD206536F01C7EFEB9411DBED70465F2D1F136431ED8B335C46253119035E018A08A7DD0AF0DA39735832F892DD808032CBCC5CD6A7DCA"
		"E008C795A444A4F545F322ACD1D1F587DE2A82A67174E5C07197EBDB881A95FDBA00532A09248D750AE7A4B2B0B8647688F3C74951EE56895596F367F53719D3"
		"2F538455F219805CB077BA280E38BAD2F1DB7422E43F36EAD25611BBA77357930A7590374AF5601D13FD047FC492F1816A5AC2F774892ECF8148F6948B69021D"
		"4069B3E7482FB295D04979849A815FE1C276EB023AD7610D90E88C28A145DF2A6511D3ED5802D1E993575CF4D974916F1F99539ADA0F08726385EE9AD8C2833D"
		"709036FE208AB4AA1B9D106672D4C7BE6C7F5826D847EA644B9E32C8A584CAA618C03F85D31B8EB99B77C0A4B759B7027474B82A007DB3EE92CE8CE01879BE7D"
		"93F109E9E0516889A0119FAB0AE8E1510DCA7BD6BB9C8F4587C515B528CFCF48430319F70344801AB4B5F9A0FBF8EA9C841B5BFD676983F042883C77C2EFFFF8"
		"348FCEC3F9742BA973D3AC11816D6C13C2CEB1CD1F82D78979A6E08665A135CBAC180144E21D527EFC36482E39F8F396546F5F56F2BC9B76F20DBFF279E97973"
		"C4888DA71D634065DD3A4C7D66F9336848F9FD21D592E24B0191AEEFB8E63469A1F1E17DF0C291959B1C17BE77A73F3AE4E2D8124F9DF32357FED1881DFB1D7F"
		"87DAC38BACE5B5B657F49C013A648C388A8A553FD50D06367EF842CD942EA0BE400E8909E1D17E70C410AB1F403D56ABA06CB1BCFF1B9514F723093A669F2FB7"
		"60FE1D794411336FB1A61F928ED7A4134C24AF6545A106430535E4186641130E47EF9E10882D4C46F7D30C1AE16FCE7A9302F53CBB7C86241F7F2E08E04AB63F"
		"22621D34E4580CEC0970F4CEBA4D66C99DF3F8D9B21BB8E20A06595CCDB4B717120FA161F641A31099B2C9FA28B67069D0AF9BF6098490D8A4AB159555D87FE7"
		"A8A6863FAC19D4A6D50A96A727394436C8EFEBC5129F194B59A1344C954B8C95512369FBFE3E3A36AF78859C72E2288A22156DBEBC270439672EDD44954B5276"
		"292383D638A2FC7F49951E24A7E9E90F62C9C8AC96F18A2E37E094FB9A6136FBA905C2D03EF1408A0760D1CF05E8B9C90F4563DE82AF5C614F92EAA282B64FAA"
		"27D67F1F637CBCBE5A5C111F838FC6DDA140F1428A4B8B6011B0C2F63D144DB53FBBBCDB6E902CC35C33130A981AC303B9805DE37ABEAD8F9D304CD473BD52B6"
		"E492AE5735530BF2616722192110C9351DB81703058296407A199B146B05EAC88126C99218A6F2202EDA2B9DCACFB6D5EB16C0E0CD75B46C4577678837052EBA",
		kct, sizeof(kct));
	qsctest_hex_to_bin("A9D5AEDD8A7D9BB81B1E42139258BB4AF79B16FA6C97232BF5A469BFA45758F4425CE1C977A877D42784CA4325745A0DE12A7F059847B8E08F944B0C1B8B39B4"
		"A302190A0D5D8C48054180E237BE40051871E6477F383A6852A8625C36A316601E5557A8D0A90A37A4C4F207063082F704C174512949E0C4B32C94D51C429582"
		"BEC7129E703A60F4CCB48745609A187E6E894A3637972D78353CBB327AC3CABE885CB95A6B4BA78E4C1B90FF59CFC33B7B8BB99816687ED050A7701856A07552"
		"20959DB6C47F566351193222C9A86721066D6BF150D4EB526463209338905A355C5F8B1BDF99C97255C8E3F8CE0422164CDB14F237399B381443770267E2667D"
		"14818E8C5BAE9874373A9F79811803FB3724B5AA0DE74A4D132B84447F28B1BE7BFC7323A88492DCBC8046201F847558292252694E3D9BCDD21597A6B22E6453"
		"BF39C7AB92617E802068C2032F83B7577781057FC20D72B28840200A963C9D4540385C73104FF99F57F189D55C89D530C07B90942B9B396EB8BCAC4B6912A599"
		"71367AD6A9713FE80C3AE64054D8A06D44B0DE3A1E7D58B3CDE095FA6964E849238223B3F27A0522C8B8DFD5527AAA2CCEAA3EA0A3C70B55C420F0BC319327D9"
		"35328B81708F0A98503C713ED61DBE6A14C490AD26870CEE32BD46A42076338278F57DD1691A68186FCDB522B92654C62C152D9737B71B1C9AA0139113301F05"
		"7648743B7A154B655AA2D193CB92A4CE78FAB78E3592A953309AE049A643375A256DCEFA80D675B5D87A8DD3444523EC462CB39D00B34FFDD2106DA632AE350F"
		"3A1A0AB3AAB4B5384827C540F5A1B93DA5BDAF706A302B7E2FF0C6C45C92F20653B69960E7C35F77318260E21E87A01EE5A8A8B54028E72B24C1C4C9EA355DD0"
		"34A8D5AA41FFC2710B7BB17516AD1252B249795F75794EC9EC551B6215DAC35D3FFA3C7F8364ABFB6A13F013FB293B3530CF35BA4550E16227B8A2FD95162EF1"
		"63B9158C5BEA94AFB83E1C1A9534F47C63A0CA3A709B8A266A9BB20EB2067A28DB940EF813F491B5D7161BE819342F4949DAB28A28E1C37D3821515227DAC8CA"
		"45F7C6BC2A20E1EAA801F319819C0A01C0C3E39B1E59608B5650B2C115CB01360A02E053CF7A43EAE62A3DC2A98F2C82291270C791CAE5DB612A9B5E6286A6CF"
		"7A391AC8318B56A6634AAA3D4AB80A46B151EAA7A87C59C9BA18C6D05BC96A36828365706CB3BE144F91D338ACFCCC0FD0AC2E7790A50401B3959B01D0339B91"
		"8BCC94BACE593288E66AD008BC4C9516131BB583143962C7CBB130A3A5609A7C2CBAD2464877C441189CC85F271F6E722D220853D7122FC1D9B9E5791CDAF5C5"
		"598098E1D788ADB3C181CA5BAD236ECDD51E78C6703A18030BA266BDF73A4036859265A275221EFAE59E3E75C5FBB6BEF286A520C2BB3606178DF31654CC5A67"
		"B14DE9C3CCEA92C169C8288B9050FCDB530544749D65AFB4590992BB4EF8822D9DF859C0E67C7BF580C84A9F7331488867621AEC958B858FCC238A47C36D51E0"
		"54E6D2CACAE7072226BB43D181B9793D8246630ADB1C7E1363C9F95A2B5ACD0DA4BEE2FC089E323F3A360568C43D16F017CF253DDC6C4E29ABB925619471D381"
		"CB755354719D4719B4E579CB6961987A973AD4C491B083C5FAFB9277B47B2A64A2C1B959BE763D1B2A513F7A4E49F3ADE5B91D46109CF76C9E017295CB5237EC"
		"2279FFB547C04240791912E5E2CC6A594CE006633DD23581B9949D94B566BA8CA4D397796C76C18620F569864B616898752F613C9760F87B69CAB93C6384E898"
		"7051D238B7A21753B0CAD4940C8D06A20E55C6D7460CFE5196DA6143B5863549C2C7C367AE7D922FAA27AB6739644D14AB277274E8372B8F88AC65D0A2407AA9"
		"E0E119248544C442A30222BD70E7B87523573055732555C90A0326FD43344E2C05BE6052C785BE3536C13A41330F0CBCC838A7D426A51D4C44B2CBC8DD502305"
		"69239CFC3BD095BFAA722B8A132186D41EECBC27467BA802B1107BC67C408B46F6AA5AF4DB55A23C69C75966BF75243E37C4C34734C26859DE614109123E9FE1"
		"AA289858C6800F0A87C1A1E605321B845A2B01BEEB0D12882D7C5C08B57A9F8FB66A2C890E557B166EF87D97D75F87C54445D3970A09049631ACD50B5F4DC9A1"
		"AEBB3DEA539EFF5ABE46B14CD7512363092AF068C419F3787946743C28C510E0B155581EF3342707A44EC181079A1A28C3DA04F8B17D7D6488C382A34101B312"
		"843353064A4B6B8BA4F5A3347A567FE33B7DEB2F7D866F294B498F4436E01825DB22CDDDAB7D05F687B1C53B757C8FFE98B0BEF135D090B9ABC8390687B2C568"
		"C22B837FC5F2917E247CB41A0450B3B6BDFBA82DF60E3F660BD0D722C559B52078B88E11A37406BFBF13C1DD92341052AD297C9F5D22710EE94F3D442E0FF19C"
		"56D3A723D63EA683113909416BD2BEC957589C7A1061FA34A80153C1ACAD34021AF8A8021008586DC78DD818BCD03805BACB3FB8A5A7B53A6E10D231D8134C48"
		"6981A2CA657E03899CA85885D2BC8A9488F4D9097CA5703427B124D90C26331AA329283790ACDF39A915381DAA9B8691A6AA10171A453679BB14783178C35711"
		"4189C2685734A042F09B22628A8E16C715860E71520E75C48175D26222C8CEFAFC4D34F8985334436258C9B0D1522108A59C788254C1892D4575A3A986F0B289"
		"65EAFD465FC64A0C5F8F3F9003489415899D59A543D8208C54A3166529B53922", kpk, sizeof(kpk));
	qsctest_hex_to_bin("07638FB69868F3D320E5862BD96933FEB311B362093C9B5D50170BCED43F1B536D9A204BB1F22695950BA1F2A9E8EB828B284488760B3FC84FABA04275D5628E"
		"39C5B2471374283C503299C0AB49B66B8BBB56A4186624F919A2BA59BB08D8551880C2BEFC4F87F25F59AB587A79C327D792D54C974A69262FF8A78938289E9A"
		"87B688B083E0595FE218B6BB1505941CE2E81A5A64C5AAC60417256985349EE47A52420A5F97477B7236AC76BC70E8288729287EE3E34A3DBC3683C0B7B10029"
		"FC203418537E7466BA6385A8FF301EE12708F82AAA1E380FC7A88F8F205AB7E88D7E95952A55BA20D09B79A47141D62BF6EB7DD307B08ECA13A5BC5F6B68581C"
		"6865B27BBCDDAB142F4B2CBFF488C8A22705FAA98A2B9EEA3530C76662335CC7EA3A00777725EBCCCD2A4636B2D9122FF3AB77123CE0883C1911115E50C9E8A9"
		"4194E48DD0D09CFFB3ADCD2C1E92430903D07ADBF00532031575AA7F9E7B5A1F3362DEC936D4043C05F2476C07578BC9CBAF2AB4E382727AD41686A96B254882"
		"0BB03B32F11B2811AD62F489E951632ABA0D1DF89680CC8A8B53B481D92A68D70B4EA1C3A6A561C0692882B5CA8CC942A8D495AFCB06DE89498FB935B775908F"
		"E7A03E324D54CC19D4E1AABD3593B38B19EE1388FE492B43127E5A504253786A0D69AD32601C28E2C88504A5BA599706023A61363E17C6B9BB59BDC697452CD0"
		"59451983D738CA3FD034E3F5988854CA05031DB09611498988197C6B30D258DFE26265541C89A4B31D6864E9389B03CB74F7EC4323FB9421A4B9790A26D17B03"
		"98A26767350909F84D57B6694DF830664CA8B3C3C03ED2AE67B89006868A68527CCD666459AB7F056671000C6164D3A7F266A14D97CBD7004D6C92CACA770B84"
		"4A4FA9B182E7B18CA885082AC5646FCB4A14E1685FEB0C9CE3372AB95365C04FD83084F80A23FF10A05BF15F7FA5ACC6C0CB462C33CA524FA6B8BB359043BA68"
		"609EAA2536E81D08463B19653B5435BA946C9ADDEB202B04B031CC960DCC12E4518D428B32B257A4FC7313D3A7980D80082E934F9D95C32B0A0191A23604384D"
		"D9E079BBBAA266D14C3F756B9F2133107433A4E83FA7187282A809203A4FAF841851833D121AC383843A5E55BC2381425E16C7DB4CC9AB5C1B0D91A47E2B8DE0"
		"E582C86B6B0D907BB360B97F40AB5D038F6B75C814B27D9B968D419832BC8C2BEE605EF6E5059D33100D90485D378450014221736C07407CAC260408AA649266"
		"19788B8601C2A752D1A6CBF820D7C7A04716203225B3895B9342D147A8185CFC1BB65BA06B4142339903C0AC4651385B45D98A8B19D28CD6BAB088787F7EE1B1"
		"2461766B43CBCCB96434427D93C065550688F6948ED1B5475A425F1B85209D061C08B56C1CC069F6C0A7C6F29358CAB911087732A649D27C9B98F9A48879387D"
		"9B00C25959A71654D6F6A946164513E47A75D005986C2363C09F6B537ECA78B9303A5FA457608A586A653A347DB04DFCC19175B3A301172536062A658A952775"
		"70C8852CA8973F4AE123A334047DD711C8927A634A03388A527B034BF7A8170FA702C1F7C23EC32D18A2374890BE9C787A9409C82D192C4BB705A2F996CE405D"
		"85A4C1A1AB9B6AEB49CCE1C2F8A97C3516C72A00A46263BAA696BF25727719C3216423618FF33380934A6C10545C4C5C5155B12486181FC7A2319873978B6A2A"
		"67490F8256BD2196FE1792A4C00077B812EAE8BED3572499684AB3371876761E450C9F9D2768A36806D7AB2046C91F17599E9AC592990808DCD7B4D0919072F1"
		"4EC361773B7252444C323C308326F4A30F8680D2F748F56A132B82674ED0184620B82AD2CB182C97B481626647491290A011CC73828685A8C367A5B9CF8D621B"
		"0D5C1EFF03172758BD004978C251CD51342228989CAE6332AC486437CB5C57D4307462865253BE217B3515C73DF405B7F28217AD0B8CF60C2FFFAA0A0048B1FB"
		"4ACDCDC38B5250CFEC356A6DE26CFA7A588FDC86F98C854AC64C7BFAA96F5A32CC0610934BAA6A586B9A2054F13BA274174AA0D2B3A81B96A940666F789B5A6B"
		"CDC0A6A0178A0C9A02578A493F6EEA0D2E6C13951C9F249A5E8DD71DD49A742D451F1ABBA19AF8C547855E0AFC728E90ABB499C9BEEB766F4729CDA22263E324"
		"F4D7748A788C83376D4B2368923774FEBC21CA00C8F0796553241D18143A1CB46A0ECA03F5F09DE6332BD00117AA89ABBED2C317767CF7F8CDBA4A5A93C8880C"
		"9CB756474731BB241532A043E8C5F53325E62B13AC803A5CB352DB4B6ABFE0AF75DC066FCC9C53A0BC50908487A69156B09D3DE35B33827F6BB513DDD0B2A55B"
		"20E95CA6043C108A380515961E33176C02136E102B9BBF86092FF524EFF16BB541A83474C466300AFA0170B524855A615BB9E904F8902C550CB5A3C40B8EE60E"
		"FD026AF4D6881CF5399569643CA059BA06290C80014426A3669510D39395D6AC4298C434B769189E952B24378B3AB02C7F169A19192A907C904228A9F901295B"
		"558B46DB15C3112CE490BA076AAF58624328C8256AD508F11211C97257EDFABB688600DAB2A374DCA571120BF6F2153BD4A2002C333C875E0C3C0C02A806F633"
		"4694C35412FC5538EACC6F71C3F730327D17BFDB70CAF5D7AAE1E50F113746081B69F7A6C97B9383434A0CD3CC4DAD8B6B83ABAC814B36066505A405A4B4C967"
		"A9D5AEDD8A7D9BB81B1E42139258BB4AF79B16FA6C97232BF5A469BFA45758F4425CE1C977A877D42784CA4325745A0DE12A7F059847B8E08F944B0C1B8B39B4"
		"A302190A0D5D8C48054180E237BE40051871E6477F383A6852A8625C36A316601E5557A8D0A90A37A4C4F207063082F704C174512949E0C4B32C94D51C429582"
		"BEC7129E703A60F4CCB48745609A187E6E894A3637972D78353CBB327AC3CABE885CB95A6B4BA78E4C1B90FF59CFC33B7B8BB99816687ED050A7701856A07552"
		"20959DB6C47F566351193222C9A86721066D6BF150D4EB526463209338905A355C5F8B1BDF99C97255C8E3F8CE0422164CDB14F237399B381443770267E2667D"
		"14818E8C5BAE9874373A9F79811803FB3724B5AA0DE74A4D132B84447F28B1BE7BFC7323A88492DCBC8046201F847558292252694E3D9BCDD21597A6B22E6453"
		"BF39C7AB92617E802068C2032F83B7577781057FC20D72B28840200A963C9D4540385C73104FF99F57F189D55C89D530C07B90942B9B396EB8BCAC4B6912A599"
		"71367AD6A9713FE80C3AE64054D8A06D44B0DE3A1E7D58B3CDE095FA6964E849238223B3F27A0522C8B8DFD5527AAA2CCEAA3EA0A3C70B55C420F0BC319327D9"
		"35328B81708F0A98503C713ED61DBE6A14C490AD26870CEE32BD46A42076338278F57DD1691A68186FCDB522B92654C62C152D9737B71B1C9AA0139113301F05"
		"7648743B7A154B655AA2D193CB92A4CE78FAB78E3592A953309AE049A643375A256DCEFA80D675B5D87A8DD3444523EC462CB39D00B34FFDD2106DA632AE350F"
		"3A1A0AB3AAB4B5384827C540F5A1B93DA5BDAF706A302B7E2FF0C6C45C92F20653B69960E7C35F77318260E21E87A01EE5A8A8B54028E72B24C1C4C9EA355DD0"
		"34A8D5AA41FFC2710B7BB17516AD1252B249795F75794EC9EC551B6215DAC35D3FFA3C7F8364ABFB6A13F013FB293B3530CF35BA4550E16227B8A2FD95162EF1"
		"63B9158C5BEA94AFB83E1C1A9534F47C63A0CA3A709B8A266A9BB20EB2067A28DB940EF813F491B5D7161BE819342F4949DAB28A28E1C37D3821515227DAC8CA"
		"45F7C6BC2A20E1EAA801F319819C0A01C0C3E39B1E59608B5650B2C115CB01360A02E053CF7A43EAE62A3DC2A98F2C82291270C791CAE5DB612A9B5E6286A6CF"
		"7A391AC8318B56A6634AAA3D4AB80A46B151EAA7A87C59C9BA18C6D05BC96A36828365706CB3BE144F91D338ACFCCC0FD0AC2E7790A50401B3959B01D0339B91"
		"8BCC94BACE593288E66AD008BC4C9516131BB583143962C7CBB130A3A5609A7C2CBAD2464877C441189CC85F271F6E722D220853D7122FC1D9B9E5791CDAF5C5"
		"598098E1D788ADB3C181CA5BAD236ECDD51E78C6703A18030BA266BDF73A4036859265A275221EFAE59E3E75C5FBB6BEF286A520C2BB3606178DF31654CC5A67"
		"B14DE9C3CCEA92C169C8288B9050FCDB530544749D65AFB4590992BB4EF8822D9DF859C0E67C7BF580C84A9F7331488867621AEC958B858FCC238A47C36D51E0"
		"54E6D2CACAE7072226BB43D181B9793D8246630ADB1C7E1363C9F95A2B5ACD0DA4BEE2FC089E323F3A360568C43D16F017CF253DDC6C4E29ABB925619471D381"
		"CB755354719D4719B4E579CB6961987A973AD4C491B083C5FAFB9277B47B2A64A2C1B959BE763D1B2A513F7A4E49F3ADE5B91D46109CF76C9E017295CB5237EC"
		"2279FFB547C04240791912E5E2CC6A594CE006633DD23581B9949D94B566BA8CA4D397796C76C18620F569864B616898752F613C9760F87B69CAB93C6384E898"
		"7051D238B7A21753B0CAD4940C8D06A20E55C6D7460CFE5196DA6143B5863549C2C7C367AE7D922FAA27AB6739644D14AB277274E8372B8F88AC65D0A2407AA9"
		"E0E119248544C442A30222BD70E7B87523573055732555C90A0326FD43344E2C05BE6052C785BE3536C13A41330F0CBCC838A7D426A51D4C44B2CBC8DD502305"
		"69239CFC3BD095BFAA722B8A132186D41EECBC27467BA802B1107BC67C408B46F6AA5AF4DB55A23C69C75966BF75243E37C4C34734C26859DE614109123E9FE1"
		"AA289858C6800F0A87C1A1E605321B845A2B01BEEB0D12882D7C5C08B57A9F8FB66A2C890E557B166EF87D97D75F87C54445D3970A09049631ACD50B5F4DC9A1"
		"AEBB3DEA539EFF5ABE46B14CD7512363092AF068C419F3787946743C28C510E0B155581EF3342707A44EC181079A1A28C3DA04F8B17D7D6488C382A34101B312"
		"843353064A4B6B8BA4F5A3347A567FE33B7DEB2F7D866F294B498F4436E01825DB22CDDDAB7D05F687B1C53B757C8FFE98B0BEF135D090B9ABC8390687B2C568"
		"C22B837FC5F2917E247CB41A0450B3B6BDFBA82DF60E3F660BD0D722C559B52078B88E11A37406BFBF13C1DD92341052AD297C9F5D22710EE94F3D442E0FF19C"
		"56D3A723D63EA683113909416BD2BEC957589C7A1061FA34A80153C1ACAD34021AF8A8021008586DC78DD818BCD03805BACB3FB8A5A7B53A6E10D231D8134C48"
		"6981A2CA657E03899CA85885D2BC8A9488F4D9097CA5703427B124D90C26331AA329283790ACDF39A915381DAA9B8691A6AA10171A453679BB14783178C35711"
		"4189C2685734A042F09B22628A8E16C715860E71520E75C48175D26222C8CEFAFC4D34F8985334436258C9B0D1522108A59C788254C1892D4575A3A986F0B289"
		"65EAFD465FC64A0C5F8F3F9003489415899D59A543D8208C54A3166529B53922A5266E9F0ECEF8F06A3081E880D668D8A216B775700C0F460889B4B73F3A14F2"
		"8626ED79D451140800E03B59B956F8210E556067407D13DC90FA9E8B872BFB8F", ksk, sizeof(ksk));
	qsctest_hex_to_bin("ACED87539FE53EE6701BB06DA1C3462ED02C655319AA47760D0A521110AC2842", kss, sizeof(kss));
#else
#	error The parameter set is invalid!
#endif

/* NIST PQC Round 3 KATs */
#if !defined(QSC_KYBER_S6Q3329N256K5)
	parse_nist_cipher_kat(path, seed, &seedlen, kpk, &pklen, ksk, &sklen, kct, &ctlen, kss, &sslen, 0);
#endif

	qsctest_nistrng_prng_initialize(seed, NULL, 0);

	/* generate public and secret keys */
	qsc_kyber_generate_keypair(pk, sk, qsctest_nistrng_prng_generate);

	/* compare the public key to the expected output */
	if (qsc_intutils_are_equal8(pk, kpk, QSC_KYBER_PUBLICKEY_SIZE) != true)
	{
		qsc_consoleutils_print_line("Failure! kyber kat: public-key does not match known answer! -KAT0");
		ret = false;
	}

	/* compare the secret key to the expected output */
	if (qsc_intutils_are_equal8(sk, ksk, QSC_KYBER_PRIVATEKEY_SIZE) != true)
	{
		qsc_consoleutils_print_line("Failure! kyber kat: private-key does not match known answer! -KAT1");
		ret = false;
	}

	/* derive a shared-secret key and creates a response (in: pk | out: ct and ss2) */
	qsc_kyber_encapsulate(ss2, ct, pk, qsctest_nistrng_prng_generate);

	/* compare the cipher-text to the expected output */
	if (qsc_intutils_are_equal8(ct, kct, QSC_KYBER_CIPHERTEXT_SIZE) != true)
	{
		qsc_consoleutils_print_line("Failure! kyber kat: cipher-text does not match known answer! -KAT2");
		ret = false;
	}

	/* a uses b's response to get the shared-secret key (in: ct, sk | out: ss1) */
	if (qsc_kyber_decapsulate(ss1, ct, sk) != true)
	{
		qsc_consoleutils_print_line("Failure! kyber kat: decryption authentication failure! -KAT3");
		ret = false;
	}

	/* compare the two keys for equality */
	if (qsc_intutils_are_equal8(ss1, ss2, QSC_KYBER_SHAREDSECRET_SIZE) != true)
	{
		qsc_consoleutils_print_line("Failure! kyber kat: shared secrets do not match! -KAT4");
		ret = false;
	}

	/* compare the key to the expected output */
	if (qsc_intutils_are_equal8(ss1, kss, QSC_KYBER_SHAREDSECRET_SIZE) != true)
	{
		qsc_consoleutils_print_line("Failure! kyber kat: shared secret does not match known answer! -KAT5");
		ret = false;
	}

	return ret;
}

bool qsctest_kyber_privatekey_integrity()
{
	uint8_t ct[QSC_KYBER_CIPHERTEXT_SIZE] = { 0 };
	uint8_t pk[QSC_KYBER_PUBLICKEY_SIZE] = { 0 };
	uint8_t seed[QSCTEST_NIST_RNG_SEED_SIZE] = { 0 };
	uint8_t sk[QSC_KYBER_PRIVATEKEY_SIZE] = { 0 };
	uint8_t ssk1[QSC_KYBER_SHAREDSECRET_SIZE] = { 0 };
	uint8_t ssk2[QSC_KYBER_SHAREDSECRET_SIZE] = { 0 };
	bool res;

	res = true;
	qsctest_hex_to_bin("061550234D158C5EC95595FE04EF7A25767F2E24CC2BC479D09D86DC9ABCFDE7056A8C266F9EF97ED08541DBD2E1FFA1", seed, sizeof(seed));
	qsctest_nistrng_prng_initialize(seed, NULL, 0);

	/* generate public and secret keys */
	qsc_kyber_generate_keypair(pk, sk, qsctest_nistrng_prng_generate);

	/* derive a shared-secret key and creates a response (in: pk | out: ct and ss2) */
	qsc_kyber_encapsulate(ssk2, ct, pk, qsctest_nistrng_prng_generate);

	/* replace secret key with random values */
	if (qsc_csp_generate(sk + QSC_KYBER_PUBLICKEY_SIZE, 32) != true)
	{
		qsc_consoleutils_print_line("Failure! kyber_test_operations: the shared secrets are not equal -INTEG0");
		res = false;
	}

	/* invalid secret key, should fail */
	if (qsc_kyber_decapsulate(ssk1, ct, sk) == true)
	{
		res = false;
	}

	/* fail if equal */
	if (qsc_intutils_are_equal8(ssk1, ssk2, QSC_KYBER_SHAREDSECRET_SIZE) == true)
	{
		res = false;
	}

	return res;
}

bool qsctest_kyber_publickey_integrity()
{
	uint8_t ct[QSC_KYBER_CIPHERTEXT_SIZE] = { 0 };
	uint8_t pk[QSC_KYBER_PUBLICKEY_SIZE] = { 0 };
	uint8_t seed[QSCTEST_NIST_RNG_SEED_SIZE] = { 0 };
	uint8_t sk[QSC_KYBER_PRIVATEKEY_SIZE] = { 0 };
	uint8_t ssk1[QSC_KYBER_SHAREDSECRET_SIZE] = { 0 };
	uint8_t ssk2[QSC_KYBER_SHAREDSECRET_SIZE] = { 0 };
	bool res;

	res = true;
	qsctest_hex_to_bin("061550234D158C5EC95595FE04EF7A25767F2E24CC2BC479D09D86DC9ABCFDE7056A8C266F9EF97ED08541DBD2E1FFA1", seed, sizeof(seed));
	qsctest_nistrng_prng_initialize(seed, NULL, 0);

	/* generate public and secret keys */
	qsc_kyber_generate_keypair(pk, sk, qsctest_nistrng_prng_generate);

	/* replace public key with random values */
	if (qsc_csp_generate(pk, 32) != true)
	{
		qsc_consoleutils_print_line("Failure! kyber_test_operations: the shared secrets are not equal -INTEG0");
		res = false;
	}

	/* derive a shared-secret key and creates a response (in: pk | out: ct and ss2) */
	qsc_kyber_encapsulate(ssk2, ct, pk, qsctest_nistrng_prng_generate);

	/* invalid secret key, should fail */
	if (qsc_kyber_decapsulate(ssk1, ct, sk) == true)
	{
		res = false;
	}

	/* fail if equal */
	if (qsc_intutils_are_equal8(ssk1, ssk2, QSC_KYBER_SHAREDSECRET_SIZE) == true)
	{
		res = false;
	}

	return res;
}

bool qsctest_kyber_operations_test()
{
	uint8_t ct[QSC_KYBER_CIPHERTEXT_SIZE] = { 0 };
	uint8_t esd[QSC_KYBER_SEED_SIZE] = { 0 };
	uint8_t pk[QSC_KYBER_PUBLICKEY_SIZE] = { 0 };
	uint8_t seed[QSCTEST_NIST_RNG_SEED_SIZE] = { 0 };
	uint8_t ssk1[QSC_KYBER_SHAREDSECRET_SIZE] = { 0 };
	uint8_t ssk2[QSC_KYBER_SHAREDSECRET_SIZE] = { 0 };
	uint8_t sk[QSC_KYBER_PRIVATEKEY_SIZE] = { 0 };
	bool res;

	res = true;
	qsctest_hex_to_bin("061550234D158C5EC95595FE04EF7A25767F2E24CC2BC479D09D86DC9ABCFDE7056A8C266F9EF97ED08541DBD2E1FFA1", seed, sizeof(seed));
	qsctest_nistrng_prng_initialize(seed, NULL, 0);

	/* generate public and secret keys */
	qsc_kyber_generate_keypair(pk, sk, qsctest_nistrng_prng_generate);

	/* derive a shared-secret key and creates a response (in: pk | out: ct and ss2) */
	qsc_kyber_encapsulate(ssk2, ct, pk, qsctest_nistrng_prng_generate);

	/* decapsulate the cipher-text and extract the shared secret */
	if (qsc_kyber_decapsulate(ssk1, ct, sk) != true)
	{
		qsc_consoleutils_print_line("Failure! kyber operations: decapsulation failure -KOT1");
		res = false;
	}

	if (qsc_intutils_are_equal8(ssk1, ssk2, QSC_KYBER_SHAREDSECRET_SIZE) != true)
	{
		qsc_consoleutils_print_line("Failure! kyber operations: the shared secrets are not equal -KOT2");
		res = false;
	}

	/* test encrypt/decrypt api */

	qsc_memutils_clear(ct, sizeof(ct));
	qsc_memutils_clear(ssk1, sizeof(ssk1));
	qsc_memutils_clear(ssk2, sizeof(ssk2));

	qsc_csp_generate(esd, sizeof(esd));

	qsc_kyber_encrypt(ssk1, ct, pk, esd);
	qsc_kyber_decrypt(ssk2, ct, sk);

	if (qsc_intutils_are_equal8(ssk1, ssk2, QSC_KYBER_SHAREDSECRET_SIZE) != true)
	{
		qsc_consoleutils_print_line("Failure! kyber operations: the shared secrets are not equal -KOT3");
		res = false;
	}

	return res;
}

void qsctest_kyber_run()
{
	if (qsctest_kyber_kat_test() == true)
	{
		qsc_consoleutils_print_line("Success! Passed the Kyber encryption, and decryption known answer test.");
	}
	else
	{
		qsc_consoleutils_print_line("Failure! Failed the Kyber encryption, and decryption known answer test.");
	}

	if (qsctest_kyber_operations_test() == true)
	{
		qsc_consoleutils_print_line("Success! Passed the Kyber key generation, encryption, and decryption stress test.");
	}
	else
	{
		qsc_consoleutils_print_line("Failure! Failed the Kyber key generation, encryption, and decryption stress test.");
	}

	if (qsctest_kyber_privatekey_integrity() == true)
	{
		qsc_consoleutils_print_line("Success! Passed the Kyber private-key tamper test.");
	}
	else
	{
		qsc_consoleutils_print_line("Failure! Failed the Kyber private-key tamper test.");
	}

	if (qsctest_kyber_publickey_integrity() == true)
	{
		qsc_consoleutils_print_line("Success! Passed the Kyber public-key tamper test.");
	}
	else
	{
		qsc_consoleutils_print_line("Failure! Failed the Kyber public-key tamper test.");
	}

	if (qsctest_kyber_ciphertext_integrity() == true)
	{
		qsc_consoleutils_print_line("Success! Passed the Kyber cipher-text tamper test.");
	}
	else
	{
		qsc_consoleutils_print_line("Failure! Failed the Kyber cipher-text tamper test.");
	}
}

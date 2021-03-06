// KiCAD_parse.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>

#if defined(_DEBUG) && !defined(_LIB)
std::ofstream spirit_os("spirit_os.txt");
#define BOOST_SPIRIT_X3_DEBUG_OUT spirit_os
#define BOOST_SPIRIT_X3_DEBUG
#endif

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/adapted/std_pair.hpp>

#include "KiCADStore.h"
#include "KiCAD_spirit.h"

#define FULL //

// ................................................................
NSAssembly::Assembly ParseKiCAD(const std::string& buf)
{
	using namespace boost::spirit::x3;
	typedef std::string::const_iterator iterator_type;

	NSAssembly::Assembly board;
	iterator_type begin = buf.begin();
	iterator_type end = buf.end();
	auto the_parser = Bridge::kicad;
	bool r = phrase_parse(begin, end, the_parser, space, board);
#ifdef FULL
	std::wcout << "cnt: " << board.components.size() << std::endl;

	for (auto& c : board.components)
	{
		std::cout << " name: " << c.name << "\n  pos: " << c.pos.x << " " << c.pos.y << std::endl
			<< "  layer: " << c.layer << std::endl;
		for (auto& at : c.comp_attrs)
		{
			size_t cnt = at.second.end() - at.second.begin();
			cnt = cnt > 30 ? 30 : cnt;
			std::cout << " attr: " << at.first << "\t :: " << std::string(at.second.begin(), at.second.begin() + cnt) << "...\n";
		}

		for (auto& l : c.items.lineset)
			std::cout << "  " << l << std::endl;
		//for (auto t : c.)
		//	std::cout << " text: " << t.first.front() << std::endl;
		//for (auto t : comp.components)
		//	std::cout << " text: " << t.first.front() << std::endl;
		std::cout << " pads: " << c.items.padset.size() << std::endl << std::endl;
	}
	for (auto& l : board.assem_items.texts)
		std::cout << "text: " << l.text << " " << l.layer << std::endl;
	for (auto& l : board.assem_items.lineset)
		std::cout << "line: " << l.get()->layer << std::endl;
	for (auto& l : board.assem_items.segments)
		std::cout << "seg: " << l.line.a << " " << l.line.b << " c: " << l.line.layer << std::endl;
	for (auto& l : board.assem_items.vias)
		std::cout << "via: " << l.at.x << " " << l.at.y << " size: " << l.size << std::endl;

#endif
	return board;
}

#ifndef _LIB
// ................................................................
#ifdef FULL
int main()
{
	//"C:\cpp\ECAD\project\test_files\KiCAD_project\test.kicad_pcb"
	//std::ifstream fbuf(R"(..\project\test_files\KiCAD_project\test.kicad_pcb)");
	//std::ifstream fbuf(R"(..\project\test_files\ecc83-pp.kicad_pcb)");
	std::ifstream fbuf(R"(..\project\test_files\sonde xilinx.kicad_pcb)");
	std::stringstream  bufstr;
	bufstr << fbuf.rdbuf();
	std::string input(bufstr.str());

	auto board = ParseKiCAD(input);
}

#else //sub testing

//#define TEST_COMP
//#define TEST_PAD
//#define TEST_ATTRIBUTES
//#define TEST_SEGMENT
//#define TEST_GR_LINE
//#define TEST_ASMITEMS
#define TEST_ZONE
//#define TEST_GRTEXT

#ifdef TEST_GRTEXT
int main()
{
	//	std::vector<NSAssembly::Line> attr;
	std::string pad(R"(
  (gr_text "Component Side" (at 121.92 69.85) (layer Dessus)
    (effects (font (size 1.524 1.524) (thickness 0.3048)))
  )
  (gr_text "Copper layer" (at 119.38 68.58) (layer Dessous)
    (effects (font (size 1.524 1.524) (thickness 0.3048)) (justify mirror))
  )
)");
//	std::string pad(R"(	 (width 0.3048) )");
	std::vector<Bridge::GRText> attr;
	auto begin = pad.begin();
	using namespace boost::spirit::x3;
	auto the_parser = Bridge::gr_text;
	bool r = phrase_parse(begin, pad.end(), *the_parser, space, attr);
	std::cout << "test" << std::endl;
}
#endif;

#ifdef TEST_ZONE
int main()
{
	//	std::vector<NSAssembly::Line> attr;
	std::string pad(R"(
  (zone (net 13) (net_name GND) (layer Dessous) (tstamp 4BF0F951) (hatch edge 0.508)
    (connect_pads (clearance 0.508))
    (min_thickness 0.254)
    (fill (arc_segments 16) (thermal_gap 0.508) (thermal_bridge_width 0.508))
    (polygon
      (pts
        (xy 182.88 109.22) (xy 182.88 68.58) (xy 105.41 68.58) (xy 105.41 109.22)
      )
    )
    (filled_polygon
      (pts
        (xy 130.73126 88.265) (xy 129.7813 89.2175) (xy 128.40716 89.2175) (xy 128.17856 88.9889) (xy 127.96266 88.9)
        (xy 128.17856 88.8111) (xy 128.56972 88.41994) (xy 128.78308 87.90686) (xy 128.78308 87.35314) (xy 128.56972 86.84006)
        (xy 128.17856 86.4489) (xy 127.97536 86.36254) (xy 128.397 86.06028) (xy 128.6891 85.58784) (xy 128.73228 85.43798)
        (xy 128.6129 85.217) (xy 127.254 85.217) (xy 127.127 85.217) (xy 126.873 85.217) (xy 126.746 85.217)
        (xy 125.3871 85.217) (xy 125.26772 85.43798) (xy 125.3109 85.58784) (xy 125.603 86.06028) (xy 126.0221 86.36254)
        (xy 125.82144 86.4489) (xy 125.62586 86.64448) (xy 125.32106 86.33968) (xy 124.76226 86.10854) (xy 124.15774 86.10854)
        (xy 123.59894 86.33968) (xy 123.31446 86.62416) (xy 123.31446 86.08314) (xy 123.1011 85.57006) (xy 122.70994 85.1789)
        (xy 122.49404 85.09) (xy 122.70994 85.0011) (xy 122.93854 84.7725) (xy 124.46 84.7725) (xy 124.82322 84.69884)
        (xy 124.82576 84.69884) (xy 125.1331 84.4931) (xy 125.67666 83.947) (xy 125.73 83.96986) (xy 125.80874 83.96986)
        (xy 125.603 84.11972) (xy 125.3109 84.59216) (xy 125.26772 84.74202) (xy 125.3871 84.963) (xy 126.746 84.963)
        (xy 126.873 84.963) (xy 127.127 84.963) (xy 127.254 84.963) (xy 128.6129 84.963) (xy 128.73228 84.74202)
        (xy 128.6891 84.59216) (xy 128.397 84.11972) (xy 128.18872 83.96986) (xy 128.26746 83.96986) (xy 128.50114 83.87334)
        (xy 128.67894 83.69554) (xy 128.77546 83.4644) (xy 128.77546 83.21294) (xy 128.77546 81.63814) (xy 128.75768 81.5975)
        (xy 129.1463 81.5975) (xy 129.8575 82.3087) (xy 129.8575 86.995) (xy 129.93116 87.36076) (xy 130.1369 87.6681)
        (xy 130.73126 88.265)
      )
    )
    (filled_polygon
      (pts
        (xy 182.753 109.093) (xy 157.52826 109.093) (xy 157.52826 70.03796) (xy 157.50032 69.5198) (xy 157.35808 69.17182)
        (xy 157.12694 69.1134) (xy 156.39034 69.85) (xy 157.12694 70.5866) (xy 157.35808 70.52818) (xy 157.52826 70.03796)
        (xy 157.52826 109.093) (xy 156.9466 109.093) (xy 156.9466 70.76694) (xy 156.21 70.03034) (xy 156.02966 70.21068)
        (xy 156.02966 69.85) (xy 155.29306 69.1134) (xy 155.06192 69.17182) (xy 154.89174 69.66204) (xy 154.91968 70.1802)
        (xy 155.06192 70.52818) (xy 155.29306 70.5866) (xy 156.02966 69.85) (xy 156.02966 70.21068) (xy 155.4734 70.76694)
        (xy 155.53182 70.99808) (xy 156.02204 71.16826) (xy 156.5402 71.14032) (xy 156.88818 70.99808) (xy 156.9466 70.76694)
        (xy 156.9466 109.093) (xy 149.90826 109.093) (xy 149.90826 108.13796) (xy 149.88032 107.6198) (xy 149.73808 107.27182)
        (xy 149.50694 107.2134) (xy 149.3266 107.39374) (xy 149.3266 107.03306) (xy 149.26818 106.80192) (xy 148.77796 106.63174)
        (xy 148.2598 106.65968) (xy 147.91182 106.80192) (xy 147.8534 107.03306) (xy 148.59 107.76966) (xy 149.3266 107.03306)
        (xy 149.3266 107.39374) (xy 148.77034 107.95) (xy 149.50694 108.6866) (xy 149.73808 108.62818) (xy 149.90826 108.13796)
        (xy 149.90826 109.093) (xy 149.26818 109.093) (xy 149.3266 108.86694) (xy 148.59 108.13034) (xy 148.40966 108.31068)
        (xy 148.40966 107.95) (xy 147.67306 107.2134) (xy 147.44192 107.27182) (xy 147.27174 107.76204) (xy 147.29968 108.2802)
        (xy 147.44192 108.62818) (xy 147.67306 108.6866) (xy 148.40966 107.95) (xy 148.40966 108.31068) (xy 147.8534 108.86694)
        (xy 147.90928 109.093) (xy 146.09826 109.093) (xy 146.09826 108.13796) (xy 146.07032 107.6198) (xy 145.92808 107.27182)
        (xy 145.69694 107.2134) (xy 144.96034 107.95) (xy 145.69694 108.6866) (xy 145.92808 108.62818) (xy 146.09826 108.13796)
        (xy 146.09826 109.093) (xy 145.45818 109.093) (xy 145.5166 108.86694) (xy 144.78 108.13034) (xy 144.59966 108.31068)
        (xy 144.0434 108.86694) (xy 144.09928 109.093) (xy 142.28826 109.093) (xy 141.64818 109.093) (xy 141.7066 108.86694)
        (xy 140.97 108.13034) (xy 140.78966 108.31068) (xy 140.2334 108.86694) (xy 140.28928 109.093) (xy 128.73228 109.093)
        (xy 128.73228 98.13798) (xy 128.6129 97.917) (xy 127.127 97.917) (xy 127.127 99.2124) (xy 127.4064 99.2124)
        (xy 127.94742 99.0854) (xy 128.397 98.76028) (xy 128.6891 98.28784) (xy 128.73228 98.13798) (xy 128.73228 109.093)
        (xy 126.873 109.093) (xy 126.873 99.2124) (xy 126.873 97.917) (xy 125.3871 97.917) (xy 125.26772 98.13798)
        (xy 125.3109 98.28784) (xy 125.603 98.76028) (xy 126.05258 99.0854) (xy 126.5936 99.2124) (xy 126.873 99.2124)
        (xy 126.873 109.093) (xy 123.31446 109.093) (xy 123.31446 104.41686) (xy 123.31446 103.86314) (xy 123.1011 103.35006)
      )
    )
  )

)");
	Bridge::Zone attr;
	auto begin = pad.begin();
	using namespace boost::spirit::x3;
	auto the_parser = Bridge::zone;
	bool r = phrase_parse(begin, pad.end(), the_parser, space, attr);
	std::cout << "test" << std::endl;
}
#endif;

#ifdef TEST_GR_LINE
int main()
{
	//	std::vector<NSAssembly::Line> attr;
	std::string pad(R"(
 (gr_line (start 102.87 110.49) (end 102.87 106.68) (angle 90) (layer Contours.Ci) (width 0.3048))
  (gr_line (start 179.07 110.49) (end 102.87 110.49) (angle 90) (layer Contours.Ci) (width 0.3048))
  (gr_line (start 102.87 67.31) (end 179.07 67.31) (angle 90) (layer Contours.Ci) (width 0.3048))
  (gr_line (start 102.87 71.12) (end 102.87 67.31) (angle 90) (layer Contours.Ci) (width 0.3048))
  (gr_text TMS-PROG (at 158.115 102.235) (layer Dessus)
    (effects (font (size 1.524 1.524) (thickness 0.381)))
  )

  (segment (start 106.045 78.105) (end 105.537 77.978) (width 0.635) (layer Dessus) (net 1) (status 400))
 )");
	std::vector<Bridge::KiAssemLine> attr;
	auto begin = pad.begin();
	using namespace boost::spirit::x3;
	auto the_parser = Bridge::gr_line;
	bool r = phrase_parse(begin, pad.end(), *the_parser, space, attr);
	std::cout << "test" << std::endl;
}
#endif;

#ifdef TEST_ASMITEMS
int main()
{
	//	std::vector<NSAssembly::Line> attr;
	std::string pad("(tag \"this_is_the_attribute and more\")");
	Bridge::KiBoardItems attr;
	auto begin = pad.begin();
	using namespace boost::spirit::x3;
	auto the_parser = Bridge::assem_items;
	bool r = phrase_parse(begin, pad.end(), the_parser, space, attr);
	std::cout << "test" << std::endl;
}
#endif;

#ifdef TEST_ATTRIBUTES
int main()
{
	//	std::vector<NSAssembly::Line> attr;
	std::string pad("(tag \"this_is_the_attribute and more\")");
	NSAssembly::attr_pair attr;
	auto begin = pad.begin();
	using namespace boost::spirit::x3;
	auto the_parser = Bridge::comp_attrs;
	bool r = phrase_parse(begin, pad.end(), the_parser, space, attr);
	std::cout << "test" << std::endl;
}
#endif;

#ifdef TEST_PAD
	std::string pad2(R"(
    (pad 1 thru_hole circle (at 0 0) (size 2.4 2.4) (drill 1.2) (layers *.Cu *.Mask)
      (net 1 "Net-(R1-Pad1)\")
	)
    (pad "" thru_hole rect (at -1.524 0) (size 2.794 0.254)
      (layers *.Cu *.Mask Dessus.SilkS)
    )
    (pad "" thru_hole rect (at 0 -1.524) (size 0.254 2.794) (drill 1.2)
      (layers *.Cu *.Mask Dessus.SilkS)
    )
    (pad "" thru_hole circle (at 0 0) (size 0.254 0.254) (drill 0.0508)
      (layers *.Cu *.Mask)
    )
	(pad 2 thru_hole oval(at 30.48 0) (size 2.4 2.4) (drill 1.2) (layers *.Cu *.Mask)
		(net 2 "Net-(R1-Pad2)\"))
	)
)");

int main()
{
	//	std::vector<NSAssembly::Line> attr;
	std::string pad(pad2);
	std::vector<NSAssembly::KiPad> attrs;
	auto begin = pad.begin();
	using namespace boost::spirit::x3;
	auto the_parser = Bridge::pad;
	bool r = phrase_parse(begin, pad.end(), *the_parser, space, attrs);
	for(auto& item : attrs)
		std::cout << item.number << "  " << item.layers.front() << " net: " << item.net.second << std::endl;
	std::cout << "\ntest" << std::endl;
}
#endif

#ifdef TEST_SEGMENT
	std::string test(R"(
   (segment (start 106.045 78.105) (end 105.537 77.978) (width 0.635) (layer Dessus) (net 1) (status 400))
(segment (start 113.03 96.52) (end 113.03 88.9) (width 0.25) (layer F.Cu) (net 1) (status 400000))
  (segment (start 115.57 86.36) (end 121.92 86.36) (width 0.25) (layer F.Cu) (net 1) (tstamp 5C151114) (status 800000))
  (segment (start 113.03 88.9) (end 115.57 86.36) (width 0.25) (layer F.Cu) (net 1) (tstamp 5C151111))
)
)");

int main()
{
	//	std::vector<NSAssembly::Line> attr;
	NSAssembly::SegmentSet attr;
	auto begin = test.begin();
	using namespace boost::spirit::x3;
	auto the_parser = Bridge::seg_line;
	bool r = phrase_parse(begin, test.end(), *the_parser, space, attr);
	return 0;
}
#endif

#ifdef TEST_COMP
	std::string test(R"(
   (module R4 (layer Dessus) (tedit 200000) (tstamp 3)
    (at 116.84 76.2 180)
    (descr "Resitance 4 pas")
    (tags R)
    (path /3EBF7D16)
    (autoplace_cost180 10)
    (fp_text reference R1 (at 0 0 180) (layer Dessus.SilkS)
      (effects (font (size 1.397 1.27) (thickness 0.2032)))
    )
    (fp_text value 100 (at 0 0 180) (layer Dessus.SilkS) hide
      (effects (font (size 1.397 1.27) (thickness 0.2032)))
    )
    (fp_line (start -5.08 0) (end -4.064 0) (layer Dessus.SilkS) (width 0.3048))
    (fp_line (start -4.064 0) (end -4.064 -1.016) (layer Dessus.SilkS) (width 0.3048))
    (fp_line (start -4.064 -1.016) (end 4.064 -1.016) (layer Dessus.SilkS) (width 0.3048))
    (fp_line (start 4.064 -1.016) (end 4.064 1.016) (layer Dessus.SilkS) (width 0.3048))
    (fp_line (start 4.064 1.016) (end -4.064 1.016) (layer Dessus.SilkS) (width 0.3048))
    (fp_line (start -4.064 1.016) (end -4.064 0) (layer Dessus.SilkS) (width 0.3048))
)
 (module R4 (layer Dessus) (tedit 200000) (tstamp 3)
    (at 116.84 83.82 180)
    (descr "Resitance 4 pas")
    (tags R)
    (path /3EBF7D26)
    (autoplace_cost180 10)
    (fp_text reference R6 (at 0 0 180) (layer Dessus.SilkS)
      (effects (font (size 1.397 1.27) (thickness 0.2032)))
    )
    (fp_text value 100 (at 0 0 180) (layer Dessus.SilkS) hide
      (effects (font (size 1.397 1.27) (thickness 0.2032)))
    )
    (fp_line (start -5.08 0) (end -4.064 0) (layer Dessus.SilkS) (width 0.3048))
    (fp_line (start -4.064 0) (end -4.064 -1.016) (layer Dessus.SilkS) (width 0.3048))
    (fp_line (start -4.064 -1.016) (end 4.064 -1.016) (layer Dessus.SilkS) (width 0.3048))
    (fp_line (start 4.064 -1.016) (end 4.064 1.016) (layer Dessus.SilkS) (width 0.3048))
    (fp_line (start 4.064 1.016) (end -4.064 1.016) (layer Dessus.SilkS) (width 0.3048))
    (fp_line (start -4.064 1.016) (end -4.064 0) (layer Dessus.SilkS) (width 0.3048))
    (fp_line (start -4.064 -0.508) (end -3.556 -1.016) (layer Dessus.SilkS) (width 0.3048))
    (fp_line (start 5.08 0) (end 4.064 0) (layer Dessus.SilkS) (width 0.3048))
)
)");

int main()
{
	//	std::vector<NSAssembly::Line> attr;
	NSAssembly::PcComponentSet attribute;
	auto begin = test.begin();
	using namespace boost::spirit::x3;
	auto the_parser = Bridge::comp;
	bool r = phrase_parse(begin, test.end(), *the_parser, space, attribute);
	return 0;
}
#endif //TEST_COMP
#endif //FULL
#endif //_LIB

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

#define FULL
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
	for (auto& l : board.assem_items.segments)
		std::cout << "seg: " << l.line.a << " " << l.line.b << " c: " << l.line.layer << std::endl;
	for (auto& l : board.assem_items.lineset)
		std::cout << "line: " << l.get()->layer << std::endl;
#endif
	return board;
}

#ifndef _LIB
// ................................................................
#ifdef FULL
int main()
{
	//std::ifstream fbuf(R"(..\KiCAD_project\test.kicad_pcb)");
	std::ifstream fbuf(R"(..\project\test_files\ecc83-pp.kicad_pcb)");
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

#ifdef TEST_ZONE
int main()
{
	//	std::vector<NSAssembly::Line> attr;
	std::string pad(R"( (zone (net 1) (net_name GND) (layer Cuivre) (tstamp 4EED97A2) (hatch edge 0.508)
    (connect_pads (clearance 0.635))
    (min_thickness 0.381)
    (fill yes (arc_segments 32) (thermal_gap 0.254) (thermal_bridge_width 0.50038))
    (polygon
      (pts
        (xy 167.64 132.08) (xy 167.64 91.44) (xy 124.46 91.44) (xy 124.46 132.08)
      )
    )
    (filled_polygon
      (pts
        (xy 167.386 127.8747) (xy 167.3002 127.745561) (xy 166.909756 127.352381) (xy 166.450378 127.042527) (xy 165.939566 126.827801)
        (xy 165.613438 126.760856) (xy 165.613438 125.080387) (xy 165.58852 124.856618) (xy 165.520426 124.64201) (xy 165.428009 124.469114)
        (xy 165.239548 124.404867) (xy 165.155133 124.489281) (xy 165.155133 124.320452) (xy 165.090886 124.131991) (xy 164.890987 124.02839)
        (xy 164.674717 123.965779) (xy 164.450387 123.946562) (xy 164.226618 123.97148) (xy 164.01201 124.039574) (xy 163.839114 124.131991)
      )
 )))");
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
	std::string pad(R"(  (gr_line (start 123.825 132.715) (end 168.275 132.715) (angle 90) (layer Edge.Cuts) (width 0.127))
		(gr_line(start 168.275 90.805) (end 123.825 90.805) (angle 90) (layer Edge.Cuts) (width 0.127))
	)");
	Bridge::KiAssemLine attr;
	auto begin = pad.begin();
	using namespace boost::spirit::x3;
	auto the_parser = Bridge::gr_line;
	bool r = phrase_parse(begin, pad.end(), the_parser, space, attr);
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

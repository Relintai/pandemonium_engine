#ifndef HTML_BUILDER_BIND_H
#define HTML_BUILDER_BIND_H

#include "core/object.h"
#include "core/ustring.h"

#include "core/reference.h"

class _HTMLBuilder;

class _HTMLTag : public Reference {
	GDCLASS(_HTMLTag, Reference)

public:
	bool get_simple() const;
	void set_simple(const bool val);

	String get_result();
	void set_result(const String &str);

	Ref<_HTMLTag> str(const String &str);
	Ref<_HTMLTag> style(const String &val);
	Ref<_HTMLTag> href(const String &val);
	Ref<_HTMLTag> cls(const String &val);
	Ref<_HTMLTag> clsse(const String &val); //se -> skip empty
	Ref<_HTMLTag> id(const String &val);
	Ref<_HTMLTag> name(const String &val);
	Ref<_HTMLTag> content(const String &val);
	Ref<_HTMLTag> value(const String &val);
	Ref<_HTMLTag> accept(const String &val);
	Ref<_HTMLTag> src(const String &val);
	Ref<_HTMLTag> alt(const String &val);
	Ref<_HTMLTag> inputmode(const String &val);
	Ref<_HTMLTag> list(const String &val);

	Ref<_HTMLTag> autocomplete(const String &val);

	Ref<_HTMLTag> autocomplete_off();
	Ref<_HTMLTag> autocomplete_on();
	Ref<_HTMLTag> autocomplete_name();
	Ref<_HTMLTag> autocomplete_name_honorific_prefix();
	Ref<_HTMLTag> autocomplete_name_given_name();
	Ref<_HTMLTag> autocomplete_name_additional_name();
	Ref<_HTMLTag> autocomplete_name_family_name();
	Ref<_HTMLTag> autocomplete_name_honorific_suffix();
	Ref<_HTMLTag> autocomplete_name_nickname();
	Ref<_HTMLTag> autocomplete_email();
	Ref<_HTMLTag> autocomplete_username();
	Ref<_HTMLTag> autocomplete_new_password();
	Ref<_HTMLTag> autocomplete_current_password();
	Ref<_HTMLTag> autocomplete_one_time_code();
	Ref<_HTMLTag> autocomplete_organization_title();
	Ref<_HTMLTag> autocomplete_organization();
	Ref<_HTMLTag> autocomplete_street_address();
	Ref<_HTMLTag> autocomplete_address_line1();
	Ref<_HTMLTag> autocomplete_address_line2();
	Ref<_HTMLTag> autocomplete_address_line3();
	Ref<_HTMLTag> autocomplete_address_level_1();
	Ref<_HTMLTag> autocomplete_address_level_2();
	Ref<_HTMLTag> autocomplete_address_level_3();
	Ref<_HTMLTag> autocomplete_address_level_4();
	Ref<_HTMLTag> autocomplete_country();
	Ref<_HTMLTag> autocomplete_country_name();
	Ref<_HTMLTag> autocomplete_postal_code();
	Ref<_HTMLTag> autocomplete_cc_name();
	Ref<_HTMLTag> autocomplete_cc_given_name();
	Ref<_HTMLTag> autocomplete_cc_additional_name();
	Ref<_HTMLTag> autocomplete_cc_family_name();
	Ref<_HTMLTag> autocomplete_cc_number();
	Ref<_HTMLTag> autocomplete_cc_exp();
	Ref<_HTMLTag> autocomplete_cc_exp_month();
	Ref<_HTMLTag> autocomplete_cc_exp_year();
	Ref<_HTMLTag> autocomplete_cc_csc();
	Ref<_HTMLTag> autocomplete_cc_type();
	Ref<_HTMLTag> autocomplete_transaction_currency();
	Ref<_HTMLTag> autocomplete_transaction_amount();
	Ref<_HTMLTag> autocomplete_language();
	Ref<_HTMLTag> autocomplete_bday();
	Ref<_HTMLTag> autocomplete_bday_day();
	Ref<_HTMLTag> autocomplete_bday_month();
	Ref<_HTMLTag> autocomplete_bday_year();
	Ref<_HTMLTag> autocomplete_sex();
	Ref<_HTMLTag> autocomplete_tel();
	Ref<_HTMLTag> autocomplete_tel_country_code();
	Ref<_HTMLTag> autocomplete_tel_national();
	Ref<_HTMLTag> autocomplete_tel_area_code();
	Ref<_HTMLTag> autocomplete_tel_local();
	Ref<_HTMLTag> autocomplete_tel_extension();
	Ref<_HTMLTag> autocomplete_impp();
	Ref<_HTMLTag> autocomplete_url();
	Ref<_HTMLTag> autocomplete_photo();

	Ref<_HTMLTag> onclick(const String &val);

	Ref<_HTMLTag> checked(const bool val = true);
	Ref<_HTMLTag> selected(const bool val = true);
	Ref<_HTMLTag> autofocus(const bool val = true);
	Ref<_HTMLTag> disabled(const bool val = true);
	Ref<_HTMLTag> multiple(const bool val = true);
	Ref<_HTMLTag> required(const bool val = true);
	Ref<_HTMLTag> spellcheck(const bool val);

	Ref<_HTMLTag> max(const String &val);
	Ref<_HTMLTag> min(const String &val);
	Ref<_HTMLTag> step(const String &val);
	Ref<_HTMLTag> step_any();

	Ref<_HTMLTag> minlength(const int val);
	Ref<_HTMLTag> minlength(const String &val);
	Ref<_HTMLTag> maxlength(const int val);
	Ref<_HTMLTag> maxlength(const String &val);
	Ref<_HTMLTag> size(const int val);
	Ref<_HTMLTag> size(const String &val);

	Ref<_HTMLTag> width(const int val);
	Ref<_HTMLTag> width(const String &val);
	Ref<_HTMLTag> height(const int val);
	Ref<_HTMLTag> height(const String &val);

	Ref<_HTMLTag> pattern(const String &val);

	Ref<_HTMLTag> method(const String &val);
	Ref<_HTMLTag> method_get();
	Ref<_HTMLTag> method_post();

	Ref<_HTMLTag> action(const String &val);
	Ref<_HTMLTag> type(const String &val);
	Ref<_HTMLTag> placeholder(const String &val);
	Ref<_HTMLTag> fora(const String &val); // for attrib -> for is reserved keyword

	Ref<_HTMLTag> rel(const String &val);
	Ref<_HTMLTag> rel_stylesheet();
	Ref<_HTMLTag> rel_alternate();
	Ref<_HTMLTag> rel_author();
	Ref<_HTMLTag> rel_bookmark();
	Ref<_HTMLTag> rel_external();
	Ref<_HTMLTag> rel_help();
	Ref<_HTMLTag> rel_license();
	Ref<_HTMLTag> rel_next();
	Ref<_HTMLTag> rel_nofollow();
	Ref<_HTMLTag> rel_noopener();
	Ref<_HTMLTag> rel_noreferrer();
	Ref<_HTMLTag> rel_prev();
	Ref<_HTMLTag> rel_search();
	Ref<_HTMLTag> rel_tag();

	Ref<_HTMLTag> charset(const String &val);
	Ref<_HTMLTag> charset_utf_8();

	Ref<_HTMLTag> itbutton();
	Ref<_HTMLTag> itcheckbox();
	Ref<_HTMLTag> itcolor();
	Ref<_HTMLTag> itdate();
	Ref<_HTMLTag> itdatetime_local();
	Ref<_HTMLTag> itemail();
	Ref<_HTMLTag> itfile();
	Ref<_HTMLTag> ithidden();
	Ref<_HTMLTag> itimage();
	Ref<_HTMLTag> itmonth();
	Ref<_HTMLTag> itnumber();
	Ref<_HTMLTag> itpassword();
	Ref<_HTMLTag> itradio();
	Ref<_HTMLTag> itrange();
	Ref<_HTMLTag> itreset();
	Ref<_HTMLTag> itsearch();
	Ref<_HTMLTag> itsubmit();
	Ref<_HTMLTag> ittel();
	Ref<_HTMLTag> ittext();
	Ref<_HTMLTag> ittime();
	Ref<_HTMLTag> iturl();
	Ref<_HTMLTag> itweek();

	Ref<_HTMLTag> inputmode_none();
	Ref<_HTMLTag> inputmode_text();
	Ref<_HTMLTag> inputmode_decimal();
	Ref<_HTMLTag> inputmode_numeric();
	Ref<_HTMLTag> inputmode_tel();
	Ref<_HTMLTag> inputmode_search();
	Ref<_HTMLTag> inputmode_email();
	Ref<_HTMLTag> inputmode_url();

	Ref<_HTMLTag> attrib(const String &attr, const String &val);

	Ref<_HTMLTag> start(const String &p_new_tag, const bool p_simple = false);
	Ref<_HTMLTag> reset();
	Ref<_HTMLTag> close();

	Ref<_HTMLBuilder> f();

	bool has_data();

	_HTMLTag();

	Ref<_HTMLBuilder> owner;
	bool simple;
	String result;

protected:
	static void _bind_methods();
};

class _HTMLBuilder : public Reference {
	GDCLASS(_HTMLBuilder, Reference)

public:
	String get_result();
	void set_result(const String &str);

	Ref<_HTMLBuilder> comment(const String &val);
	Ref<_HTMLTag> doctype();
	Ref<_HTMLBuilder> doctype(const String &val);

	Ref<_HTMLTag> a(const String &href = "", const String &cls = "", const String &id = "");
	Ref<_HTMLBuilder> fa(const String &href, const String &body, const String &cls = "", const String &id = "");
	Ref<_HTMLTag> abbr();
	Ref<_HTMLTag> acronym(); // Not supported in HTML5.
	Ref<_HTMLTag> address();
	Ref<_HTMLTag> applet(); // Not supported in HTML5.
	Ref<_HTMLTag> area();
	Ref<_HTMLTag> article();
	Ref<_HTMLTag> aside();
	Ref<_HTMLTag> audio();
	Ref<_HTMLTag> b();
	Ref<_HTMLTag> basefont(); // Not supported in HTML5.
	Ref<_HTMLTag> bdi();
	Ref<_HTMLTag> bdo();
	Ref<_HTMLTag> big(); // Not supported in HTML5.
	Ref<_HTMLTag> blockquote();
	Ref<_HTMLTag> body();
	Ref<_HTMLTag> br();
	Ref<_HTMLTag> button();
	Ref<_HTMLTag> canvas();
	Ref<_HTMLTag> caption();
	Ref<_HTMLTag> center(); // Not supported in HTML5.
	Ref<_HTMLTag> cite();
	Ref<_HTMLTag> code();
	Ref<_HTMLTag> col();
	Ref<_HTMLTag> colgroup();
	Ref<_HTMLTag> data();
	Ref<_HTMLTag> datalist();
	Ref<_HTMLTag> dd();
	Ref<_HTMLTag> del();
	Ref<_HTMLTag> details();
	Ref<_HTMLTag> dfn();
	Ref<_HTMLTag> dialog();
	Ref<_HTMLTag> dir(); // Not supported in HTML5.
	Ref<_HTMLTag> div(const String &cls = "", const String &id = "");
	Ref<_HTMLBuilder> fdiv(const String &body, const String &cls = "", const String &id = "");
	Ref<_HTMLTag> dl();
	Ref<_HTMLTag> dt();
	Ref<_HTMLTag> em();
	Ref<_HTMLTag> embed();
	Ref<_HTMLTag> fieldset();
	Ref<_HTMLTag> figcaption();
	Ref<_HTMLTag> figure();
	Ref<_HTMLTag> font(); // Not supported in HTML5.
	Ref<_HTMLTag> footer();
	Ref<_HTMLTag> form();
	Ref<_HTMLTag> frame(); // Not supported in HTML5.
	Ref<_HTMLTag> frameset(); // Not supported in HTML5.
	Ref<_HTMLTag> h1();
	Ref<_HTMLTag> h2();
	Ref<_HTMLTag> h3();
	Ref<_HTMLTag> h4();
	Ref<_HTMLTag> h5();
	Ref<_HTMLTag> h6();
	Ref<_HTMLTag> head();
	Ref<_HTMLTag> header();
	Ref<_HTMLTag> hr();
	Ref<_HTMLTag> html();

	Ref<_HTMLTag> i();
	Ref<_HTMLTag> iframe();
	Ref<_HTMLTag> img();
	Ref<_HTMLTag> input();
	Ref<_HTMLTag> ins();
	Ref<_HTMLTag> kbd();
	Ref<_HTMLTag> label();
	Ref<_HTMLTag> legend();
	Ref<_HTMLTag> li();
	Ref<_HTMLTag> link();
	Ref<_HTMLTag> main();
	Ref<_HTMLTag> map();
	Ref<_HTMLTag> mark();
	Ref<_HTMLTag> meta();
	Ref<_HTMLTag> meter();

	Ref<_HTMLTag> nav();
	Ref<_HTMLTag> noframes(); // Not supported in HTML5.
	Ref<_HTMLTag> noscript();
	Ref<_HTMLTag> object();
	Ref<_HTMLTag> ol();
	Ref<_HTMLTag> optgroup();
	Ref<_HTMLTag> option(const String &value = "");
	Ref<_HTMLBuilder> foption(const String &value, const String &body, const bool selected = false);
	Ref<_HTMLTag> output();
	Ref<_HTMLTag> p();
	Ref<_HTMLTag> param();
	Ref<_HTMLTag> picture();
	Ref<_HTMLTag> pre();
	Ref<_HTMLTag> progress();
	Ref<_HTMLTag> q();
	Ref<_HTMLTag> rp();

	Ref<_HTMLTag> rt();
	Ref<_HTMLTag> ruby();
	Ref<_HTMLTag> s();
	Ref<_HTMLTag> samp();
	Ref<_HTMLTag> script();
	Ref<_HTMLTag> section();
	Ref<_HTMLTag> select(const String &name = "", const String &cls = "", const String &id = "");
	Ref<_HTMLTag> small();
	Ref<_HTMLTag> source();
	Ref<_HTMLTag> span();
	Ref<_HTMLTag> strike(); // Not supported in HTML5
	Ref<_HTMLTag> strong();
	Ref<_HTMLTag> style();
	Ref<_HTMLTag> sub();
	Ref<_HTMLTag> summary();
	Ref<_HTMLTag> sup();

	Ref<_HTMLTag> svg();
	Ref<_HTMLTag> table();
	Ref<_HTMLTag> tbody();
	Ref<_HTMLTag> td();
	Ref<_HTMLTag> templateh();
	Ref<_HTMLTag> textarea(const String &name = "", const String &cls = "", const String &id = "");
	Ref<_HTMLBuilder> ftextarea(const String &name, const String &body, const String &cls = "", const String &id = "");
	Ref<_HTMLTag> tfoot();
	Ref<_HTMLTag> th();
	Ref<_HTMLTag> thead();
	Ref<_HTMLTag> time();
	Ref<_HTMLTag> title();
	Ref<_HTMLTag> tr();
	Ref<_HTMLTag> track();
	Ref<_HTMLTag> tt(); // Not supported in HTML5.
	Ref<_HTMLTag> u();
	Ref<_HTMLTag> ul();
	Ref<_HTMLTag> var();
	Ref<_HTMLTag> video();
	Ref<_HTMLTag> wbr();

	// closing tags c prefix means close
	// Note simple tags should not have these like <br>
	// Note that I might have a few that shouldn't be here, those will be removed as I find them
	Ref<_HTMLBuilder> ca();
	Ref<_HTMLBuilder> cabbr();
	Ref<_HTMLBuilder> cacronym();
	Ref<_HTMLBuilder> caddress();
	Ref<_HTMLBuilder> capplet();
	Ref<_HTMLBuilder> carea();
	Ref<_HTMLBuilder> carticle();
	Ref<_HTMLBuilder> caside();
	Ref<_HTMLBuilder> caudio();
	Ref<_HTMLBuilder> cb();
	Ref<_HTMLBuilder> cbasefont();
	Ref<_HTMLBuilder> cbdi();
	Ref<_HTMLBuilder> cbdo();
	Ref<_HTMLBuilder> cbig();
	Ref<_HTMLBuilder> cblockquote();
	Ref<_HTMLBuilder> cbody();
	Ref<_HTMLBuilder> cbutton();
	Ref<_HTMLBuilder> ccanvas();

	Ref<_HTMLBuilder> ccaption();
	Ref<_HTMLBuilder> ccenter();
	Ref<_HTMLBuilder> ccite();
	Ref<_HTMLBuilder> ccode();
	Ref<_HTMLBuilder> ccol();
	Ref<_HTMLBuilder> ccolgroup();
	Ref<_HTMLBuilder> cdata();
	Ref<_HTMLBuilder> cdatalist();
	Ref<_HTMLBuilder> cdd();
	Ref<_HTMLBuilder> cdel();
	Ref<_HTMLBuilder> cdetails();
	Ref<_HTMLBuilder> cdfn();
	Ref<_HTMLBuilder> cdialog();
	Ref<_HTMLBuilder> cdir();
	Ref<_HTMLBuilder> cdiv();
	Ref<_HTMLBuilder> cdl();
	Ref<_HTMLBuilder> cdt();

	Ref<_HTMLBuilder> cem();
	Ref<_HTMLBuilder> cembed();
	Ref<_HTMLBuilder> cfieldset();
	Ref<_HTMLBuilder> cfigcaption();
	Ref<_HTMLBuilder> cfigure();
	Ref<_HTMLBuilder> cfont();
	Ref<_HTMLBuilder> cfooter();
	Ref<_HTMLBuilder> cform();
	Ref<_HTMLBuilder> cframe();
	Ref<_HTMLBuilder> cframeset();
	Ref<_HTMLBuilder> ch1();
	Ref<_HTMLBuilder> ch2();
	Ref<_HTMLBuilder> ch3();
	Ref<_HTMLBuilder> ch4();
	Ref<_HTMLBuilder> ch5();
	Ref<_HTMLBuilder> ch6();
	Ref<_HTMLBuilder> chead();
	Ref<_HTMLBuilder> cheader();
	Ref<_HTMLBuilder> chr();
	Ref<_HTMLBuilder> chtml();

	Ref<_HTMLBuilder> ci();
	Ref<_HTMLBuilder> ciframe();
	Ref<_HTMLBuilder> cimg();
	Ref<_HTMLBuilder> cinput();
	Ref<_HTMLBuilder> cins();
	Ref<_HTMLBuilder> ckbd();
	Ref<_HTMLBuilder> clabel();
	Ref<_HTMLBuilder> clegend();
	Ref<_HTMLBuilder> cli();
	Ref<_HTMLBuilder> clink();
	Ref<_HTMLBuilder> cmain();
	Ref<_HTMLBuilder> cmap();
	Ref<_HTMLBuilder> cmark();
	Ref<_HTMLBuilder> cmeta();
	Ref<_HTMLBuilder> cmeter();

	Ref<_HTMLBuilder> cnav();
	Ref<_HTMLBuilder> cnoframes();
	Ref<_HTMLBuilder> cnoscript();
	Ref<_HTMLBuilder> cobject();
	Ref<_HTMLBuilder> c_ol();
	Ref<_HTMLBuilder> coptgroup();
	Ref<_HTMLBuilder> coption();
	Ref<_HTMLBuilder> coutput();
	Ref<_HTMLBuilder> cp();
	Ref<_HTMLBuilder> cparam();
	Ref<_HTMLBuilder> cpicture();
	Ref<_HTMLBuilder> cpre();
	Ref<_HTMLBuilder> cprogress();
	Ref<_HTMLBuilder> cq();
	Ref<_HTMLBuilder> crp();

	Ref<_HTMLBuilder> crt();
	Ref<_HTMLBuilder> cruby();
	Ref<_HTMLBuilder> cs();
	Ref<_HTMLBuilder> csamp();
	Ref<_HTMLBuilder> cscript();
	Ref<_HTMLBuilder> csection();
	Ref<_HTMLBuilder> cselect();
	Ref<_HTMLBuilder> csmall();
	Ref<_HTMLBuilder> csource();
	Ref<_HTMLBuilder> cspan();
	Ref<_HTMLBuilder> cstrike();
	Ref<_HTMLBuilder> cstrong();
	Ref<_HTMLBuilder> cstyle();
	Ref<_HTMLBuilder> csub();
	Ref<_HTMLBuilder> csummary();
	Ref<_HTMLBuilder> csup();

	Ref<_HTMLBuilder> csvg();
	Ref<_HTMLBuilder> ctable();
	Ref<_HTMLBuilder> ctbody();
	Ref<_HTMLBuilder> ctd();
	Ref<_HTMLBuilder> ctemplateh();
	Ref<_HTMLBuilder> ctextarea();
	Ref<_HTMLBuilder> ctfoot();
	Ref<_HTMLBuilder> cth();
	Ref<_HTMLBuilder> cthead();
	Ref<_HTMLBuilder> ctime();
	Ref<_HTMLBuilder> ctitle();
	Ref<_HTMLBuilder> ctr();
	Ref<_HTMLBuilder> ctrack();
	Ref<_HTMLBuilder> ctt();
	Ref<_HTMLBuilder> cu();
	Ref<_HTMLBuilder> cul();
	Ref<_HTMLBuilder> cvar();
	Ref<_HTMLBuilder> cvideo();
	Ref<_HTMLBuilder> cwbr();

	Ref<_HTMLTag> form_get(const String &action = "", const String &cls = "", const String &id = "");
	Ref<_HTMLTag> form_post(const String &action = "", const String &cls = "", const String &id = "");
	// will add a csrf token from request
	//Ref<_HTMLBuilder> form_post(const String &action, Request *request, const String &cls = "", const String &id = "");

	Ref<_HTMLTag> input_button(const String &name = "", const String &value = "", const String &cls = "", const String &id = "");
	Ref<_HTMLTag> input_checkbox(const String &name = "", const String &value = "", const bool checked = false, const String &cls = "", const String &id = "");
	Ref<_HTMLTag> input_color(const String &name = "", const String &value = "", const String &cls = "", const String &id = "");
	Ref<_HTMLTag> input_date(const String &name = "", const String &value = "", const String &cls = "", const String &id = "", const String &date_min = "", const String &date_max = "", const String &date_step = "");
	Ref<_HTMLTag> input_datetime_local(const String &name = "", const String &value = "", const String &cls = "", const String &id = "", const String &date_min = "", const String &date_max = "", const String &date_step = "");
	Ref<_HTMLTag> input_email(const String &name = "", const String &value = "", const String &placeholder = "", const String &cls = "", const String &id = "");
	Ref<_HTMLTag> input_file(const String &name = "", const String &accept = "", const String &cls = "", const String &id = "");
	Ref<_HTMLTag> input_image(const String &name = "", const String &src = "", const String &alt = "", const String &cls = "", const String &id = "", const int width = 0, const int height = 0);
	Ref<_HTMLTag> input_month(const String &name = "", const String &cls = "", const String &id = "");
	Ref<_HTMLTag> input_number(const String &name = "", const String & = "", const String & = "", const String &cls = "", const String &id = "");
	Ref<_HTMLTag> input_password(const String &name = "", const String &value = "", const String &placeholder = "", const String &cls = "", const String &id = "", const String &minlength = "", const String &maxlength = "", const String &size = "");
	Ref<_HTMLTag> input_radio(const String &name = "", const String &value = "", const String &cls = "", const String &id = "");
	Ref<_HTMLTag> input_range(const String &name = "", const String &value = "", const String &vmin = "", const String &vmax = "", const String &vstep = "", const String &cls = "", const String &id = "");
	Ref<_HTMLTag> input_reset(const String &name = "", const String &value = "", const String &cls = "", const String &id = "");
	Ref<_HTMLTag> input_search(const String &name = "", const String &value = "", const String &placeholder = "", const String &cls = "", const String &id = "", const String &minlength = "", const String &maxlength = "", const String &size = "", const String &pattern = "");
	Ref<_HTMLTag> input_submit(const String &value = "", const String &cls = "", const String &id = "");
	Ref<_HTMLTag> input_tel(const String &name = "", const String &value = "", const String &placeholder = "", const String &cls = "", const String &id = "", const String &minlength = "", const String &maxlength = "", const String &size = "", const String &pattern = "");
	Ref<_HTMLTag> input_text(const String &name = "", const String &value = "", const String &placeholder = "", const String &cls = "", const String &id = "", const String &minlength = "", const String &maxlength = "", const String &size = "");
	Ref<_HTMLTag> input_time(const String &name = "", const String &cls = "", const String &id = "", const String &vmin = "", const String &vmax = "", const String &vstep = "");
	Ref<_HTMLTag> input_url(const String &name = "", const String &value = "", const String &placeholder = "", const String &cls = "", const String &id = "", const String &minlength = "", const String &maxlength = "", const String &size = "");
	Ref<_HTMLTag> input_week(const String &name = "", const String &cls = "", const String &id = "", const String &vmin = "", const String &vmax = "");
	Ref<_HTMLTag> input_hidden(const String &name = "", const String &value = "");

	Ref<_HTMLBuilder> flabel(const String &pfor, const String &plabel, const String &cls = "", const String &id = "");

	Ref<_HTMLBuilder> csrf_token(const String &token);
	//Ref<_HTMLBuilder> csrf_token(Request *request);

	void f();

	// write
	Ref<_HTMLBuilder> w(const String &val);

	Ref<_HTMLBuilder> wn(const double val, int p_decimals = -1);
	Ref<_HTMLBuilder> wns(const double val);
	Ref<_HTMLBuilder> wr(const double val, const bool p_trailing = true);
	Ref<_HTMLBuilder> wi(const int64_t val, const int base = 10, const bool capitalize_hex = false);
	Ref<_HTMLBuilder> wui(const uint64_t val, const int base = 10, const bool capitalize_hex = false);
	Ref<_HTMLBuilder> wbn(const bool val);
	Ref<_HTMLBuilder> wbs(const bool val);

	// write_escaped
	Ref<_HTMLBuilder> we(const String &val);

	Ref<_HTMLBuilder> write_tag();

	_HTMLBuilder();
	virtual ~_HTMLBuilder();

	String result;

protected:
	static void _bind_methods();

	Ref<_HTMLTag> tag;
};

#endif

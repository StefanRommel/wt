/*
 * Copyright (C) 2011 Emweb bvba, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include <boost/test/unit_test.hpp>

#include <boost/filesystem/operations.hpp>

#include "Wt/Test/WTestEnvironment"
#include "Wt/WApplication"
#include "Wt/WString"

#include <iostream>

namespace {

void pluralResourceBundleException(const std::string &resourceName)
{
  Wt::Test::WTestEnvironment environment;
  Wt::WApplication app(environment);

  std::string file = app.appRoot() + resourceName;
  BOOST_REQUIRE(boost::filesystem::exists(file + ".xml"));
  
  app.messageResourceBundle().use(file);

  BOOST_REQUIRE(Wt::WString::tr("file").toUTF8() == "??file??");
}

std::string trn(const std::string &key, int n)
{
  return Wt::WString::trn(key, n).arg(n).toUTF8();
}

}

BOOST_AUTO_TEST_CASE( I18n_messageResourceBundleTest )
{
  BOOST_REQUIRE(Wt::WString::tr("welcome-text").toUTF8() == "??welcome-text??");
  
  Wt::Test::WTestEnvironment environment;
  Wt::WApplication app(environment);

  BOOST_REQUIRE(Wt::WString::tr("welcome-text").toUTF8() == "??welcome-text??");

  app.messageResourceBundle().use(app.appRoot() + "private/i18n/plain");

  std::string welcome = Wt::WString::tr("welcome-text").arg("Joske").toUTF8();
  BOOST_REQUIRE(welcome == 
		"Welcome dear visiter, Joske of the WFooBar magic website !");

  BOOST_REQUIRE(Wt::WString::tr("welcome").toUTF8() == "??welcome??");

  app.setLocale("nl");
  welcome = Wt::WString::tr("welcome-text").arg("Joske").toUTF8();
  BOOST_REQUIRE(welcome == 
		"Welkom beste bezoeker, Joske van de WFooBar magic website !");
}

BOOST_AUTO_TEST_CASE( I18n_pluralResourceBundleException1 )
{
  pluralResourceBundleException("private/i18n/plural_err_1");
}

BOOST_AUTO_TEST_CASE( I18n_pluralResourceBundleException2 )
{
  pluralResourceBundleException("private/i18n/plural_err_2");
}

BOOST_AUTO_TEST_CASE( I18n_pluralResourceBundleException3 )
{
  pluralResourceBundleException("private/i18n/plural_err_3");
}

BOOST_AUTO_TEST_CASE( I18n_pluralResourceBundleException4 )
{
  pluralResourceBundleException("private/i18n/plural_err_4");
}

BOOST_AUTO_TEST_CASE( I18n_pluralResourceBundleException5 )
{
  pluralResourceBundleException("private/i18n/plural_err_5");
}

BOOST_AUTO_TEST_CASE( I18n_pluralResourceBundle1 )
{
  Wt::Test::WTestEnvironment environment;
  Wt::WApplication app(environment);

  app.messageResourceBundle().use(app.appRoot() + "private/i18n/plural");

  BOOST_REQUIRE(trn("file", 0) == "0 files");
  BOOST_REQUIRE(trn("file", 1) == "1 file");
  BOOST_REQUIRE(trn("file", 6) == "6 files");

  BOOST_REQUIRE(Wt::WString::tr("welcome").toUTF8() == "??welcome??");

  app.setLocale("pl");
  
  BOOST_REQUIRE(trn("file", 0) == "0 pliko'w");
  BOOST_REQUIRE(trn("file", 1) == "1 plik");
  BOOST_REQUIRE(trn("file", 2) == "2 pliki");
  BOOST_REQUIRE(trn("file", 6) == "6 pliko'w");
  BOOST_REQUIRE(trn("file", 23) == "23 pliki");
  BOOST_REQUIRE(trn("file", 26) == "26 pliko'w");

  BOOST_REQUIRE(Wt::WString::tr("welcome").toUTF8() == "??welcome??");
}

BOOST_AUTO_TEST_CASE( I18n_findCaseException1 )
{
  Wt::Test::WTestEnvironment environment;
  Wt::WApplication app(environment);

  app.messageResourceBundle().use("private/i18n/plural_findcase_err1");

  std::string error;
  try {
    Wt::WString::trn("file", 1).toUTF8();
  } catch(std::logic_error &e) {
    error = e.what();
  }

  BOOST_REQUIRE(error == 
		"Expression 'n-10' evaluates to '-9' for n=1 and values "
		"smaller than 0 are not allowed.");
}

BOOST_AUTO_TEST_CASE( I18n_findCaseException2 )
{
  Wt::Test::WTestEnvironment environment;
  Wt::WApplication app(environment);

  app.messageResourceBundle().use(app.appRoot() + 
				  "private/i18n/plural_findcase_err2");

  std::string error;
  try {
    Wt::WString::trn("file", 1).toUTF8();
  } catch(std::logic_error &e) {
    error = e.what();
  }

  BOOST_REQUIRE(error == 
		"Expression '2' evaluates to '2' for n=1 which is greater "
		"than the list of cases (size=1).");
}
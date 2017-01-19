/*
    QEagleLib * Qt based library for managing Eagle CAD XML files
    Copyright (C) 2012-2017 Mirai Computing (mirai.computing@gmail.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
//------------------------------------------------------------------------------
#include <iostream>
//------------------------------------------------------------------------------
#include "QEagleLib.h"
//------------------------------------------------------------------------------
//#define DEBUG_MSG

namespace Eagle
{

const double M_PI_DIV_180 = M_PI / 180.0;
inline double degtorad(double x) { return x * M_PI_DIV_180; }
inline double radtodeg(double x) { return x / M_PI_DIV_180; }

QString escapeXml(const QString &str)
{
 QString xml = str;
 xml.replace("&", "&amp;");
 xml.replace("<", "&lt;");
 xml.replace(">", "&gt;");
 xml.replace("'", "&apos;");
 xml.replace("\"", "&quot;");
 return xml;
}

QString unescapeXml(const QString &str)
{
 QString xml = str;
 xml.replace("&quot;", "\"");
 xml.replace("&apos;", "'");
 xml.replace("&gt;", ">");
 xml.replace("&lt;", "<");
 xml.replace("&amp;", "&");
 return xml;
}

bool decodeTransformation(const QString& str, double& rotation, bool& reflection, bool& spin)
{
 if (str.length()>=2)
 {
  reflection = false;
  int mod_pos = 0;
  if ('S'==str.at(mod_pos))
  {
   mod_pos++;
   spin = true;
  }
  if ('M'==str.at(mod_pos))
  {
   mod_pos++;
   reflection = true;
  }
  if ('R'==str.at(mod_pos))
  {
   mod_pos++;
   bool ok = false;
   QString ss = str.mid(mod_pos);
   double value = ss.toDouble(&ok);
   if (ok)
   {
    rotation = value;
    return true;
   }
  }
 }
 return false;
}

QString encodeTransformation(const double rotation, const bool reflection, const bool spin)
{
 QString result;
 if (spin) result.append("S");
 if (reflection) result.append("M");
 result.append(QString("R%1").arg(rotation));
 return result;
}

CEntity::CEntity(void)
{
 clear();
}

CEntity::~CEntity(void)
{
 clear();
}

QString CEntity::toString(const bool value)
{
 if (value) return "yes"; else return "no";
}

void CEntity::clear(void)
{
 //
}

void CEntity::show(std::ostream& out, const int level)
{
 for (int i = 0; i < level; i++)  out<<"\t";
}

bool CEntity::readFromXML(const QDomElement& root)
{
 return true;
}

bool CEntity::writeToXML(QDomDocument& host, QDomElement& root, const bool defaults)
{
 return (!host.isNull() && !root.isNull());
}

//------------------------------------------------------------------------------

CSettings::CSettings(const CSettings& settings)
{
 assign(settings);
}

CSettings::CSettings(void)
{
 //clear();
}

CSettings::~CSettings(void)
{
 //
}

QString CSettings::toString(const CSettings::VerticalText value)
{
 switch (value)
 {
  case CSettings::vtUp: return "up";
  case CSettings::vtDown: return "down";
 }
 return "";
}

void CSettings::operator =(const CSettings& settings)
{
 assign(settings);
}

void CSettings::clear(void)
{
 m_AlwaysVectorFont = false;
 m_VerticalText = CSettings::vtUp;
}

void CSettings::assign(const CSettings& settings)
{
 m_AlwaysVectorFont = settings.m_AlwaysVectorFont;
 m_VerticalText = settings.m_VerticalText;
}

void CSettings::show(std::ostream& out, const int level)
{
 CEntity::show(out,level);
 out<<"Settings:{AlwaysVectorFont="<<CEntity::toString(m_AlwaysVectorFont).toUtf8().data()
    <<", VerticalText="<<toString(m_VerticalText).toUtf8().data()<<"}"<<std::endl;
}

bool CSettings::readFromXML(const QDomElement& root)
{
 int result = 0;
 QDomElement e = root.firstChildElement("setting");
 while (!e.isNull())
 {
  QString s = e.attribute("alwaysvectorfont");
  if (!s.isEmpty())
  {
   if (s=="yes") { m_AlwaysVectorFont = true; result |= 0x1; }
   if (s=="no") { m_AlwaysVectorFont = false; result |= 0x1; }
  }
  s = e.attribute("verticaltext");
  if (!s.isEmpty())
  {
   if (s=="up") { m_VerticalText = CSettings::vtUp; result |= 0x2; }
   if (s=="down") { m_VerticalText = CSettings::vtDown; result |=0x2; }
  }
  e = e.nextSiblingElement("setting");
 }
 return (result==0x3);
}

bool CSettings::writeToXML(QDomDocument& host, QDomElement& root, const bool defaults)
{
 if (!host.isNull() && !root.isNull())
 {
  QDomElement e = host.createElement("settings");
  QDomElement c = host.createElement("setting");
  c.setAttribute("alwaysvectorfont",CEntity::toString(m_AlwaysVectorFont));
  e.appendChild(c);
  if (defaults || CSettings::vtUp!=m_VerticalText)
  {
   c = host.createElement("setting");
   c.setAttribute("verticaltext",toString(m_VerticalText));
   e.appendChild(c);
  }
  root.appendChild(e);
  return true;
 }
 return false;
}

//------------------------------------------------------------------------------

CGrid::CGrid(const CGrid& grid)
{
}

CGrid::CGrid(void)
{
 clear();
}

CGrid::~CGrid(void)
{
 clear();
}

QString CGrid::toString(const CGrid::Unit value)
{
 switch (value)
 {
  case CGrid::guMic: return "mic";
  case CGrid::guMM: return "mm";
  case CGrid::guMil: return "mil";
  case CGrid::guInch: return "inch";
 }
 return "";
}

QString CGrid::toString(const CGrid::Style value)
{
 switch (value)
 {
  case CGrid::gsLines: return "lines";
  case CGrid::gsDots: return "dots";
 }
 return "";
}

void CGrid::clear(void)
{
 m_Distance = 0.1;
 m_UnitDist = CGrid::guInch;
 m_Unit = CGrid::guInch;
 m_Style = CGrid::gsLines;
 m_Multiple = 1;
 m_Display = false;
 m_AltDistance = 1.0;
 m_AltUnitDist = CGrid::guInch;
 m_AltUnit = CGrid::guInch;
}

void CGrid::assign(const CGrid& grid)
{
 m_Distance = grid.m_Distance;
 m_UnitDist = grid.m_UnitDist;
 m_Unit = grid.m_Unit;
 m_Style = grid.m_Style;
 m_Multiple = grid.m_Multiple;
 m_Display = grid.m_Display;
 m_AltDistance = grid.m_AltDistance;
 m_AltUnitDist = grid.m_AltUnitDist;
 m_AltUnit = grid.m_AltUnit;
}

void CGrid::operator =(const CGrid& grid)
{
 assign(grid);
}

void CGrid::scale(const double factor)
{
 m_Distance *= factor;
 m_AltDistance *= factor;
}

void CGrid::show(std::ostream& out, const int level)
{
 CEntity::show(out,level);
 out<<"Grid:{Distance="<<m_Distance<<", UnitDist="
    <<toString(m_UnitDist).toUtf8().data()<<", Unit="
    <<toString(m_Unit).toUtf8().data()<<", Style="
    <<toString(m_Style).toUtf8().data()<<", Multiple="<<m_Multiple<<", Display"
    <<CEntity::toString(m_Display).toUtf8().data()<<", AltDistance="
    <<m_AltDistance<<", AltUnitDist="
    <<toString(m_AltUnitDist).toUtf8().data()
    <<", AltUnit="<<toString(m_AltUnit).toUtf8().data()
    <<"}"<<std::endl;
}

bool CGrid::readFromXML(const QDomElement& root)
{
 int result = 0;
 QDomElement e = root;
 if (e.nodeName()!="grid") e = e.nextSiblingElement("grid");
 if (!e.isNull())
 {
  QString s = e.attribute("distance");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toDouble(&ok);
   if (ok) { m_Distance = value; result |= 0x01; }
  }
  s = e.attribute("unitdist");
  if (!s.isEmpty())
  {
   CGrid::Unit value = CGrid::guInch;
   if (stringToUnit(s,value)) { m_UnitDist = value; result |= 0x02; }
  }
  s = e.attribute("unit");
  if (!s.isEmpty())
  {
   CGrid::Unit value = CGrid::guInch;
   if (stringToUnit(s,value)) { m_Unit = value; result |= 0x04; }
  }
  s = e.attribute("style");
  if (!s.isEmpty())
  {
   if (s=="lines") { m_Style = CGrid::gsLines; result |= 0x08; }
   else if (s=="dots") { m_Style = CGrid::gsDots; result |= 0x08; }
  }
  s = e.attribute("multiple");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toInt(&ok);
   if (ok) { m_Multiple = value; result |= 0x10; }
  }
  s = e.attribute("display");
  if (!s.isEmpty())
  {
   if (s=="yes") { m_Display = true; result |= 0x20; }
   else if (s=="no") { m_Display = false; result |= 0x20; }
  }
  s = e.attribute("altdistance");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toDouble(&ok);
   if (ok) { m_AltDistance = value; result |= 0x40; }
  }
  s = e.attribute("altunitdist");
  if (!s.isEmpty())
  {
   CGrid::Unit value = CGrid::guInch;
   if (stringToUnit(s,value)) { m_AltUnitDist = value; result |= 0x80; }
  }
  s = e.attribute("altunit");
  if (!s.isEmpty())
  {
   CGrid::Unit value = CGrid::guInch;
   if (stringToUnit(s,value)) { m_AltUnit = value; result |= 0x100; }
  }
 }
 return (result==0x1ff);
}

bool CGrid::writeToXML(QDomDocument& host, QDomElement& root, const bool defaults)
{
 if (!host.isNull() && !root.isNull())
 {
  QDomElement e = host.createElement("grid");
  e.setAttribute("distance",QString("%1").arg(m_Distance));
  e.setAttribute("unitdist",toString(m_UnitDist));
  e.setAttribute("unit",toString(m_Unit));
  if (defaults || CGrid::gsLines!=m_Style)
  {
   e.setAttribute("style",toString(m_Style));
  }
  if (defaults || 1!=m_Multiple)
  {
   e.setAttribute("multiple",QString("%1").arg(m_Multiple));
  }
  if (defaults || false!=m_Display)
  {
   e.setAttribute("display",CEntity::toString(m_Display));
  }
  e.setAttribute("altdistance",QString("%1").arg(m_AltDistance));
  e.setAttribute("altunitdist",toString(m_AltUnitDist));
  e.setAttribute("altunit",toString(m_AltUnit));
  root.appendChild(e);
  return true;
 }
 return false;
}

bool CGrid::stringToUnit(const QString& name, CGrid::Unit& value)
{
 if (name=="mic") { value = CGrid::guMic; return true; }
 else if (name=="mm") { value = CGrid::guMM; return true; }
 else if (name=="mil") { value = CGrid::guMil; return true; }
 else if (name=="inch") { value = CGrid::guInch; return true; }
 return false;
}

//------------------------------------------------------------------------------

CLayer::CLayer(const TLayer layer, const QString& name, const int color,
 const int fill, const bool visible, const bool active)
{
 m_Layer = layer;
 m_Name = name;
 m_Color = color;
 m_Fill = fill;
 m_Visible = visible;
 m_Active = active;
}

CLayer::CLayer(const CLayer& layer)
{
 assign(layer);
}

CLayer::CLayer(void)
{
 clear();
}

CLayer::~CLayer(void)
{
 clear();
}

QString CLayer::layerName(const TLayer layer)
{
 switch (layer)
 {
  default:              return "";
  case LAYER_TOP:       return "Top";
  case LAYER_LAYER2:    return "Layer2";
  case LAYER_LAYER3:    return "Layer3";
  case LAYER_LAYER4:    return "Layer4";
  case LAYER_LAYER5:    return "Layer5";
  case LAYER_LAYER6:    return "Layer6";
  case LAYER_LAYER7:    return "Layer7";
  case LAYER_LAYER8:    return "Layer8";
  case LAYER_LAYER9:    return "Layer9";
  case LAYER_LAYER10:   return "Layer10";
  case LAYER_LAYER11:   return "Layer11";
  case LAYER_LAYER12:   return "Layer12";
  case LAYER_LAYER13:   return "Layer13";
  case LAYER_LAYER14:   return "Layer14";
  case LAYER_LAYER15:   return "Layer15";
  case LAYER_BOTTOM:    return "Bottom";
  case LAYER_PADS:      return "Pads";
  case LAYER_VIAS:      return "Vias";
  case LAYER_UNROUTED:  return "unrouted";
  case LAYER_DIMENSION: return "Dimension";
  case LAYER_TPLACE:    return "tPlace";
  case LAYER_BPLACE:    return "bPlace";
  case LAYER_TORIGINS:  return "tOrigins";
  case LAYER_BORIGINS:  return "bOrigins";
  case LAYER_TNAMES:    return "tNames";
  case LAYER_BNAMES:    return "bNames";
  case LAYER_TVALUES:   return "tValues";
  case LAYER_BVALUES:   return "bValues";
  case LAYER_TSTOP:     return "tStop";
  case LAYER_BSTOP:     return "bStop";
  case LAYER_TCREAM:    return "tCream";
  case LAYER_BCREAM:    return "bCream";
  case LAYER_TFINISH:   return "tFinish";
  case LAYER_BFINISH:   return "bFinish";
  case LAYER_TGLUE:     return "tGlue";
  case LAYER_BGLUE:     return "bGlue";
  case LAYER_TTEST:     return "tTest";
  case LAYER_BTEST:     return "bTest";
  case LAYER_TKEEPOUT:  return "tKeepout";
  case LAYER_BKEEPOUT:  return "bKeepout";
  case LAYER_TRESTRICT: return "tRestrict";
  case LAYER_BRESTRICT: return "bRestrict";
  case LAYER_VRESTRICT: return "vRestrict";
  case LAYER_DRILLS:    return "Drills";
  case LAYER_HOLES:     return "Holes";
  case LAYER_MILLING:   return "Milling";
  case LAYER_MEASURES:  return "Measures";
  case LAYER_DOCUMENT:  return "Document";
  case LAYER_REFERENCE: return "Reference";
  case LAYER_TDOCU:     return "tDocu";
  case LAYER_BDOCU:     return "bDocu";
  case LAYER_NETS:      return "Nets";
  case LAYER_BUSSES:    return "Busses";
  case LAYER_PINS:      return "Pins";
  case LAYER_SYMBOLS:   return "Symbols";
  case LAYER_NAMES:     return "Names";
  case LAYER_VALUES:    return "Values";
  case LAYER_INFO:      return "Info";
  case LAYER_GUIDE:     return "Guide";
 }
 return "";
}

void CLayer::operator =(const CLayer& layer)
{
 assign(layer);
}

void CLayer::clear(void)
{
 m_Layer = 0;
 m_Name.clear();
 m_Color = 0;
 m_Fill = 0;
 m_Visible = true;
 m_Active = true;
}

void CLayer::assign(const CLayer& layer)
{
 m_Layer = layer.m_Layer;
 m_Name = layer.m_Name;
 m_Color = layer.m_Color;
 m_Fill = layer.m_Fill;
 m_Visible = layer.m_Visible;
 m_Active = layer.m_Active;
}

void CLayer::show(std::ostream& out, const int level)
{
 CEntity::show(out,level);
 out<<"Layer:{Layer="<<m_Layer<<", Name='"<<m_Name.toUtf8().data()
    <<"', Color="<<m_Color<<", Fill="<<m_Fill<<", Visible="
    <<CEntity::toString(m_Visible).toUtf8().data()<<", Active="
    <<CEntity::toString(m_Active).toUtf8().data()
    <<"}"<<std::endl;
}

bool CLayer::readFromXML(const QDomElement& root)
{
 int result = 0;
 QDomElement e = root;
 if (e.nodeName()!="layer") e = e.nextSiblingElement("layer");
 if (!e.isNull())
 {
  QString s = e.attribute("number");
  if (!s.isEmpty())
  {
   bool ok = false;
   int value = 0;
   value = s.toInt(&ok);
   if (ok) { m_Layer = value; result |= 0x01; }
  }
  s = e.attribute("name");
  if (!s.isEmpty())
  {
   m_Name = s; result |= 0x02;
  }
  s = e.attribute("color");
  if (!s.isEmpty())
  {
   bool ok = false;
   int value = 0;
   value = s.toInt(&ok);
   if (ok) { m_Color = value; result |= 0x04; }
  }
  s = e.attribute("fill");
  if (!s.isEmpty())
  {
   bool ok = false;
   int value = 0;
   value = s.toInt(&ok);
   if (ok) { m_Fill = value; result |= 0x08; }
  }
  s = e.attribute("visible");
  if (!s.isEmpty())
  {
   if (s=="yes") { m_Visible = true; result |= 0x10; }
   else if (s=="no") { m_Visible = false; result |= 0x10; }
  }
  s = e.attribute("active");
  if (!s.isEmpty())
  {
   if (s=="yes") { m_Active = true; result |= 0x20; }
   else if (s=="no") { m_Active = false; result |= 0x20; }
  }
#ifdef DEBUG_MSG
 std::cout<<"Layer:number "<<m_Layer<<std::endl;
 std::cout<<"Layer:name "<<m_Name.toUtf8().data()<<std::endl;
 std::cout<<"Layer:color "<<m_Color<<std::endl;
 std::cout<<"Layer:fill "<<m_Fill<<std::endl;
 std::cout<<"Layer:visible "<<m_Visible<<std::endl;
 std::cout<<"Layer:active "<<m_Active<<std::endl;
#endif
 }
 return (result==0x3f);
}

bool CLayer::writeToXML(QDomDocument& host, QDomElement& root, const bool defaults)
{
 if (!host.isNull() && !root.isNull())
 {
  QDomElement e = host.createElement("layer");
  e.setAttribute("number",QString("%1").arg(m_Layer));
  e.setAttribute("name",m_Name);
  e.setAttribute("color",QString("%1").arg(m_Color));
  e.setAttribute("fill",QString("%1").arg(m_Fill));
  if (defaults || !m_Visible)
  {
   e.setAttribute("visible",CEntity::toString(m_Visible));
  }
  if (defaults || !m_Active)
  {
   e.setAttribute("active",CEntity::toString(m_Active));
  }
  root.appendChild(e);
  return true;
 }
 return false;
}

//------------------------------------------------------------------------------

CClearance::CClearance(const CClearance& clearance)
{
 assign(clearance);
}

CClearance::CClearance(void)
{
 clear();
}

CClearance::~CClearance(void)
{
 clear();
}

void CClearance::operator =(const CClearance& clearance)
{
 assign(clearance);
}

void CClearance::clear(void)
{
 m_Class = 0;
 m_Value = 0.0;
}

void CClearance::assign(const CClearance& clearance)
{
 m_Class = clearance.m_Class;
 m_Value = clearance.m_Value;
}

void CClearance::show(std::ostream& out, const int level)
{
 CEntity::show(out,level);
 out<<"Clearance:{Class="<<m_Class<<", Value="<<m_Value<<"}"<<std::endl;
}

bool CClearance::readFromXML(const QDomElement& root)
{
 QDomElement e = root;
 if (e.nodeName()!="clearance") e = e.nextSiblingElement("clearance");
 if (!e.isNull())
 {
  QString s = e.attribute("class");
  if (!s.isEmpty())
  {
   bool ok = false;
   int value = s.toInt(&ok);
   if (ok) { m_Class = value; }
  }
  s = e.attribute("value");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toDouble(&ok);
   if (ok) { m_Value = value; }
  }
  return true;
 }
 return false;
}

bool CClearance::writeToXML(QDomDocument& host, QDomElement& root, const bool defaults)
{
 if (!host.isNull() && !root.isNull())
 {
  QDomElement e = host.createElement("clearance");
  e.setAttribute("class",QString("%1").arg(m_Class));
  if (defaults || 0.0!=m_Value)
  {
   e.setAttribute("value",QString("%1").arg(m_Value));
  }
  root.appendChild(e);
  return true;
 }
 return false;
}

//------------------------------------------------------------------------------

CDescription::CDescription(const CDescription& description)
{
 assign(description);
}

CDescription::CDescription(void)
{
 clear();
}

CDescription::~CDescription(void)
{
 clear();
}

void CDescription::operator =(const CDescription& description)
{
 assign(description);
}

void CDescription::clear(void)
{
 m_Description.clear();
 m_Language = "en";
}

void CDescription::assign(const CDescription& description)
{
 m_Description = description.m_Description;
 m_Language = description.m_Language;
}

void CDescription::show(std::ostream& out, const int level)
{
 CEntity::show(out,level);
 out<<"Description:{Language='"<<m_Language.toUtf8().data()<<"', {"
    <<m_Description.toUtf8().data()<<"} }"<<std::endl;
}

bool CDescription::readFromXML(const QDomElement& root)
{
 QDomElement e = root;
 if (e.nodeName()!="description") e = e.nextSiblingElement("description");
 if (!e.isNull())
 {
  QString s = e.attribute("language");
  if (!s.isEmpty())
  {
   m_Language = s;
  }
  QDomNode n = e.firstChild();
  while (!n.isNull())
  {
   if (n.isText())
   {
    m_Description = unescapeXml(n.nodeValue());
    break;
   }
   n = n.nextSibling();
  }
  return true;
 }
 return false;
}

bool CDescription::writeToXML(QDomDocument& host, QDomElement& root, const bool defaults)
{
 if (!host.isNull() && !root.isNull())
 {
  if (!m_Description.isEmpty())
  {
   QDomElement e = host.createElement("description");
   if (defaults || !(m_Language=="en"))
   {
    e.setAttribute("language",m_Language);
   }
   QDomText t = host.createTextNode(m_Description);
   e.appendChild(t);
   root.appendChild(e);
  }
  return true;
 }
 return false;
}

//------------------------------------------------------------------------------

CParam::CParam(const CParam& param)
{
 assign(param);
}

CParam::CParam(void)
{
 clear();
}

CParam::~CParam(void)
{
 clear();
}

void CParam::operator =(const CParam& param)
{
 assign(param);
}

void CParam::clear(void)
{
 m_Name.clear();
 m_Value.clear();
}

void CParam::assign(const CParam& param)
{
 m_Name = param.m_Name;
 m_Value = param.m_Value;
}

void CParam::show(std::ostream& out, const int level)
{
 CEntity::show(out,level);
 out<<"Param:{Name='"<<m_Name.toUtf8().data()
    <<"', Value='"<<m_Value.toUtf8().data()<<"'}"<<std::endl;
}

bool CParam::readFromXML(const QDomElement& root)
{
 QDomElement e = root;
 if (e.nodeName()!="param") e = e.nextSiblingElement("param");
 if (!e.isNull())
 {
  QString s = e.attribute("name");
  if (!s.isEmpty())
  {
   m_Name = s;
  }
  s = e.attribute("value");
  if (!s.isEmpty())
  {
   m_Value = s;
  }
  return true;
 }
 return false;
}

bool CParam::writeToXML(QDomDocument& host, QDomElement& root, const bool defaults)
{
 if (!host.isNull() && !root.isNull())
 {
  QDomElement e = host.createElement("param");
  e.setAttribute("name",m_Name);
  e.setAttribute("value",m_Value);
  root.appendChild(e);
  return true;
 }
 return false;
}

//------------------------------------------------------------------------------

CApproved::CApproved(const CApproved& error)
{
 assign(error);
}

CApproved::CApproved(void)
{
 clear();
}

CApproved::~CApproved(void)
{
 clear();
}

void CApproved::operator =(const CApproved& error)
{
 assign(error);
}

void CApproved::clear(void)
{
 m_Hash.clear();
}

void CApproved::assign(const CApproved& error)
{
 m_Hash = error.m_Hash;
}

void CApproved::show(std::ostream& out, const int level)
{
 CEntity::show(out,level);
 out<<"Approved:{Hash='"<<m_Hash.toUtf8().data()<<"'}"<<std::endl;
}

bool CApproved::readFromXML(const QDomElement& root)
{
 QDomElement e = root;
 if (e.nodeName()!="approved") e = e.nextSiblingElement("approved");
 if (!e.isNull())
 {
  QString s = e.attribute("hash");
  if (!s.isEmpty())
  {
   m_Hash = s;
  }
  return true;
 }
 return false;
}

bool CApproved::writeToXML(QDomDocument& host, QDomElement& root, const bool defaults)
{
 if (!host.isNull() && !root.isNull())
 {
  QDomElement e = host.createElement("approved");
  e.setAttribute("hash",m_Hash);
  root.appendChild(e);
  return true;
 }
 return false;
}

//------------------------------------------------------------------------------

CPass::CPass(const CPass& pass)
{
 assign(pass);
}

CPass::CPass(void)
{
 clear();
}

CPass::~CPass(void)
{
 clear();
}

void CPass::operator =(const CPass& pass)
{
 assign(pass);
}

void CPass::clear(void)
{
 for (int i = 0; i < m_Params.size(); i++)
 {
  if (0!=m_Params[i]) delete m_Params[i];
 }
 m_Params.clear();
 m_Name.clear();
 m_Refer.clear();
 m_Active = true;
}

void CPass::assign(const CPass& pass)
{
 clear();
 for (int i = 0; i < pass.m_Params.size(); i++)
 {
  CParam *p = new CParam(*pass.m_Params.at(i));
  m_Params.append(p);
 }
 m_Name = pass.m_Name;
 m_Refer = pass.m_Refer;
 m_Active = pass.m_Active;
}

void CPass::show(std::ostream& out, const int level)
{
 CEntity::show(out,level);
 out<<"Pass:{Name='"<<m_Name.toUtf8().data()<<"', Refer='"<<m_Refer.toUtf8().data()
    <<"', Active="<<CEntity::toString(m_Active).toUtf8().data()<<std::endl;
 for (int i = 0; i < m_Params.size(); i++)
 {
  m_Params[i]->show(out,level+1);
 }
 CEntity::show(out,level); out<<"}"<<std::endl;
}

bool CPass::readFromXML(const QDomElement& root)
{
 QDomElement e = root;
 if (e.nodeName()!="pass") e = e.nextSiblingElement("pass");
 if (!e.isNull())
 {
  QDomElement c = e.firstChildElement("param");
  while (!c.isNull())
  {
   CParam *p = new CParam();
   m_Params.append(p);
   p->readFromXML(c);
   c = c.nextSiblingElement("param");
  }
  QString s = e.attribute("name");
  if (!s.isEmpty())
  {
   m_Name = s;
  }
  s = e.attribute("refer");
  if (!s.isEmpty())
  {
   m_Refer = s;
  }
  s = e.attribute("active");
  if (!s.isEmpty())
  {
   if (s=="yes") { m_Active = true; }
   else if (s=="no") { m_Active = false; }
  }
  return true;
 }
 return false;
}

bool CPass::writeToXML(QDomDocument& host, QDomElement& root, const bool defaults)
{
 if (!host.isNull() && !root.isNull())
 {
  QDomElement e = host.createElement("pass");
  //QDomElement c = host.createElement("params");
  for (int i = 0; i < m_Params.size(); i++)
  {
   m_Params[i]->writeToXML(host,e,defaults);
  }
  e.setAttribute("name",m_Name);
  e.setAttribute("refer",m_Refer);
  if (defaults || !m_Active)
  {
   e.setAttribute("active",CEntity::toString(m_Active));
  }
  root.appendChild(e);
  //e.appendChild(c);
  return true;
 }
 return false;
}

//------------------------------------------------------------------------------

CClass::CClass(const CClass& value)
{
 assign(value);
}

CClass::CClass(void)
{
 clear();
}

CClass::~CClass(void)
{
 clear();
}

void CClass::operator =(const CClass& value)
{
 assign(value);
}

void CClass::clear(void)
{
 for (int i = 0; i < m_Clearances.size(); i++)
 {
  if (0!=m_Clearances[i]) delete m_Clearances[i];
 }
 m_Clearances.clear();
 m_Number = 0;
 m_Name.clear();
 m_Width = 0;
 m_Drill = 0;
}

void CClass::assign(const CClass& value)
{
 clear();
 for (int i = 0; i < value.m_Clearances.size(); i++)
 {
  CClearance *c = new CClearance(*value.m_Clearances.at(i));
  m_Clearances.append(c);
 }
 m_Number = value.m_Number;
 m_Name = value.m_Name;
 m_Width = value.m_Width;
 m_Drill = value.m_Drill;
}

void CClass::show(std::ostream& out, const int level)
{
 CEntity::show(out,level);
 out<<"Class:{Number="<<m_Number<<", Name='"<<m_Name.toUtf8().data()
    <<"', Width="<<m_Width<<", Drill="<<m_Drill<<std::endl;
 CEntity::show(out,level); out<<"\tClearances="<<std::endl;
 CEntity::show(out,level); out<<"\t{"<<std::endl;
 for (int i = 0; i < m_Clearances.size(); i++)
 {
  m_Clearances[i]->show(out,level+2);
 }
 CEntity::show(out,level); out<<"\t}"<<std::endl;
 CEntity::show(out,level); out<<"}"<<std::endl;
}

bool CClass::readFromXML(const QDomElement& root)
{
 QDomElement e = root;
 if (e.nodeName()!="class") e = e.nextSiblingElement("class");
 if (!e.isNull())
 {
  QDomElement c = e.firstChildElement("clearance");
  while (!c.isNull())
  {
   CClearance *l = new CClearance();
   m_Clearances.append(l);
   l->readFromXML(c);
   c = c.nextSiblingElement("clearance");
  }
  QString s = e.attribute("number");
  if (!s.isEmpty())
  {
   bool ok = false;
   int value = s.toInt(&ok);
   if (ok) { m_Number = value; }
  }
  s = e.attribute("name");
  if (!s.isEmpty())
  {
   m_Name = s;
  }
  s = e.attribute("width");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toDouble(&ok);
   if (ok) { m_Width = value; }
  }
  s = e.attribute("drill");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toDouble(&ok);
   if (ok) { m_Drill = value; }
  }
  return true;
 }
 return false;
}

bool CClass::writeToXML(QDomDocument& host, QDomElement& root, const bool defaults)
{
 if (!host.isNull() && !root.isNull())
 {
  QDomElement e = host.createElement("class");
  e.setAttribute("number",QString("%1").arg(m_Number));
  e.setAttribute("name",m_Name);
  if (defaults || 0.0!=m_Width)
  {
   e.setAttribute("width",QString("%1").arg(m_Width));
  }
  if (defaults || 0.0!=m_Drill)
  {
   e.setAttribute("drill",QString("%1").arg(m_Drill));
  }
  root.appendChild(e);
  for (int i = 0; i < m_Clearances.size(); i++)
  {
   //QDomElement c = host.createElement("clearance");
   m_Clearances[i]->writeToXML(host,e,defaults);
   //e.appendChild(c);
  }
  return true;
 }
 return false;
}

//------------------------------------------------------------------------------

CDesignRule::CDesignRule(const CDesignRule& designRule)
{
 assign(designRule);
}

CDesignRule::CDesignRule(void)
{
 clear();
}

CDesignRule::~CDesignRule(void)
{
 clear();
}

void CDesignRule::operator =(const CDesignRule& designRule)
{
 assign(designRule);
}

void CDesignRule::clear(void)
{
 for (int i = 0; i < m_Descriptions.size(); i++)
 {
  if (0!=m_Descriptions[i]) delete m_Descriptions[i];
 }
 for (int i = 0; i < m_Params.size(); i++)
 {
  if (0!=m_Params[i]) delete m_Params[i];
 }
 m_Descriptions.clear();
 m_Params.clear();
 m_Name.clear();
}

void CDesignRule::assign(const CDesignRule& designRule)
{
 clear();
 for (int i = 0; i < designRule.m_Descriptions.size(); i++)
 {
  CDescription *d = new CDescription(*designRule.m_Descriptions.at(i));
  m_Descriptions.append(d);
 }
 for (int i = 0; i < designRule.m_Params.size(); i++)
 {
  CParam *p = new CParam(*designRule.m_Params.at(i));
  m_Params.append(p);
 }
 m_Name = designRule.m_Name;
}

void CDesignRule::show(std::ostream& out, const int level)
{
 CEntity::show(out,level); out<<"DesignRule:"<<std::endl;
 CEntity::show(out,level); out<<"{"<<std::endl;
 CEntity::show(out,level); out<<"\tName='"<<m_Name.toUtf8().data()<<"'"<<std::endl;
 CEntity::show(out,level); out<<"\tParams="<<std::endl;
 CEntity::show(out,level); out<<"\t{"<<std::endl;
 for (int i = 0; i < m_Params.size(); i++)
 {
  m_Params[i]->show(out,level+2);
 }
 CEntity::show(out,level); out<<"\t}"<<std::endl;
 CEntity::show(out,level); out<<"\tDescriptions="<<std::endl;
 CEntity::show(out,level); out<<"\t{"<<std::endl;
 for (int i = 0; i < m_Descriptions.size(); i++)
 {
  m_Descriptions[i]->show(out,level+2);
 }
 CEntity::show(out,level); out<<"\t}"<<std::endl;
 CEntity::show(out,level); out<<"}"<<std::endl;
}

bool CDesignRule::readFromXML(const QDomElement& root)
{
 QDomElement e = root;
 if (e.nodeName()!="designrules") e = e.nextSiblingElement("designrules");
 if (!e.isNull())
 {
  QDomElement c = e.firstChildElement("description");
  while (!c.isNull())
  {
   CDescription *d = new CDescription();
   m_Descriptions.append(d);
   d->readFromXML(c);
   c = c.nextSiblingElement("description");
  }
  c = e.firstChildElement("param");
  while (!c.isNull())
  {
   CParam *p = new CParam();
   m_Params.append(p);
   p->readFromXML(c);
   c = c.nextSiblingElement("param");
  }
  QString s = e.attribute("name");
  if (!s.isEmpty())
  {
   m_Name = s;
  }
  return true;
 }
 return false;
}

bool CDesignRule::writeToXML(QDomDocument& host, QDomElement& root, const bool defaults)
{
 if (!host.isNull() && !root.isNull())
 {
  root.setAttribute("name",m_Name);
  for (int i = 0; i < m_Descriptions.size(); i++)
  {
   //QDomElement e = host.createElement("description");
   m_Descriptions[i]->writeToXML(host,root,defaults);
   //root.appendChild(e);
  }
  for (int i = 0; i < m_Params.size(); i++)
  {
   //QDomElement e = host.createElement("param");
   m_Params[i]->writeToXML(host,root,defaults);
   //root.appendChild(e);
  }
  return true;
 }
 return false;
}

//------------------------------------------------------------------------------

CVariantDef::CVariantDef(const CVariantDef& variantDef)
{
 assign(variantDef);
}

CVariantDef::CVariantDef(void)
{
 clear();
}

CVariantDef::~CVariantDef(void)
{
 clear();
}

void CVariantDef::operator =(const CVariantDef& variantDef)
{
 assign(variantDef);
}

void CVariantDef::clear(void)
{
 m_Name.clear();
 m_Current = false;
}

void CVariantDef::assign(const CVariantDef& variantDef)
{
 m_Name = variantDef.m_Name;
 m_Current = variantDef.m_Current;
}

void CVariantDef::show(std::ostream& out, const int level)
{
 CEntity::show(out,level);
 out<<"VariantDef:{Name='"<<m_Name.toUtf8().data()
    <<"', Current"<<CEntity::toString(m_Current).toUtf8().data()<<"}"<<std::endl;
}

bool CVariantDef::readFromXML(const QDomElement& root)
{
 QDomElement e = root;
 if (e.nodeName()!="designrule") e = e.nextSiblingElement("designrule");
 if (!e.isNull())
 {
  QString s = e.attribute("name");
  if (!s.isEmpty())
  {
   m_Name = s;
  }
  s = e.attribute("current");
  if (!s.isEmpty())
  {
   if (s=="yes") { m_Current = true; }
   else if (s=="no") { m_Current = false; }
  }
  return true;
 }
 return false;
}

bool CVariantDef::writeToXML(QDomDocument& host, QDomElement& root, const bool defaults)
{
 if (!host.isNull() && !root.isNull())
 {
  QDomElement e = host.createElement("variantdef");
  e.setAttribute("name",m_Name);
  if (defaults || m_Current)
  {
   e.setAttribute("current",CEntity::toString(m_Current));
  }
  root.appendChild(e);
  return true;
 }
 return false;
}

//------------------------------------------------------------------------------

CVariant::CVariant(const CVariant& variant)
{
 assign(variant);
}

CVariant::CVariant(void)
{
 clear();
}

CVariant::~CVariant(void)
{
 clear();
}

void CVariant::operator =(const CVariant& variant)
{
 assign(variant);
}

void CVariant::clear(void)
{
 m_Name.clear();
 m_Populate = true;
 m_Value.clear();
 m_Technology.clear();
}

void CVariant::assign(const CVariant& variant)
{
 m_Name = variant.m_Name;
 m_Populate = variant.m_Populate;
 m_Value = variant.m_Value;
 m_Technology = variant.m_Technology;
}

void CVariant::show(std::ostream& out, const int level)
{
 CEntity::show(out,level);
 out<<"Variant:{Name='"<<m_Name.toUtf8().data()
    <<"', Populate="<<CEntity::toString(m_Populate).toUtf8().data()
    <<", Value='"<<m_Value.toUtf8().data()
    <<"', Technology='"<<m_Technology.toUtf8().data()
    <<"'}"<<std::endl;
}

bool CVariant::readFromXML(const QDomElement& root)
{
 QDomElement e = root;
 if (e.nodeName()!="variant") e = e.nextSiblingElement("variant");
 if (!e.isNull())
 {
  QString s = e.attribute("name");
  if (!s.isEmpty())
  {
   m_Name = s;
  }
  s = e.attribute("populate");
  if (!s.isEmpty())
  {
   if (s=="yes") { m_Populate = true; }
   else if (s=="no") { m_Populate = false; }
  }
  s = e.attribute("value");
  if (!s.isEmpty())
  {
   m_Value = s;
  }
  s = e.attribute("technology");
  if (!s.isEmpty())
  {
   m_Technology = s;
  }
  return true;
 }
 return false;
}

bool CVariant::writeToXML(QDomDocument& host, QDomElement& root, const bool defaults)
{
 if (!host.isNull() && !root.isNull())
 {
  QDomElement e = host.createElement("variant");
  e.setAttribute("name",m_Name);
  if (defaults || !m_Populate)
  {
   e.setAttribute("populate",CEntity::toString(m_Populate));
  }
  e.setAttribute("value",m_Value);
  e.setAttribute("technology",m_Technology);
  root.appendChild(e);
  return true;
 }
 return false;
}

//------------------------------------------------------------------------------

CGate::CGate(const QString& name, const QString& symbol, const TCoord x,
 const TCoord y, const CGate::AddLevel addLevel, const int swapLevel)
{
 setName(name);
 setSymbol(symbol);
 setX(x);
 setY(y);
 setAddLevel(addLevel);
 setSwapLevel(swapLevel);
}

CGate::CGate(const CGate& gate)
{
 assign(gate);
}

CGate::CGate(void)
{
 clear();
}

CGate::~CGate(void)
{
 clear();
}

QString CGate::toString(const CGate::AddLevel value)
{
 switch (value)
 {
  case CGate::alMust: return "must";
  case CGate::alCan: return "can";
  case CGate::alNext: return "next";
  case CGate::alRequest: return "request";
  case CGate::alAlways: return "always";
 }
 return "";
}

void CGate::operator =(const CGate& gate)
{
 assign(gate);
}

void CGate::clear(void)
{
 m_Name.clear();
 m_Symbol.clear();
 m_X = 0.0;
 m_Y = 0.0;
 m_AddLevel = CGate::alNext;
 m_SwapLevel = 0;
}

void CGate::assign(const CGate& gate)
{
 m_Name = gate.m_Name;
 m_Symbol = gate.m_Symbol;
 m_X = gate.m_X;
 m_Y = gate.m_Y;
 m_AddLevel = gate.m_AddLevel;
 m_SwapLevel = gate.m_SwapLevel;
}

void CGate::scale(const double factor)
{
 m_X *= factor;
 m_Y *= factor;
}

void CGate::show(std::ostream& out, const int level)
{
 CEntity::show(out,level);
 out<<"Gate:{Name='"<<m_Name.toUtf8().data()<<"', Symbol='"<<m_Symbol.toUtf8().data()
    <<"', X="<<m_X<<", Y="<<m_Y<<", AddLevel="<<toString(m_AddLevel).toUtf8().data()
    <<", SwapLevel="<<m_SwapLevel
    <<"}"<<std::endl;
}

bool CGate::readFromXML(const QDomElement& root)
{
 QDomElement e = root;
 if (e.nodeName()!="gate") e = e.nextSiblingElement("gate");
 if (!e.isNull())
 {
  QString s = e.attribute("name");
  if (!s.isEmpty())
  {
   m_Name = s;
  }
  s = e.attribute("symbol");
  if (!s.isEmpty())
  {
   m_Symbol = s;
  }
  s = e.attribute("x");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toDouble(&ok);
   if (ok) { m_X = value; }
  }
  s = e.attribute("y");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toDouble(&ok);
   if (ok) { m_Y = value; }
  }
  s = e.attribute("addlevel");
  if (!s.isEmpty())
  {
   if (s=="must") { m_AddLevel = CGate::alMust; }
   else if (s=="can") { m_AddLevel = CGate::alCan; }
   else if (s=="next") { m_AddLevel = CGate::alNext; }
   else if (s=="request") { m_AddLevel = CGate::alRequest; }
   else if (s=="always") { m_AddLevel = CGate::alAlways; }
  }
  s = e.attribute("swaplevel");
  if (!s.isEmpty())
  {
   bool ok = false;
   int value = s.toInt(&ok);
   if (ok) { m_SwapLevel = value; }
  }
  return true;
 }
 return false;
}

bool CGate::writeToXML(QDomDocument& host, QDomElement& root, const bool defaults)
{
 if (!host.isNull() && !root.isNull())
 {
  QDomElement e = host.createElement("gate");
  e.setAttribute("name",m_Name);
  e.setAttribute("symbol",m_Symbol);
  e.setAttribute("x",QString("%1").arg(m_X));
  e.setAttribute("y",QString("%1").arg(m_Y));
  if (defaults || CGate::alNext!=m_AddLevel)
  {
   e.setAttribute("addlevel",toString(m_AddLevel));
  }
  if (defaults || 0!=m_SwapLevel)
  {
   e.setAttribute("swaplevel",QString("%1").arg(m_SwapLevel));
  }
  root.appendChild(e);
  return true;
 }
 return false;
}

//------------------------------------------------------------------------------

CWire::CWire(const TCoord x1, const TCoord y1, const TCoord x2, const TCoord y2,
 const TDimension width, const double curve, const TLayer layer,
 const CWire::Style style, const CWire::Cap cap, const QString& extent)
{
 m_X1 = x1; m_Y1 = y1; m_X2 = x2; m_Y2 = y2; m_Width = width, m_Curve = curve;
 m_Layer = layer; m_Style = style; m_Cap = cap; m_Extent = extent;
}

CWire::CWire(const CWire& wire)
{
 assign(wire);
}

CWire::CWire(void)
{
 clear();
}

CWire::~CWire(void)
{
 clear();
}

QString CWire::toString(const CWire::Style value)
{
 switch (value)
 {
  case CWire::wsContinuous: return "continuous";
  case CWire::wsLongDash: return "longdash";
  case CWire::wsShortDash: return "shortdash";
  case CWire::wsDashDot: return "dashdot";
 }
 return "";
}

QString CWire::toString(const CWire::Cap value)
{
 switch (value)
 {
  case CWire::wcFlat: return "flat";
  case CWire::wcRound: return "round";
 }
 return "";
}

void CWire::operator =(const CWire& wire)
{
 assign(wire);
}

void CWire::clear(void)
{
 m_X1 = 0.0;
 m_Y1 = 0.0;
 m_X2 = 1.0;
 m_Y2 = 1.0;
 m_Width = 0.1;
 m_Layer = 1;
 m_Extent.clear();
 m_Style = CWire::wsContinuous;
 m_Curve = 0.0;
 m_Cap = CWire::wcRound;
}

void CWire::assign(const CWire& wire)
{
 m_X1 = wire.m_X1;
 m_Y1 = wire.m_Y1;
 m_X2 = wire.m_X2;
 m_Y2 = wire.m_Y2;
 m_Width = wire.m_Width;
 m_Layer = wire.m_Layer;
 m_Extent = wire.m_Extent;
 m_Style = wire.m_Style;
 m_Curve = wire.m_Curve;
 m_Cap = wire.m_Cap;
}

double CWire::chord(void) const
{
 return hypot(m_X2-m_X1,m_Y2-m_Y1);
}

double CWire::length(void) const
{
 if (0.0!=m_Curve)
 {
  return radius()*degtorad(m_Curve);
 }
 else
 {
  return chord();
 }
}

double CWire::radius(void) const
{
 if (0.0!=m_Curve)
 {
  return chord()/(2.0*sin(0.5*degtorad(m_Curve)));
 }
 return 0.0;
}

void CWire::scale(const double factor)
{
 m_X1 *= factor;
 m_Y1 *= factor;
 m_X2 *= factor;
 m_Y2 *= factor;
 m_Width *= factor;
 // m_Curve *= factor;
}

void CWire::show(std::ostream& out, const int level)
{
 CEntity::show(out,level);
 out<<"Wire:{X1="<<m_X1<<", Y1="<<m_Y1<<", X2="<<m_X2<<", Y2="<<m_Y2
    <<", Width="<<m_Width<<", Layer="<<m_Layer<<", Extent='"
    <<m_Extent.toUtf8().data()<<"', Style="<<toString(m_Style).toUtf8().data()
    <<", Curve="<<m_Curve<<", Cap="<<toString(m_Cap).toUtf8().data()
    <<"}"<<std::endl;
}

bool CWire::readFromXML(const QDomElement& root)
{
 QDomElement e = root;
 if (e.nodeName()!="wire") e = e.nextSiblingElement("wire");
 if (!e.isNull())
 {
  QString s = e.attribute("x1");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toDouble(&ok);
   if (ok) { m_X1 = value; }
  }
  s = e.attribute("y1");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toDouble(&ok);
   if (ok) { m_Y1 = value; }
  }
  s = e.attribute("x2");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toDouble(&ok);
   if (ok) { m_X2 = value; }
  }
  s = e.attribute("y2");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toDouble(&ok);
   if (ok) { m_Y2 = value; }
  }
  s = e.attribute("width");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toDouble(&ok);
   if (ok) { m_Width = value; }
  }
  s = e.attribute("layer");
  if (!s.isEmpty())
  {
   bool ok = false;
   int value = s.toInt(&ok);
   if (ok) { m_Layer = value; }
  }
  s = e.attribute("style");
  if (!s.isEmpty())
  {
   if (s=="continuous") { m_Style = CWire::wsContinuous; }
   else if (s=="longdash") { m_Style = CWire::wsLongDash; }
   else if (s=="shortdash") { m_Style = CWire::wsShortDash; }
   else if (s=="dashdot") { m_Style = CWire::wsDashDot; }
  }
  s = e.attribute("curve");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toDouble(&ok);
   if (ok) { m_Curve = value; }
  }
  s = e.attribute("cap");
  if (!s.isEmpty())
  {
   if (s=="flat") { m_Cap = CWire::wcFlat; }
   else if (s=="round") { m_Cap = CWire::wcRound; }
  }
  return true;
 }
 return false;
}

bool CWire::writeToXML(QDomDocument& host, QDomElement& root, const bool defaults)
{
 if (!host.isNull() && !root.isNull())
 {
  QDomElement e = host.createElement("wire");
  e.setAttribute("x1",QString("%1").arg(m_X1));
  e.setAttribute("y1",QString("%1").arg(m_Y1));
  e.setAttribute("x2",QString("%1").arg(m_X2));
  e.setAttribute("y2",QString("%1").arg(m_Y2));
  e.setAttribute("width",QString("%1").arg(m_Width));
  e.setAttribute("layer",QString("%1").arg(m_Layer));
  if (!m_Extent.isEmpty())
  {
   e.setAttribute("extent",m_Extent);
  }
  if (defaults || CWire::wsContinuous!=m_Style)
  {
   e.setAttribute("style",toString(m_Style));
  }
  if (defaults || 0.0!=m_Curve)
  {
   e.setAttribute("curve",QString("%1").arg(m_Curve));
  }
  if (defaults || CWire::wcRound!=m_Cap)
  {
   e.setAttribute("cap",toString(m_Cap));
  }
  root.appendChild(e);
  return true;
 }
 return false;
}

//------------------------------------------------------------------------------

CDimension::CDimension(const CDimension& dimension)
{
 assign(dimension);
}

CDimension::CDimension(void)
{
 clear();
}

CDimension::~CDimension(void)
{
 clear();
}

QString CDimension::toString(const CDimension::Type value)
{
 switch(value)
 {
  case CDimension::dtParallel: return "parallel";
  case CDimension::dtHorizontal: return "horizontal";
  case CDimension::dtVertical: return "vertical";
  case CDimension::dtRadius: return "radius";
  case CDimension::dtDiameter: return "diameter";
  case CDimension::dtLeader: return "leader";
 }
 return "";
}

void CDimension::operator =(const CDimension& dimension)
{
 assign(dimension);
}

void CDimension::clear(void)
{
 m_X1 = 0.0;
 m_Y1 = 0.0;
 m_X2 = 0.0;
 m_Y2 = 0.0;
 m_X3 = 0.0;
 m_Y3 = 0.0;
 m_Layer = 1;
 m_DType = CDimension::dtParallel;
 m_Width = 0.1;
 m_ExtWidth = 0.0;
 m_ExtLength = 0.0;
 m_ExtOffset = 0.0;
 m_TextSize = 1.0;
 m_TextRatio = 8;
 m_GridUnit = CGrid::guMM;
 m_Precision = 2;
 m_Visible = false;
}

void CDimension::assign(const CDimension& dimension)
{
 m_X1 = dimension.m_X1;
 m_Y1 = dimension.m_Y1;
 m_X2 = dimension.m_X2;
 m_Y2 = dimension.m_Y2;
 m_X3 = dimension.m_X3;
 m_Y3 = dimension.m_Y3;
 m_Layer = dimension.m_Layer;
 m_DType = dimension.m_DType;
 m_Width = dimension.m_Width;
 m_ExtWidth = dimension.m_ExtWidth;
 m_ExtLength = dimension.m_ExtLength;
 m_ExtOffset = dimension.m_ExtOffset;
 m_TextSize = dimension.m_TextSize;
 m_TextRatio = dimension.m_TextRatio;
 m_GridUnit = dimension.m_GridUnit;
 m_Precision = dimension.m_Precision;
 m_Visible = dimension.m_Visible;
}

void CDimension::scale(const double factor)
{
 m_X1 *= factor;
 m_Y1 *= factor;
 m_X2 *= factor;
 m_Y2 *= factor;
 m_X3 *= factor;
 m_Y3 *= factor;
 m_Width *= factor;
 m_ExtWidth *= factor;
 m_ExtLength *= factor;
 m_ExtOffset *= factor;
 m_TextSize *= factor;
}

void CDimension::show(std::ostream& out, const int level)
{
 CEntity::show(out,level);
 out<<"Dimension:{X1="<<m_X1<<", Y1="<<m_Y1<<", X2="<<m_X2<<", Y2="<<m_Y2
    <<", X3="<<m_X3<<", Y3="<<m_Y3<<", Layer="<<m_Layer<<", Type="
    <<toString(m_DType).toUtf8().data()<<", ";
 CEntity::show(out,level);
 out<<"\tWidth="<<m_Width<<", ExtWidth="<<m_ExtWidth<<", ExtLength="<<m_ExtLength
    <<", ExtOffset="<<m_ExtOffset;
 CEntity::show(out,level);
 out<<"\tTextSize="<<m_TextSize<<", TextRatio="<<m_TextRatio<<", GridUnit="
    <<CGrid::toString(m_GridUnit).toUtf8().data()<<", Precision="<<m_Precision
    <<", Visible="<<CEntity::toString(m_Visible).toUtf8().data()
    <<"}"<<std::endl;
}

bool CDimension::readFromXML(const QDomElement& root)
{
 QDomElement e = root;
 if (e.nodeName()!="dimension") e = e.nextSiblingElement("dimension");
 if (!e.isNull())
 {
  QString s = e.attribute("x1");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toDouble(&ok);
   if (ok) { m_X1 = value; }
  }
  s = e.attribute("y1");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toDouble(&ok);
   if (ok) { m_Y1 = value; }
  }
  s = e.attribute("x2");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toDouble(&ok);
   if (ok) { m_X2 = value; }
  }
  s = e.attribute("y2");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toDouble(&ok);
   if (ok) { m_Y2 = value; }
  }
  s = e.attribute("x3");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toDouble(&ok);
   if (ok) { m_X3 = value; }
  }
  s = e.attribute("y3");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toDouble(&ok);
   if (ok) { m_Y3 = value; }
  }
  s = e.attribute("layer");
  if (!s.isEmpty())
  {
   bool ok = false;
   int value = s.toInt(&ok);
   if (ok) { m_Layer = value; }
  }
  s = e.attribute("dtype");
  if (!s.isEmpty())
  {
   if (s=="parallel") { m_DType = CDimension::dtParallel; }
   else if (s=="horizontal") { m_DType = CDimension::dtHorizontal; }
   else if (s=="vertical") { m_DType = CDimension::dtVertical; }
   else if (s=="radius") { m_DType = CDimension::dtRadius; }
   else if (s=="diameter") { m_DType = CDimension::dtDiameter; }
   else if (s=="leader") { m_DType = CDimension::dtLeader; }
  }
  return true;
  s = e.attribute("width");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toDouble(&ok);
   if (ok) { m_Width = value; }
  }
  s = e.attribute("extwidth");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toDouble(&ok);
   if (ok) { m_ExtWidth = value; }
  }
  s = e.attribute("extlength");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toDouble(&ok);
   if (ok) { m_ExtLength = value; }
  }
  s = e.attribute("extoffset");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toDouble(&ok);
   if (ok) { m_ExtOffset = value; }
  }
  s = e.attribute("textsize");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toDouble(&ok);
   if (ok) { m_TextSize = value; }
  }
  s = e.attribute("textratio");
  if (!s.isEmpty())
  {
   bool ok = false;
   int value = s.toInt(&ok);
   if (ok) { m_TextRatio = value; }
  }
  s = e.attribute("unit");
  if (!s.isEmpty())
  {
   CGrid::Unit value;
   bool ok = CGrid::stringToUnit(s,value);
   if (ok) { m_GridUnit = value; }
  }
  s = e.attribute("precision");
  if (!s.isEmpty())
  {
   bool ok = false;
   int value = s.toInt(&ok);
   if (ok) { m_Precision = value; }
  }
  s = e.attribute("visible");
  if (!s.isEmpty())
  {
   if (s=="yes") { m_Visible = true; }
   else if (s=="no") { m_Visible = false; }
  }
 }
 return false;
}

bool CDimension::writeToXML(QDomDocument& host, QDomElement& root, const bool defaults)
{
 if (!host.isNull() && !root.isNull())
 {
  QDomElement e = host.createElement("dimension");
  e.setAttribute("x1",QString("%1").arg(m_X1));
  e.setAttribute("y1",QString("%1").arg(m_Y1));
  e.setAttribute("x2",QString("%1").arg(m_X2));
  e.setAttribute("y2",QString("%1").arg(m_Y2));
  e.setAttribute("x3",QString("%1").arg(m_X3));
  e.setAttribute("y3",QString("%1").arg(m_Y3));
  e.setAttribute("layer",QString("%1").arg(m_Layer));
  if (defaults || CDimension::dtParallel!=m_DType)
  {
   e.setAttribute("dtype",toString(m_DType));
  }
  e.setAttribute("width",QString("%1").arg(m_Width));
  if (defaults || DEFAULT_EXT_WIDTH!=m_ExtWidth)
  {
   e.setAttribute("extwidth",QString("%1").arg(m_ExtWidth));
  }
  if (defaults || DEFAULT_EXT_LENGTH!=m_ExtLength)
  {
   e.setAttribute("extlength",QString("%1").arg(m_ExtLength));
  }
  if (defaults || DEFAULT_EXT_OFFSET!=m_ExtOffset)
  {
   e.setAttribute("extoffset",QString("%1").arg(m_ExtOffset));
  }
  e.setAttribute("textsize",QString("%1").arg(m_TextSize));
  if (defaults || DEFAULT_GRID_UNIT!=m_GridUnit)
  {
   e.setAttribute("unit",CGrid::toString(m_GridUnit));
  }
  if (defaults || DEFAULT_PRECISION!=m_Precision)
  {
   e.setAttribute("precision",QString("%1").arg(m_Precision));
  }
  if (defaults || m_Visible)
  {
   e.setAttribute("visible",CEntity::toString(m_Visible));
  }
  root.appendChild(e);
  return true;
 }
 return false;
}

//------------------------------------------------------------------------------

CText::CText(const QString& text, const TCoord x, const TCoord y,
 const TDimension size, const TLayer layer, const CText::Font font,
 const int ratio, const double rotation, const bool reflection,
 const bool spin, const CText::Align align, const int distance)
{
 setText(text);
 setX(x);
 setY(y);
 setSize(size);
 setLayer(layer);
 setFont(font);
 setRatio(ratio);
 setRotation(rotation);
 setReflection(reflection);
 setSpin(spin);
 setAlign(align);
 setDistance(distance);
}

CText::CText(const CText& text)
{
 assign(text);
}

CText::CText(void)
{
 clear();
}

CText::~CText(void)
{
 clear();
}

QString CText::toString(const CText::Font value)
{
 switch (value)
 {
  case CText::tfVector: return "vector";
  case CText::tfProportional: return "proportional";
  case CText::tfFixed: return "fixed";
 }
 return "";
}

QString CText::toString(const CText::Align value)
{
 switch (value)
 {
  case CText::taBottomLeft: return "bottom-left";
  case CText::taBottomCenter: return "bottom-center";
  case CText::taBottomRight: return "bottom-right";
  case CText::taCenterLeft: return "center-left";
  case CText::taCenter: return "center";
  case CText::taCenterRight: return "center-right";
  case CText::taTopLeft: return "top-left";
  case CText::taTopCenter: return "top-center";
  case CText::taTopRight: return "top-right";
 }
 return "";
}

void CText::operator =(const CText& text)
{
 assign(text);
}

void CText::clear(void)
{
 m_Text.clear();
 m_X = 0.0;
 m_Y = 0.0;
 m_Size = 1.0;
 m_Layer = 1;
 m_Font = CText::DEFAULT_FONT;
 m_Ratio = CText::DEFAULT_RATIO;
 m_Rotation = CText::DEFAULT_ROTATION;
 m_Reflection = false;
 m_Spin = false;
 m_Align = CText::DEFAULT_ALIGN;
 m_Distance = CText::DEFAULT_DISTANCE;
}

void CText::assign(const CText& text)
{
 m_Text = text.m_Text;
 m_X = text.m_X;
 m_Y = text.m_Y;
 m_Size = text.m_Size;
 m_Layer = text.m_Layer;
 m_Font = text.m_Font;
 m_Ratio = text.m_Ratio;
 m_Rotation = text.m_Rotation;
 m_Reflection = text.m_Reflection;
 m_Spin = text.m_Spin;
 m_Align = text.m_Align;
 m_Distance = text.m_Distance;
}

void CText::scale(const double factor)
{
 m_X *= factor;
 m_Y *= factor;
 m_Size *= factor;
}

void CText::show(std::ostream& out, const int level)
{
 CEntity::show(out,level);
 out<<"Text:{X="<<m_X<<", Y="<<m_Y<<", Size="<<m_Size<<", Layer="<<m_Layer
    <<", Font="<<toString(m_Font).toUtf8().data()<<", Ratio="<<m_Ratio
    <<", Rotation="<<m_Rotation<<", Align="<<toString(m_Align).toUtf8().data()
    <<", Spin="<<m_Spin<<", Reflection="<<m_Reflection
    <<", Distance="<<m_Distance<<","<<std::endl;
 CEntity::show(out,level); out<<"\t{"<<m_Text.toUtf8().data()<<"}"<<std::endl;
 CEntity::show(out,level); out<<"}"<<std::endl;
}

bool CText::readFromXML(const QDomElement& root)
{
 QDomElement e = root;
 if (e.nodeName()!="text") e = e.nextSiblingElement("text");
 if (!e.isNull())
 {
  QString s = e.text();
  if (!s.isEmpty())
  {
   m_Text = s;
  }
  s = e.attribute("x");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toDouble(&ok);
   if (ok) { m_X = value; }
  }
  s = e.attribute("y");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toDouble(&ok);
   if (ok) { m_Y = value; }
  }
  s = e.attribute("size");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toDouble(&ok);
   if (ok) { m_Size = value; }
  }
  s = e.attribute("layer");
  if (!s.isEmpty())
  {
   bool ok = false;
   int value = s.toInt(&ok);
   if (ok) { m_Layer = value; }
  }
  s = e.attribute("font");
  if (!s.isEmpty())
  {
   if (s=="vector") { m_Font = CText::tfVector; }
   else if (s=="proportional") { m_Font = CText::tfProportional; }
   else if (s=="fixed") { m_Font = CText::tfFixed; }
  }
  s = e.attribute("ratio");
  if (!s.isEmpty())
  {
   bool ok = false;
   int value = s.toInt(&ok);
   if (ok) { m_Ratio = value; }
  }
  s = e.attribute("rot");
  if (!s.isEmpty())
  {
   decodeTransformation(s,m_Rotation,m_Reflection,m_Spin);
   /*
   if (s.at(0)=='R')
   {
    bool ok = false;
    QString ss = s.right(s.length()-1);
    double value = ss.toDouble(&ok);
    if (ok) { m_Rotation = value; }
   }
   */
  }
  s = e.attribute("align");
  if (!s.isEmpty())
  {
   if (s=="bottom-left") { m_Align = CText::taBottomLeft; }
   else if (s=="bottom-center") { m_Align = CText::taBottomCenter; }
   else if (s=="bottom-right") { m_Align = CText::taBottomRight; }
   else if (s=="center-left") { m_Align = CText::taCenterLeft; }
   else if (s=="center") { m_Align = CText::taCenter; }
   else if (s=="center-right") { m_Align = CText::taCenterRight; }
   else if (s=="top-left") { m_Align = CText::taTopLeft; }
   else if (s=="top-center") { m_Align = CText::taTopCenter; }
   else if (s=="top-right") { m_Align = CText::taTopRight; }
  }
  s = e.attribute("distance");
  if (!s.isEmpty())
  {
   bool ok = false;
   int value = s.toInt(&ok);
   if (ok) { m_Distance = value; }
  }
  return true;
 }
 return false;
}

bool CText::writeToXML(QDomDocument& host, QDomElement& root, const bool defaults)
{
 if (!host.isNull() && !root.isNull())
 {
  QDomElement e = host.createElement("text");
  e.setAttribute("x",QString("%1").arg(m_X));
  e.setAttribute("y",QString("%1").arg(m_Y));
  e.setAttribute("size",QString("%1").arg(m_Size));
  e.setAttribute("layer",QString("%1").arg(m_Layer));
  if (defaults || CText::DEFAULT_FONT!=m_Font)
  {
   e.setAttribute("font",toString(m_Font));
  }
  if (defaults || CText::DEFAULT_RATIO!=m_Ratio)
  {
   e.setAttribute("ratio",QString("%1").arg(m_Ratio));
  }
  if (defaults || CText::DEFAULT_ROTATION!=m_Rotation || m_Reflection || m_Spin)
  {
   //e.setAttribute("rot",QString("R%1").arg(m_Rotation));
   e.setAttribute("rot",encodeTransformation(m_Rotation,m_Reflection,m_Spin));
  }
  if (defaults || CText::taBottomLeft!=m_Align)
  {
   e.setAttribute("align",toString(m_Align));
  }
  if (defaults || CText::DEFAULT_DISTANCE!=m_Distance)
  {
   e.setAttribute("distance",QString("%1").arg(m_Distance));
  }
  QDomNode n = host.createTextNode(m_Text);
  e.appendChild(n);
  root.appendChild(e);
  return true;
 }
 return false;
}

//------------------------------------------------------------------------------

CCircle::CCircle(const CCircle& circle)
{
 assign(circle);
}

CCircle::CCircle(void)
{
 clear();
}

CCircle::~CCircle(void)
{
 clear();
}

void CCircle::operator =(const CCircle& circle)
{
 assign(circle);
}

void CCircle::clear(void)
{
 m_X = 0.0;
 m_Y = 0.0;
 m_Radius = 1.0;
 m_Width = 0.1;
 m_Layer = 1;
}

void CCircle::assign(const CCircle& circle)
{
 m_X = circle.m_X;
 m_Y = circle.m_Y;
 m_Radius = circle.m_Radius;
 m_Width = circle.m_Width;
 m_Layer = circle.m_Layer;
}

void CCircle::scale(const double factor)
{
 m_X *= factor;
 m_Y *= factor;
 m_Radius *= factor;
 m_Width *= factor;
}

void CCircle::show(std::ostream& out, const int level)
{
 CEntity::show(out,level);
 out<<"Circle:{X="<<m_X<<", Y="<<m_Y<<", Radius="<<m_Radius<<", Width="<<m_Width
    <<", Layer="<<m_Layer<<"}"<<std::endl;
}

bool CCircle::readFromXML(const QDomElement& root)
{
 QDomElement e = root;
 if (e.nodeName()!="circle") e = e.nextSiblingElement("circle");
 if (!e.isNull())
 {
  QString s = e.attribute("x");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toDouble(&ok);
   if (ok) { m_X = value; }
  }
  s = e.attribute("y");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toDouble(&ok);
   if (ok) { m_Y = value; }
  }
  s = e.attribute("radius");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toDouble(&ok);
   if (ok) { m_Radius = value; }
  }
  s = e.attribute("width");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toDouble(&ok);
   if (ok) { m_Width = value; }
  }
  s = e.attribute("layer");
  if (!s.isEmpty())
  {
   bool ok = false;
   int value = s.toInt(&ok);
   if (ok) { m_Layer = value; }
  }
  return true;
 }
 return false;
}

bool CCircle::writeToXML(QDomDocument& host, QDomElement& root, const bool defaults)
{
 if (!host.isNull() && !root.isNull())
 {
  QDomElement e = host.createElement("circle");
  e.setAttribute("x",QString("%1").arg(m_X));
  e.setAttribute("y",QString("%1").arg(m_Y));
  e.setAttribute("radius",QString("%1").arg(m_Radius));
  e.setAttribute("width",QString("%1").arg(m_Width));
  e.setAttribute("layer",QString("%1").arg(m_Layer));
  root.appendChild(e);
  return true;
 }
 return false;
}

//------------------------------------------------------------------------------

CRectangle::CRectangle(const CRectangle& rectangle)
{
}

CRectangle::CRectangle(void)
{
 clear();
}

CRectangle::~CRectangle(void)
{
 clear();
}

void CRectangle::operator =(const CRectangle& rectangle)
{
}

void CRectangle::clear(void)
{
 m_X1 = 0.0;
 m_Y1 = 0.0;
 m_X2 = 1.0;
 m_Y2 = 1.0;
 m_Layer = 1;
 m_Rotation = CRectangle::DEFAULT_ROTATION;
}

void CRectangle::assign(const CRectangle& rectangle)
{
 m_X1 = rectangle.m_X1;
 m_Y1 = rectangle.m_Y1;
 m_X2 = rectangle.m_X2;
 m_Y2 = rectangle.m_Y2;
 m_Layer = rectangle.m_Layer;
 m_Rotation = rectangle.m_Rotation;
}

void CRectangle::scale(const double factor)
{
 m_X1 *= factor;
 m_Y1 *= factor;
 m_X2 *= factor;
 m_Y2 *= factor;
}

void CRectangle::show(std::ostream& out, const int level)
{
 CEntity::show(out,level);
 out<<"Rectangle:{X1="<<m_X1<<", Y1="<<m_Y1<<", X2="<<m_X2<<", Y2="<<m_Y2
    <<", Layer="<<m_Layer<<", Rotation="<<m_Rotation<<"}"<<std::endl;
}

bool CRectangle::readFromXML(const QDomElement& root)
{
 QDomElement e = root;
 if (e.nodeName()!="rectangle") e = e.nextSiblingElement("rectangle");
 if (!e.isNull())
 {
  QString s = e.attribute("x1");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toDouble(&ok);
   if (ok) { m_X1 = value; }
  }
  s = e.attribute("y1");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toDouble(&ok);
   if (ok) { m_Y1 = value; }
  }
  s = e.attribute("x2");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toDouble(&ok);
   if (ok) { m_X2 = value; }
  }
  s = e.attribute("y2");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toDouble(&ok);
   if (ok) { m_Y2 = value; }
  }
  s = e.attribute("layer");
  if (!s.isEmpty())
  {
   bool ok = false;
   int value = s.toInt(&ok);
   if (ok) { m_Layer = value; }
  }
  s = e.attribute("rot");
  if (!s.isEmpty())
  {
   bool reflection, spin;
   decodeTransformation(s,m_Rotation,reflection,spin);
   /*
   if (s.at(0)=='R')
   {
    bool ok = false;
    QString ss = s.right(s.length()-1);
    double value = ss.toDouble(&ok);
    if (ok) { m_Rotation = value; }
   }
   */
  }
  return true;
 }
 return false;
}

bool CRectangle::writeToXML(QDomDocument& host, QDomElement& root, const bool defaults)
{
 if (!host.isNull() && !root.isNull())
 {
  QDomElement e = host.createElement("rectangle");
  e.setAttribute("x1",QString("%1").arg(m_X1));
  e.setAttribute("y1",QString("%1").arg(m_Y1));
  e.setAttribute("x2",QString("%1").arg(m_X2));
  e.setAttribute("y2",QString("%1").arg(m_Y2));
  e.setAttribute("layer",QString("%1").arg(m_Layer));
  if (defaults || DEFAULT_ROTATION!=m_Rotation)
  {
   //e.setAttribute("rot",QString("R%1").arg(m_Rotation));
   e.setAttribute("rot",encodeTransformation(m_Rotation,false,false));
  }
  root.appendChild(e);
  return true;
 }
 return false;
}

//------------------------------------------------------------------------------

CFrame::CFrame(const CFrame& frame)
{
 assign(frame);
}

CFrame::CFrame(void)
{
 clear();
}

CFrame::~CFrame(void)
{
 clear();
}

void CFrame::operator =(const CFrame& frame)
{
 assign(frame);
}

void CFrame::clear(void)
{
 m_X1 = 0.0;
 m_Y1 = 0.0;
 m_X2 = 1.0;
 m_Y2 = 1.0;
 m_Columns = 8;
 m_Rows = 5;
 m_Layer = 1;
 m_BorderLeft = true;
 m_BorderTop = true;
 m_BorderRight = true;
 m_BorderBottom = true;
}

void CFrame::assign(const CFrame& frame)
{
 m_X1 = frame.m_X1;
 m_Y1 = frame.m_Y1;
 m_X2 = frame.m_X2;
 m_Y2 = frame.m_Y2;
 m_Columns = frame.m_Columns;
 m_Rows = frame.m_Rows;
 m_Layer = frame.m_Layer;
 m_BorderLeft = frame.m_BorderLeft;
 m_BorderTop = frame.m_BorderTop;
 m_BorderRight = frame.m_BorderRight;
 m_BorderBottom = frame.m_BorderBottom;
}

void CFrame::scale(const double factor)
{
 m_X1 *= factor;
 m_Y1 *= factor;
 m_X2 *= factor;
 m_Y2 *= factor;
}

void CFrame::show(std::ostream& out, const int level)
{
 CEntity::show(out,level);
 out<<"Frame:{X1="<<m_X1<<", Y1="<<m_Y1<<", X2="<<m_X2<<", Y2="<<m_Y2
    <<", Columns="<<m_Columns<<", Rows="<<m_Rows<<", Layer="<<m_Layer
    <<", BorderLeft="<<CEntity::toString(m_BorderLeft).toUtf8().data()
    <<", BorderTop="<<CEntity::toString(m_BorderTop).toUtf8().data()
    <<", BorderRight="<<CEntity::toString(m_BorderRight).toUtf8().data()
    <<", BorderBottom="<<CEntity::toString(m_BorderBottom).toUtf8().data()
    <<"}"<<std::endl;
}

bool CFrame::readFromXML(const QDomElement& root)
{
 QDomElement e = root;
 if (e.nodeName()!="frame") e = e.nextSiblingElement("frame");
 if (!e.isNull())
 {
  QString s = e.attribute("x1");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toDouble(&ok);
   if (ok) { m_X1 = value; }
  }
  s = e.attribute("y1");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toDouble(&ok);
   if (ok) { m_Y1 = value; }
  }
  s = e.attribute("x2");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toDouble(&ok);
   if (ok) { m_X2 = value; }
  }
  s = e.attribute("y2");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toDouble(&ok);
   if (ok) { m_Y2 = value; }
  }
  s = e.attribute("columns");
  if (!s.isEmpty())
  {
   bool ok = false;
   int value = s.toInt(&ok);
   if (ok) { m_Columns = value; }
  }
  s = e.attribute("rows");
  if (!s.isEmpty())
  {
   bool ok = false;
   int value = s.toInt(&ok);
   if (ok) { m_Rows = value; }
  }
  s = e.attribute("layer");
  if (!s.isEmpty())
  {
   bool ok = false;
   int value = s.toInt(&ok);
   if (ok) { m_Layer = value; }
  }
  s = e.attribute("border-left");
  if (!s.isEmpty())
  {
   if (s=="yes") { m_BorderLeft = true; }
   else if (s=="no") { m_BorderLeft = false; }
  }
  s = e.attribute("border-top");
  if (!s.isEmpty())
  {
   if (s=="yes") { m_BorderTop = true; }
   else if (s=="no") { m_BorderTop = false; }
  }
  s = e.attribute("border-right");
  if (!s.isEmpty())
  {
   if (s=="yes") { m_BorderRight = true; }
   else if (s=="no") { m_BorderRight = false; }
  }
  s = e.attribute("border-bottom");
  if (!s.isEmpty())
  {
   if (s=="yes") { m_BorderBottom = true; }
   else if (s=="no") { m_BorderBottom = false; }
  }
  return true;
 }
 return false;
}

bool CFrame::writeToXML(QDomDocument& host, QDomElement& root, const bool defaults)
{
 if (!host.isNull() && !root.isNull())
 {
  QDomElement e = host.createElement("frame");
  e.setAttribute("x1",QString("%1").arg(m_X1));
  e.setAttribute("y1",QString("%1").arg(m_Y1));
  e.setAttribute("x2",QString("%1").arg(m_X2));
  e.setAttribute("y2",QString("%1").arg(m_Y2));
  e.setAttribute("columns",QString("%1").arg(m_Columns));
  e.setAttribute("rows",QString("%1").arg(m_Rows));
  e.setAttribute("layer",QString("%1").arg(m_Layer));
  if (defaults || !m_BorderLeft)
  {
   e.setAttribute("border-left",CEntity::toString(m_BorderLeft));
  }
  if (defaults || !m_BorderTop)
  {
   e.setAttribute("border-top",CEntity::toString(m_BorderTop));
  }
  if (defaults || !m_BorderRight)
  {
   e.setAttribute("border-right",CEntity::toString(m_BorderRight));
  }
  if (defaults || !m_BorderBottom)
  {
   e.setAttribute("border-bottom",CEntity::toString(m_BorderBottom));
  }
  root.appendChild(e);
  return true;
 }
 return false;
}

//------------------------------------------------------------------------------

CHole::CHole(const CHole& hole)
{
 assign(hole);
}

CHole::CHole(void)
{
 clear();
}

CHole::~CHole(void)
{
 clear();
}

void CHole::operator =(const CHole& hole)
{
 assign(hole);
}

void CHole::clear(void)
{
 m_X = 0.0;
 m_Y = 0.0;
 m_Drill = 0.1;
}

void CHole::assign(const CHole& hole)
{
 m_X = hole.m_X;
 m_Y = hole.m_Y;
 m_Drill = hole.m_Drill;
}

void CHole::scale(const double factor)
{
 m_X *= factor;
 m_Y *= factor;
 m_Drill *= factor;
}

void CHole::show(std::ostream& out, const int level)
{
 CEntity::show(out,level);
 out<<"Hole:{X="<<m_X<<", Y="<<m_Y<<", Drill="<<m_Drill<<"}"<<std::endl;
}

bool CHole::readFromXML(const QDomElement& root)
{
 QDomElement e = root;
 if (e.nodeName()!="hole") e = e.nextSiblingElement("hole");
 if (!e.isNull())
 {
  QString s = e.attribute("x");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toDouble(&ok);
   if (ok) { m_X = value; }
  }
  s = e.attribute("y");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toDouble(&ok);
   if (ok) { m_Y = value; }
  }
  s = e.attribute("drill");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toDouble(&ok);
   if (ok) { m_Drill = value; }
  }
  return true;
 }
 return false;
}

bool CHole::writeToXML(QDomDocument& host, QDomElement& root, const bool defaults)
{
 if (!host.isNull() && !root.isNull())
 {
  QDomElement e = host.createElement("hole");
  e.setAttribute("x",QString("%1").arg(m_X));
  e.setAttribute("y",QString("%1").arg(m_Y));
  e.setAttribute("drill",QString("%1").arg(m_Drill));
  root.appendChild(e);
  return true;
 }
 return false;
}

//------------------------------------------------------------------------------

CPad::CPad(const CPad& pad)
{
 assign(pad);
}

CPad::CPad(void)
{
 clear();
}

CPad::~CPad(void)
{
 clear();
}

QString CPad::toString(const CPad::Shape value)
{
 switch (value)
 {
  case CPad::psSquare: return "square";
  case CPad::psRound: return "round";
  case CPad::psOctagon: return "octagon";
  case CPad::psLong: return "long";
  case CPad::psOffset: return "offset";
 }
 return "";
}

void CPad::operator =(const CPad& pad)
{
 assign(pad);
}

void CPad::clear(void)
{
 m_Name = "P$1";
 m_X = 0.0;
 m_Y = 0.0;
 m_Drill = 0.1;
 m_Diameter = CPad::DEFAULT_DIAMETER;
 m_Shape = CPad::DEFAULT_SHAPE;
 m_Rotation = CPad::DEFAULT_ROTATION;
 m_Stop = true;
 m_Thermals = true;
 m_First = false;
}

void CPad::assign(const CPad& pad)
{
 m_Name = pad.m_Name;
 m_X = pad.m_X;
 m_Y = pad.m_Y;
 m_Drill = pad.m_Drill;
 m_Diameter = pad.m_Diameter;
 m_Shape = pad.m_Shape;
 m_Rotation = pad.m_Rotation;
 m_Stop = pad.m_Stop;
 m_Thermals = pad.m_Thermals;
 m_First = pad.m_First;
}

void CPad::scale(const double factor)
{
 m_X *= factor;
 m_Y *= factor;
 m_Drill *= factor;
 m_Diameter *= factor;
}

void CPad::show(std::ostream& out, const int level)
{
 CEntity::show(out,level);
 out<<"Pad:{Name='"<<m_Name.toUtf8().data()<<"', X="<<m_X<<", Y="<<m_Y<<", Drill="
    <<m_Drill<<", Diameter="<<m_Diameter<<", Shape="<<toString(m_Shape).toUtf8().data()
    <<", Rotation="<<m_Rotation<<", Stop="<<CEntity::toString(m_Stop).toUtf8().data()
    <<", Thermals="<<CEntity::toString(m_Thermals).toUtf8().data()
    <<", First="<<CEntity::toString(m_First).toUtf8().data()<<"}"<<std::endl;
}

bool CPad::readFromXML(const QDomElement& root)
{
 QDomElement e = root;
 if (e.nodeName()!="pad") e = e.nextSiblingElement("pad");
 if (!e.isNull())
 {
  QString s = e.attribute("name");
  if (!s.isEmpty())
  {
   m_Name = s;
  }
  s = e.attribute("x");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toDouble(&ok);
   if (ok) { m_X = value; }
  }
  s = e.attribute("y");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toDouble(&ok);
   if (ok) { m_Y = value; }
  }
  s = e.attribute("drill");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toDouble(&ok);
   if (ok) { m_Drill = value; }
  }
  s = e.attribute("diameter");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toDouble(&ok);
   if (ok) { m_Diameter = value; }
  }
  s = e.attribute("shape");
  if (!s.isEmpty())
  {
   if (s=="square") { m_Shape = CPad::psSquare; }
   else if (s=="round") { m_Shape = CPad::psRound; }
   else if (s=="octagon") { m_Shape = CPad::psOctagon; }
   else if (s=="long") { m_Shape = CPad::psLong; }
   else if (s=="offset") { m_Shape = CPad::psOffset; }
  }
  s = e.attribute("rot");
  if (!s.isEmpty())
  {
   bool reflection,spin;
   decodeTransformation(s,m_Rotation,reflection,spin);
   /*
   if (s.at(0)=='R')
   {
    bool ok = false;
    QString ss = s.right(s.length()-1);
    double value = ss.toDouble(&ok);
    if (ok) { m_Rotation = value; }
   }
   */
  }
  s = e.attribute("stop");
  if (!s.isEmpty())
  {
   if (s=="yes") { m_Stop = true; }
   else if (s=="no") { m_Stop = false; }
  }
  s = e.attribute("thermals");
  if (!s.isEmpty())
  {
   if (s=="yes") { m_Thermals = true; }
   else if (s=="no") { m_Thermals = false; }
  }
  s = e.attribute("first");
  if (!s.isEmpty())
  {
   if (s=="yes") { m_First = true; }
   else if (s=="no") { m_First = false; }
  }
  return true;
 }
 return false;
}

bool CPad::writeToXML(QDomDocument& host, QDomElement& root, const bool defaults)
{
 if (!host.isNull() && !root.isNull())
 {
  QDomElement e = host.createElement("pad");
  e.setAttribute("name",m_Name);
  e.setAttribute("x",QString("%1").arg(m_X));
  e.setAttribute("y",QString("%1").arg(m_Y));
  e.setAttribute("drill",QString("%1").arg(m_Drill));
  if (defaults || CPad::DEFAULT_DIAMETER!=m_Diameter)
  {
   e.setAttribute("diameter",QString("%1").arg(m_Diameter));
  }
  if (defaults || CPad::DEFAULT_SHAPE!=m_Shape)
  {
   e.setAttribute("shape",toString(m_Shape));
  }
  if (defaults || CPad::DEFAULT_ROTATION!=m_Rotation)
  {
   //e.setAttribute("rot",QString("R%1").arg(m_Rotation));
   e.setAttribute("rot",encodeTransformation(m_Rotation,false,false));
  }
  if (defaults || !m_Stop)
  {
   e.setAttribute("stop",CEntity::toString(m_Stop));
  }
  if (defaults || !m_Thermals)
  {
   e.setAttribute("thermals",CEntity::toString(m_Thermals));
  }
  if (defaults || m_First)
  {
   e.setAttribute("first",CEntity::toString(m_First));
  }
  root.appendChild(e);
  return true;
 }
 return false;
}


//------------------------------------------------------------------------------

CSMD::CSMD(const QString& name, const TCoord x, const TCoord y,
 const TDimension dx, const TDimension dy, const TLayer layer,
 const int roundness, const double rotation, const bool stop,
 const bool thermals, const bool cream)
{
 m_Name = name;
 m_X = x;
 m_Y = y;
 m_DX = dx;
 m_DY = dy;
 m_Layer = layer;
 m_Roundness = roundness;
 m_Rotation = rotation;
 m_Stop = stop;
 m_Thermals = thermals;
 m_Cream = cream;
}

CSMD::CSMD(const CSMD& smd)
{
 assign(smd);
}

CSMD::CSMD(void)
{
 clear();
}

CSMD::~CSMD(void)
{
 clear();
}

void CSMD::operator =(const CSMD& smd)
{
 assign(smd);
}

void CSMD::clear(void)
{
 m_Name = "P$1";
 m_X = 0.0;
 m_Y = 0.0;
 m_DX = 0.1;
 m_DY = 0.1;
 m_Layer = 1;
 m_Roundness = CSMD::DEFAULT_ROUNDNESS;
 m_Rotation = CSMD::DEFAULT_ROTATION;
 m_Stop = true;
 m_Thermals = true;
 m_Cream = false;
}

void CSMD::assign(const CSMD& smd)
{
 m_Name = smd.m_Name;
 m_X = smd.m_X;
 m_Y = smd.m_Y;
 m_DX = smd.m_DX;
 m_DY = smd.m_DY;
 m_Layer = smd.m_Layer;
 m_Roundness = smd.m_Roundness;
 m_Rotation = smd.m_Rotation;
 m_Stop = smd.m_Stop;
 m_Thermals = smd.m_Thermals;
 m_Cream = smd.m_Cream;
}

void CSMD::scale(const double factor)
{
 m_X *= factor;
 m_Y *= factor;
 m_DX *= factor;
 m_DY *= factor;
}

void CSMD::show(std::ostream& out, const int level)
{
 CEntity::show(out,level);
 out<<"SMD:{Name='"<<m_Name.toUtf8().data()<<"', X="<<m_X<<", Y="<<m_Y
    <<", DX="<<m_DX<<", DY="<<m_DY<<", Layer="<<m_Layer<<", Roundness="<<m_Roundness
    <<", Rotation="<<m_Rotation<<", Stop="<<CEntity::toString(m_Stop).toUtf8().data()
    <<", Thermals="<<CEntity::toString(m_Thermals).toUtf8().data()
    <<", Cream="<<CEntity::toString(m_Cream).toUtf8().data()<<"}"<<std::endl;
}

bool CSMD::readFromXML(const QDomElement& root)
{
 QDomElement e = root;
 if (e.nodeName()!="smd") e = e.nextSiblingElement("smd");
 if (!e.isNull())
 {
  QString s = e.attribute("name");
  if (!s.isEmpty())
  {
   m_Name = s;
  }
  s = e.attribute("x");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toDouble(&ok);
   if (ok) { m_X = value; }
  }
  s = e.attribute("y");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toDouble(&ok);
   if (ok) { m_Y = value; }
  }
  s = e.attribute("dx");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toDouble(&ok);
   if (ok) { m_DX = value; }
  }
  s = e.attribute("dy");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toDouble(&ok);
   if (ok) { m_DY = value; }
  }
  s = e.attribute("layer");
  if (!s.isEmpty())
  {
   bool ok = false;
   int value = s.toInt(&ok);
   if (ok) { m_Layer = value; }
  }
  s = e.attribute("roundness");
  if (!s.isEmpty())
  {
   bool ok = false;
   int value = s.toInt(&ok);
   if (ok) { m_Roundness = value; }
  }
  s = e.attribute("rot");
  if (!s.isEmpty())
  {
   bool reflection,spin;
   decodeTransformation(s,m_Rotation,reflection,spin);
   /*
   if (s.at(0)=='R')
   {
    bool ok = false;
    QString ss = s.right(s.length()-1);
    double value = ss.toDouble(&ok);
    if (ok) { m_Rotation = value; }
   }
   */
  }
  s = e.attribute("stop");
  if (!s.isEmpty())
  {
   if (s=="yes") { m_Stop = true; }
   else if (s=="no") { m_Stop = false; }
  }
  s = e.attribute("thermals");
  if (!s.isEmpty())
  {
   if (s=="yes") { m_Thermals = true; }
   else if (s=="no") { m_Thermals = false; }
  }
  s = e.attribute("cream");
  if (!s.isEmpty())
  {
   if (s=="yes") { m_Cream = true; }
   else if (s=="no") { m_Cream = false; }
  }
  return true;
 }
 return false;
}

bool CSMD::writeToXML(QDomDocument& host, QDomElement& root, const bool defaults)
{
 if (!host.isNull() && !root.isNull())
 {
  QDomElement e = host.createElement("smd");
  e.setAttribute("name",m_Name);
  e.setAttribute("x",QString("%1").arg(m_X));
  e.setAttribute("y",QString("%1").arg(m_Y));
  e.setAttribute("dx",QString("%1").arg(m_DX));
  e.setAttribute("dy",QString("%1").arg(m_DY));
  e.setAttribute("layer",QString("%1").arg(m_Layer));
  if (defaults || CSMD::DEFAULT_ROUNDNESS!=m_Roundness)
  {
   e.setAttribute("roundness",QString("%1").arg(m_Roundness));
  }
  if (defaults || CSMD::DEFAULT_ROTATION!=m_Rotation)
  {
   //e.setAttribute("rot",QString("R%1").arg(m_Rotation));
   e.setAttribute("rot",encodeTransformation(m_Rotation,false,false));
  }
  if (defaults || !m_Stop)
  {
   e.setAttribute("stop",CEntity::toString(m_Stop));
  }
  if (defaults || !m_Thermals)
  {
   e.setAttribute("thermals",CEntity::toString(m_Thermals));
  }
  if (defaults || m_Cream)
  {
   e.setAttribute("cream",CEntity::toString(m_Cream));
  }
  root.appendChild(e);
  return true;
 }
 return false;
}

//------------------------------------------------------------------------------

CVia::CVia(const CVia& via)
{
 assign(via);
}

CVia::CVia(void)
{
 clear();
}

CVia::~CVia(void)
{
 clear();
}

QString CVia::toString(const CVia::Shape value)
{
 switch (value)
 {
  case CVia::vsSquare: return "square";
  case CVia::vsRound: return "round";
  case CVia::vsOctagon: return "octagon";
 }
 return "";
}

void CVia::operator =(const CVia& via)
{
 assign(via);
}

void CVia::clear(void)
{
 m_X = 0.0;
 m_Y = 0.0;
 m_Extent.clear();
 m_Drill = 0.01;
 m_Diameter = CVia::DEFAULT_DIAMETER;
 m_Shape = CVia::DEFAULT_SHAPE;
 m_AlwaysStop = false;
}

void CVia::assign(const CVia& via)
{
 m_X = via.m_X;
 m_Y = via.m_Y;
 m_Extent = via.m_Extent;
 m_Drill = via.m_Drill;
 m_Diameter = via.m_Diameter;
 m_Shape = via.m_Shape;
 m_AlwaysStop = via.m_AlwaysStop;
}

void CVia::scale(const double factor)
{
 m_X *= factor;
 m_Y *= factor;
 m_Drill *= factor;
 m_Diameter *= factor;
}

void CVia::show(std::ostream& out, const int level)
{
 CEntity::show(out,level);
 out<<"Via:{X="<<m_X<<", Y="<<m_Y<<", Extent='"<<m_Extent.toUtf8().data()
    <<"', Drill="<<m_Drill<<", Diameter="<<m_Diameter
    <<", Shape="<<toString(m_Shape).toUtf8().data()
    <<", AlwaysStop="<<CEntity::toString(m_AlwaysStop).toUtf8().data()
    <<"}"<<std::endl;
}

bool CVia::readFromXML(const QDomElement& root)
{
 QDomElement e = root;
 if (e.nodeName()!="via") e = e.nextSiblingElement("via");
 if (!e.isNull())
 {
  QString s = e.attribute("x");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toDouble(&ok);
   if (ok) { m_X = value; }
  }
  s = e.attribute("y");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toDouble(&ok);
   if (ok) { m_Y = value; }
  }
  s = e.attribute("extent");
  if (!s.isEmpty())
  {
   m_Extent = s;
  }
  s = e.attribute("drill");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toDouble(&ok);
   if (ok) { m_Drill = value; }
  }
  s = e.attribute("diameter");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toDouble(&ok);
   if (ok) { m_Diameter = value; }
  }
  s = e.attribute("shape");
  if (!s.isEmpty())
  {
   if (s=="square") { m_Shape = CVia::vsSquare; }
   else if (s=="round") { m_Shape = CVia::vsRound; }
   else if (s=="octagon") { m_Shape = CVia::vsOctagon; }
  }
  s = e.attribute("alwaysstop");
  if (!s.isEmpty())
  {
   if (s=="yes") { m_AlwaysStop = true; }
   else if (s=="no") { m_AlwaysStop = false; }
  }
  return true;
 }
 return false;
}

bool CVia::writeToXML(QDomDocument& host, QDomElement& root, const bool defaults)
{
 if (!host.isNull() && !root.isNull())
 {
  QDomElement e = host.createElement("via");
  e.setAttribute("x",QString("%1").arg(m_X));
  e.setAttribute("y",QString("%1").arg(m_Y));
  e.setAttribute("extent",m_Extent);
  e.setAttribute("drill",QString("%1").arg(m_Drill));
  if (defaults || CVia::DEFAULT_DIAMETER!=m_Diameter)
  {
   e.setAttribute("diameter",QString("%1").arg(m_Diameter));
  }
  if (defaults || CVia::DEFAULT_SHAPE!=m_Shape)
  {
   e.setAttribute("shape",toString(m_Shape));
  }
  if (defaults || !m_AlwaysStop)
  {
   e.setAttribute("alwaysstop",CEntity::toString(m_AlwaysStop));
  }
  root.appendChild(e);
  return true;
 }
 return false;
}

//------------------------------------------------------------------------------

CVertex::CVertex(const TCoord x, const TCoord y, const double curve)
{
 clear();
 m_X = x;
 m_Y = y;
 setCurve(curve);
}

CVertex::CVertex(const CVertex& vertex)
{
 assign(vertex);
}

CVertex::CVertex(void)
{
 clear();
}

CVertex::~CVertex(void)
{
 clear();
}

void CVertex::operator =(const CVertex& vertex)
{
 assign(vertex);
}

void CVertex::clear(void)
{
 m_X = 0.0;
 m_Y = 0.0;
 m_Curve = 0.0;
}

void CVertex::assign(const CVertex& vertex)
{
 m_X = vertex.m_X;
 m_Y = vertex.m_Y;
 m_Curve = vertex.m_Curve;
}

void CVertex::scale(const double factor)
{
 m_X *= factor;
 m_Y *= factor;
}

void CVertex::show(std::ostream& out, const int level)
{
 CEntity::show(out,level);
 out<<"Vertex:{X="<<m_X<<", Y="<<m_Y<<", Curve="<<m_Curve<<"}"<<std::endl;
}

bool CVertex::readFromXML(const QDomElement& root)
{
 QDomElement e = root;
 if (e.nodeName()!="vertex") e = e.nextSiblingElement("vertex");
 if (!e.isNull())
 {
  QString s = e.attribute("x");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toDouble(&ok);
   if (ok) { m_X = value; }
  }
  s = e.attribute("y");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toDouble(&ok);
   if (ok) { m_Y = value; }
  }
  s = e.attribute("curve");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toDouble(&ok);
   if (ok) { m_Curve = value; }
  }
  return true;
 }
 return false;
}

bool CVertex::writeToXML(QDomDocument& host, QDomElement& root, const bool defaults)
{
 if (!host.isNull() && !root.isNull())
 {
  QDomElement e = host.createElement("vertex");
  e.setAttribute("x",QString("%1").arg(m_X));
  e.setAttribute("y",QString("%1").arg(m_Y));
  if (defaults || CVertex::DEFAULT_CURVE!=m_Curve)
  {
   e.setAttribute("curve",QString("%1").arg(m_Curve));
  }
  root.appendChild(e);
  return true;
 }
 return false;
}

//------------------------------------------------------------------------------

CPin::CPin(const CPin& pin)
{
 assign(pin);
}

CPin::CPin(void)
{
 clear();
}

CPin::~CPin(void)
{
 clear();
}

QString CPin::toString(const CPin::Visible value)
{
 switch (value)
 {
  case CPin::pvOff: return "off";
  case CPin::pvPad: return "pad";
  case CPin::pvPin: return "pin";
  case CPin::pvBoth: return "both";
 }
 return "";
}

QString CPin::toString(const CPin::Length value)
{
 switch (value)
 {
  case CPin::plPoint: return "point";
  case CPin::plShort: return "short";
  case CPin::plMiddle: return "middle";
  case CPin::plLong: return "long";
 }
 return "";
}

QString CPin::toString(const CPin::Direction value)
{
 switch (value)
 {
  case CPin::pdNC: return "nc";
  case CPin::pdIN: return "in";
  case CPin::pdOUT: return "out";
  case CPin::pdIO: return "io";
  case CPin::pdOC: return "oc";
  case CPin::pdPWR: return "pwr";
  case CPin::pdPAS: return "pas";
  case CPin::pdHIZ: return "hiz";
  case CPin::pdSUP: return "sup";
 }
 return "";
}

QString CPin::toString(const CPin::Function value)
{
 switch (value)
 {
  case CPin::pfNone: return "none";
  case CPin::pfDot: return "dot";
  case CPin::pfClk: return "clk";
  case CPin::pfDotClk: return "dotclk";
 }
 return "";
}

void CPin::operator =(const CPin& pin)
{
 assign(pin);
}

void CPin::clear(void)
{
 m_Name = "";
 m_X = 0.0;
 m_Y = 0.0;
 m_Visible = CPin::DEFAULT_VISIBLE;
 m_Length = CPin::DEFAULT_LENGTH;
 m_Direction = CPin::DEFAULT_DIRECTION;
 m_Function = CPin::DEFAULT_FUNCTION;
 m_SwapLevel = CPin::DEFAULT_SWAPLEVEL;
 m_Rotation = CPin::DEFAULT_ROTATION;
}

void CPin::assign(const CPin& pin)
{
 m_Name = pin.m_Name;
 m_X = pin.m_X;
 m_Y = pin.m_Y;
 m_Visible = pin.m_Visible;
 m_Length = pin.m_Length;
 m_Direction = pin.m_Direction;
 m_Function = pin.m_Function;
 m_SwapLevel = pin.m_SwapLevel;
 m_Rotation = pin.m_Rotation;
}

void CPin::scale(const double factor)
{
 m_X *= factor;
 m_Y *= factor;
}

void CPin::show(std::ostream& out, const int level)
{
 CEntity::show(out,level);
 out<<"Pin:{Name='"<<m_Name.toUtf8().data()<<"', X="<<m_X<<", Y="<<m_Y
    <<", Visible="<<toString(m_Visible).toUtf8().data()
    <<", Length="<<toString(m_Length).toUtf8().data()
    <<", Direction="<<toString(m_Direction).toUtf8().data()
    <<", Function="<<toString(m_Function).toUtf8().data()
    <<", SwapLevel="<<m_SwapLevel<<", Rotation="<<m_Rotation
    <<"}"<<std::endl;
}

bool CPin::readFromXML(const QDomElement& root)
{
 QDomElement e = root;
 if (e.nodeName()!="pin") e = e.nextSiblingElement("pin");
 if (!e.isNull())
 {
  QString s = e.attribute("name");
  if (!s.isEmpty())
  {
   m_Name = s;
  }
  s = e.attribute("x");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toDouble(&ok);
   if (ok) { m_X = value; }
  }
  s = e.attribute("y");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toDouble(&ok);
   if (ok) { m_Y = value; }
  }
  s = e.attribute("visible");
  if (!s.isEmpty())
  {
   if (s=="off") { m_Visible = CPin::pvOff; }
   else if (s=="pad") { m_Visible = CPin::pvPad; }
   else if (s=="pin") { m_Visible = CPin::pvPin; }
   else if (s=="both") { m_Visible = CPin::pvBoth; }
  }
  s = e.attribute("length");
  if (!s.isEmpty())
  {
   if (s=="point") { m_Length = CPin::plPoint; }
   else if (s=="short") { m_Length = CPin::plShort; }
   else if (s=="middle") { m_Length = CPin::plMiddle; }
   else if (s=="long") { m_Length = CPin::plLong; }
  }
  s = e.attribute("direction");
  if (!s.isEmpty())
  {
   if (s=="nc") { m_Direction = CPin::pdNC; }
   else if (s=="in") { m_Direction = CPin::pdIN; }
   else if (s=="out") { m_Direction = CPin::pdOUT; }
   else if (s=="io") { m_Direction = CPin::pdIO; }
   else if (s=="oc") { m_Direction = CPin::pdOC; }
   else if (s=="pwr") { m_Direction = CPin::pdPWR; }
   else if (s=="pas") { m_Direction = CPin::pdPAS; }
   else if (s=="hiz") { m_Direction = CPin::pdHIZ; }
   else if (s=="sup") { m_Direction = CPin::pdSUP; }
  }
  s = e.attribute("function");
  if (!s.isEmpty())
  {
   if (s=="none") { m_Function = CPin::pfNone; }
   else if (s=="dot") { m_Function = CPin::pfDot; }
   else if (s=="clk") { m_Function = CPin::pfClk; }
   else if (s=="dotclk") { m_Function = CPin::pfDotClk; }
  }
  s = e.attribute("swaplevel");
  if (!s.isEmpty())
  {
   bool ok = false;
   int value = s.toInt(&ok);
   if (ok) { m_SwapLevel = value; }
  }
  s = e.attribute("rot");
  if (!s.isEmpty())
  {
   bool reflection,spin;
   decodeTransformation(s,m_Rotation,reflection,spin);
   /*
   if (s.at(0)=='R')
   {
    bool ok = false;
    QString ss = s.right(s.length()-1);
    double value = ss.toDouble(&ok);
    if (ok) { m_Rotation = value; }
   }
   */
  }
  return true;
 }
 return false;
}

bool CPin::writeToXML(QDomDocument& host, QDomElement& root, const bool defaults)
{
 if (!host.isNull() && !root.isNull())
 {
  QDomElement e = host.createElement("pin");
  e.setAttribute("name",QString("%1").arg(m_Name));
  e.setAttribute("x",QString("%1").arg(m_X));
  e.setAttribute("y",QString("%1").arg(m_Y));
  if (defaults || CPin::DEFAULT_VISIBLE!=m_Visible)
  {
   e.setAttribute("visible",toString(m_Visible));
  }
  if (defaults || CPin::DEFAULT_LENGTH!=m_Length)
  {
   e.setAttribute("length",toString(m_Length));
  }
  if (defaults || CPin::DEFAULT_DIRECTION!=m_Direction)
  {
   e.setAttribute("direction",toString(m_Direction));
  }
  if (defaults || CPin::DEFAULT_FUNCTION!=m_Function)
  {
   e.setAttribute("function",toString(m_Function));
  }
  if (defaults || CPin::DEFAULT_SWAPLEVEL!=m_SwapLevel)
  {
   e.setAttribute("swaplevel",m_SwapLevel);
  }
  if (defaults || CPin::DEFAULT_ROTATION!=m_Rotation)
  {
   //e.setAttribute("rot",QString("R%1").arg(m_Rotation));
   e.setAttribute("rot",encodeTransformation(m_Rotation,false,false));
  }
  root.appendChild(e);
  return true;
 }
 return false;
}

//------------------------------------------------------------------------------

CLabel::CLabel(const CLabel& label)
{
 assign(label);
}

CLabel::CLabel(void)
{
 clear();
}

CLabel::~CLabel(void)
{
 clear();
}

void CLabel::operator =(const CLabel& label)
{
 assign(label);
}

void CLabel::clear(void)
{
 m_X = 0.0;
 m_Y = 0.0;
 m_Size = 0.1;
 m_Layer = 1;
 m_Font = CLabel::DEFAULT_FONT;
 m_Ratio = CLabel::DEFAULT_RATIO;
 m_Rotation = CLabel::DEFAULT_ROTATION;
 m_Reflection = false;
 m_XRef = false;
}

void CLabel::assign(const CLabel& label)
{
 m_X = label.m_X;
 m_Y = label.m_Y;
 m_Size = label.m_Size;
 m_Layer = label.m_Layer;
 m_Font = label.m_Font;
 m_Ratio = label.m_Ratio;
 m_Rotation = label.m_Rotation;
 m_Reflection = label.m_Reflection;
 m_XRef = label.m_XRef;
}

void CLabel::scale(const double factor)
{
 m_X *= factor;
 m_Y *= factor;
 m_Size *= factor;
}

void CLabel::show(std::ostream& out, const int level)
{
 CEntity::show(out,level);
 out<<"Label:{X="<<m_X<<", Y="<<m_Y<<", Size="<<m_Size<<", Layer="<<m_Layer
    <<", Font="<<CText::toString(m_Font).toUtf8().data()
    <<", Ratio="<<m_Ratio<<", Rotation="<<m_Rotation
    <<", XRef="<<CEntity::toString(m_XRef).toUtf8().data()
    <<"}"<<std::endl;
}

bool CLabel::readFromXML(const QDomElement& root)
{
 QDomElement e = root;
 if (e.nodeName()!="label") e = e.nextSiblingElement("label");
 if (!e.isNull())
 {
  QString s = e.attribute("x");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toDouble(&ok);
   if (ok) { m_X = value; }
  }
  s = e.attribute("y");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toDouble(&ok);
   if (ok) { m_Y = value; }
  }
  s = e.attribute("size");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toDouble(&ok);
   if (ok) { m_Size = value; }
  }
  s = e.attribute("layer");
  if (!s.isEmpty())
  {
   bool ok = false;
   int value = s.toInt(&ok);
   if (ok) { m_Layer = value; }
  }
  s = e.attribute("font");
  if (!s.isEmpty())
  {
   if (s=="vector") { m_Font = CText::tfVector; }
   else if (s=="proportional") { m_Font = CText::tfProportional; }
   else if (s=="fixed") { m_Font = CText::tfFixed; }
  }
  s = e.attribute("ratio");
  if (!s.isEmpty())
  {
   bool ok = false;
   int value = s.toInt(&ok);
   if (ok) { m_Ratio = value; }
  }
  s = e.attribute("rot");
  if (!s.isEmpty())
  {
   bool spin;
   decodeTransformation(s,m_Rotation,m_Reflection,spin);
   /*
   if (s.at(0)=='R')
   {
    bool ok = false;
    QString ss = s.right(s.length()-1);
    double value = ss.toDouble(&ok);
    if (ok) { m_Rotation = value; }
   }
   */
  }
  s = e.attribute("xref");
  if (!s.isEmpty())
  {
   if (s=="yes") { m_XRef = true; }
   else if (s=="no") { m_XRef = false; }
  }
  return true;
 }
 return false;
}

bool CLabel::writeToXML(QDomDocument& host, QDomElement& root, const bool defaults)
{
 if (!host.isNull() && !root.isNull())
 {
  QDomElement e = host.createElement("label");
  e.setAttribute("x",QString("%1").arg(m_X));
  e.setAttribute("y",QString("%1").arg(m_Y));
  e.setAttribute("size",QString("%1").arg(m_Size));
  e.setAttribute("layer",QString("%1").arg(m_Layer));
  if (defaults || CLabel::DEFAULT_FONT!=m_Font)
  {
   e.setAttribute("font",CText::toString(m_Font));
  }
  if (defaults || CLabel::DEFAULT_RATIO!=m_Ratio)
  {
   e.setAttribute("ratio",toString(m_Ratio));
  }
  if (defaults || CLabel::DEFAULT_ROTATION!=m_Rotation || m_Reflection)
  {
   //e.setAttribute("rot",QString("R%1").arg(m_Rotation));
   e.setAttribute("rot",encodeTransformation(m_Rotation,m_Reflection,false));
  }
  if (defaults || m_XRef)
  {
   e.setAttribute("xref",CEntity::toString(m_XRef));
  }
  root.appendChild(e);
  return true;
 }
 return false;
}

void CLabel::setRotation(const double value)
{
 if ((value<45.0) || (value>=315.0)) { m_Rotation = 0.0; }
 else if (value<135.0) { m_Rotation = 90.0; }
 else if (value<225.0) { m_Rotation = 180.0; }
 else if (value<315.0) { m_Rotation = 270.0; }
}

//------------------------------------------------------------------------------

CJunction::CJunction(const CJunction& junction)
{
 assign(junction);
}

CJunction::CJunction(void)
{
 clear();
}

CJunction::~CJunction(void)
{
 clear();
}

void CJunction::operator =(const CJunction& junction)
{
 assign(junction);
}

void CJunction::clear(void)
{
 m_X = 0.0;
 m_Y = 0.0;
}

void CJunction::assign(const CJunction& junction)
{
 m_X = junction.m_X;
 m_Y = junction.m_Y;
}

void CJunction::scale(const double factor)
{
 m_X *= factor;
 m_Y *= factor;
}

void CJunction::show(std::ostream& out, const int level)
{
 CEntity::show(out,level);
 out<<"Junction:{X="<<m_X<<", Y="<<m_Y<<"}"<<std::endl;
}

bool CJunction::readFromXML(const QDomElement& root)
{
 QDomElement e = root;
 if (e.nodeName()!="junction") e = e.nextSiblingElement("junction");
 if (!e.isNull())
 {
  QString s = e.attribute("x");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toDouble(&ok);
   if (ok) { m_X = value; }
  }
  s = e.attribute("y");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toDouble(&ok);
   if (ok) { m_Y = value; }
  }
  return true;
 }
 return false;
}

bool CJunction::writeToXML(QDomDocument& host, QDomElement& root, const bool defaults)
{
 if (!host.isNull() && !root.isNull())
 {
  QDomElement e = host.createElement("junction");
  e.setAttribute("x",QString("%1").arg(m_X));
  e.setAttribute("y",QString("%1").arg(m_Y));
  root.appendChild(e);
  return true;
 }
 return false;
}

//------------------------------------------------------------------------------

CConnect::CConnect(const CConnect& connect)
{
 assign(connect);
}

CConnect::CConnect(void)
{
 clear();
}

CConnect::~CConnect(void)
{
 clear();
}

QString CConnect::toString(const CConnect::Route value)
{
 switch (value)
 {
  case CConnect::crAll: return "all";
  case CConnect::crAny: return "any";
 }
 return "";
}

void CConnect::operator =(const CConnect& connect)
{
 assign(connect);
}

void CConnect::clear(void)
{
 m_Gate.clear();
 m_Pin.clear();
 m_Pad.clear();
 m_Route = CConnect::DEFAULT_ROUTE;
}

void CConnect::assign(const CConnect& connect)
{
 m_Gate = connect.m_Gate;
 m_Pin = connect.m_Pin;
 m_Pad = connect.m_Pad;
 m_Route = connect.m_Route;
}

void CConnect::show(std::ostream& out, const int level)
{
 CEntity::show(out,level);
 out<<"Connect:{Gate='"<<m_Gate.toUtf8().data()<<"', Pin='"<<m_Pin.toUtf8().data()
    <<"', Pad='"<<m_Pad.toUtf8().data()<<"', Route="<<toString(m_Route).toUtf8().data()
    <<"}"<<std::endl;
}

bool CConnect::readFromXML(const QDomElement& root)
{
 QDomElement e = root;
 if (e.nodeName()!="connect") e = e.nextSiblingElement("connect");
 if (!e.isNull())
 {
  QString s = e.attribute("gate");
  if (!s.isEmpty())
  {
   m_Gate = s;
  }
  s = e.attribute("pin");
  if (!s.isEmpty())
  {
   m_Pin = s;
  }
  s = e.attribute("pad");
  if (!s.isEmpty())
  {
   m_Pad = s;
  }
  s = e.attribute("route");
  if (!s.isEmpty())
  {
   if (s=="all") { m_Route = CConnect::crAll; }
   else if (s=="any") { m_Route = CConnect::crAny; }
  }
  return true;
 }
 return false;
}

bool CConnect::writeToXML(QDomDocument& host, QDomElement& root, const bool defaults)
{
 if (!host.isNull() && !root.isNull())
 {
  QDomElement e = host.createElement("connect");
  e.setAttribute("gate",m_Gate);
  e.setAttribute("pin",m_Pin);
  e.setAttribute("pad",m_Pad);
  if (defaults || CConnect::DEFAULT_ROUTE!=m_Route)
  {
   e.setAttribute("route",toString(m_Route));
  }
  root.appendChild(e);
  return true;
 }
 return false;
}

//------------------------------------------------------------------------------

CAttribute::CAttribute(const CAttribute& attribute)
{
 assign(attribute);
}

CAttribute::CAttribute(void)
{
 clear();
}

CAttribute::~CAttribute(void)
{
 clear();
}

QString CAttribute::toString(const CAttribute::Display value)
{
 switch (value)
 {
  case CAttribute::adOff: return "off";
  case CAttribute::adValue: return "value";
  case CAttribute::adName: return "name";
  case CAttribute::adBoth: return "both";
 }
 return "";
}

void CAttribute::operator =(const CAttribute& attribute)
{
 assign(attribute);
}

void CAttribute::clear(void)
{
 m_Name.clear();
 m_Value.clear();
 m_X = 0.0;
 m_Y = 0.0;
 m_Size = 0.1;
 m_Layer = 1;
 m_Font = CText::tfProportional;
 m_Ratio = 8;
 m_Rotation  = CAttribute::DEFAULT_ROTATION;
 m_Reflection = false;
 m_Display = CAttribute::DEFAULT_DISPLAY;
 m_Constant = false;
}

void CAttribute::assign(const CAttribute& attribute)
{
 m_Name = attribute.m_Name;
 m_Value = attribute.m_Value;
 m_X = attribute.m_X;
 m_Y = attribute.m_Y;
 m_Size = attribute.m_Size;
 m_Layer = attribute.m_Layer;
 m_Font = attribute.m_Font;
 m_Ratio = attribute.m_Ratio;
 m_Rotation  = attribute.m_Rotation;
 m_Reflection = attribute.m_Reflection;
 m_Display = attribute.m_Display;
 m_Constant = attribute.m_Constant;
}

void CAttribute::scale(const double factor)
{
 m_X *= factor;
 m_Y *= factor;
 m_Size *= factor;
}

void CAttribute::show(std::ostream& out, const int level)
{
 CEntity::show(out,level);
 out<<"Attribute:{Name='"<<m_Name.toUtf8().data()<<"', Value='"<<m_Value.toUtf8().data()
    <<"', X="<<m_X<<", Y="<<m_Y<<", Size="<<m_Size<<", Layer="<<m_Layer
    <<", Font="<<CText::toString(m_Font).toUtf8().data()<<", Ratio="<<m_Ratio
    <<", Rotation="<<m_Rotation<<", Display="<<toString(m_Display).toUtf8().data()
    <<", Constant="<<CEntity::toString(m_Constant).toUtf8().data()
    <<"}"<<std::endl;
}

bool CAttribute::readFromXML(const QDomElement& root)
{
 QDomElement e = root;
 if (e.nodeName()!="attribute") e = e.nextSiblingElement("attribute");
 if (!e.isNull())
 {
  QString s = e.attribute("name");
  if (!s.isEmpty())
  {
   m_Name = s;
  }
  s = e.attribute("value");
  if (!s.isEmpty())
  {
   m_Value = s;
  }
  s = e.attribute("x");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toDouble(&ok);
   if (ok) { m_X = value; }
  }
  s = e.attribute("y");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toDouble(&ok);
   if (ok) { m_Y = value; }
  }
  s = e.attribute("size");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toDouble(&ok);
   if (ok) { m_Size = value; }
  }
  s = e.attribute("layer");
  if (!s.isEmpty())
  {
   bool ok = false;
   int value = s.toInt(&ok);
   if (ok) { m_Layer = value; }
  }
  s = e.attribute("font");
  if (!s.isEmpty())
  {
   if (s=="vector") { m_Font = CText::tfVector; }
   else if (s=="proportional") { m_Font = CText::tfProportional; }
   else if (s=="fixed") { m_Font = CText::tfFixed; }
  }
  s = e.attribute("ratio");
  if (!s.isEmpty())
  {
   bool ok = false;
   int value = s.toInt(&ok);
   if (ok) { m_Ratio = value; }
  }
  s = e.attribute("rot");
  if (!s.isEmpty())
  {
   bool spin;
   decodeTransformation(s,m_Rotation,m_Reflection,spin);
   /*
   if (s.at(0)=='R')
   {
    bool ok = false;
    QString ss = s.right(s.length()-1);
    double value = ss.toDouble(&ok);
    if (ok) { m_Rotation = value; }
   }
   */
  }
  s = e.attribute("display");
  if (!s.isEmpty())
  {
   if (s=="off") { m_Display = CAttribute::adOff; }
   else if (s=="value") { m_Display = CAttribute::adValue; }
   else if (s=="name") { m_Display = CAttribute::adName; }
   else if (s=="both") { m_Display = CAttribute::adBoth; }
  }
  s = e.attribute("constant");
  if (!s.isEmpty())
  {
   if (s=="yes") { m_Constant = true; }
   else if (s=="no") { m_Constant = false; }
  }
  return true;
 }
 return false;
}

bool CAttribute::writeToXML(QDomDocument& host, QDomElement& root, const bool defaults)
{
 if (!host.isNull() && !root.isNull())
 {
  QDomElement e = host.createElement("attribute");
  e.setAttribute("name",m_Name);
  if (defaults || !m_Value.isEmpty())
  {
   e.setAttribute("value",m_Value);
  }
  e.setAttribute("x",QString("%1").arg(m_X));
  e.setAttribute("y",QString("%1").arg(m_Y));
  e.setAttribute("size",QString("%1").arg(m_Size));
  e.setAttribute("layer",QString("%1").arg(m_Layer));
  if (defaults || CText::DEFAULT_FONT!=m_Font)
  {
   e.setAttribute("font",CText::toString(m_Font));
  }
  if (defaults || CText::DEFAULT_RATIO!=m_Ratio)
  {
   e.setAttribute("ratio",QString("%1").arg(m_Ratio));
  }
  if (defaults || CAttribute::DEFAULT_ROTATION!=m_Rotation || m_Reflection)
  {
   //e.setAttribute("rot",QString("R%1").arg(m_Rotation));
   e.setAttribute("rot",encodeTransformation(m_Rotation,m_Reflection,false));
  }
  if (defaults || CAttribute::DEFAULT_DISPLAY!=m_Display)
  {
   e.setAttribute("display",toString(m_Display));
  }
  if (defaults || m_Constant)
  {
   e.setAttribute("constant",CEntity::toString(m_Constant));
  }
  root.appendChild(e);
  return true;
 }
 return false;
}

//------------------------------------------------------------------------------

CPinRef::CPinRef(const CPinRef& pinRef)
{
 assign(pinRef);
}

CPinRef::CPinRef(void)
{
 clear();
}

CPinRef::~CPinRef(void)
{
 clear();
}

void CPinRef::operator =(const CPinRef& pinRef)
{
 assign(pinRef);
}

void CPinRef::clear(void)
{
 m_Part.clear();
 m_Gate.clear();
 m_Pin.clear();
}

void CPinRef::assign(const CPinRef& pinRef)
{
 m_Part = pinRef.m_Part;
 m_Gate = pinRef.m_Gate;
 m_Pin = pinRef.m_Pin;
}

void CPinRef::show(std::ostream& out, const int level)
{
 CEntity::show(out,level);
 out<<"PinRef:{Part='"<<m_Part.toUtf8().data()<<"', Gate='"<<m_Gate.toUtf8().data()
    <<"', Pin='"<<m_Pin.toUtf8().data()
    <<"'}"<<std::endl;
}

bool CPinRef::readFromXML(const QDomElement& root)
{
 QDomElement e = root;
 if (e.nodeName()!="pinref") e = e.nextSiblingElement("pinref");
 if (!e.isNull())
 {
  QString s = e.attribute("part");
  if (!s.isEmpty())
  {
   m_Part = s;
  }
  s = e.attribute("gate");
  if (!s.isEmpty())
  {
   m_Gate = s;
  }
  s = e.attribute("pin");
  if (!s.isEmpty())
  {
   m_Pin = s;
  }
  return true;
 }
 return false;
}

bool CPinRef::writeToXML(QDomDocument& host, QDomElement& root, const bool defaults)
{
 if (!host.isNull() && !root.isNull())
 {
  QDomElement e = host.createElement("pinref");
  e.setAttribute("part",m_Part);
  e.setAttribute("gate",m_Gate);
  e.setAttribute("pin",m_Pin);
  root.appendChild(e);
  return true;
 }
 return false;
}

//------------------------------------------------------------------------------

CContactRef::CContactRef(const CContactRef& contactRef)
{
 assign(contactRef);
}

CContactRef::CContactRef(void)
{
 clear();
}

CContactRef::~CContactRef(void)
{
 clear();
}

void CContactRef::operator =(const CContactRef& contactRef)
{
 assign(contactRef);
}

void CContactRef::clear(void)
{
 m_Element.clear();
 m_Pad.clear();
 m_Route = CConnect::crAll;
 m_RouteTag.clear();
}

void CContactRef::assign(const CContactRef& contactRef)
{
 m_Element = contactRef.m_Element;
 m_Pad = contactRef.m_Pad;
 m_Route = contactRef.m_Route;
 m_RouteTag = contactRef.m_RouteTag;
}

void CContactRef::show(std::ostream& out, const int level)
{
 CEntity::show(out,level);
 out<<"ContactRef:{Element='"<<m_Element.toUtf8().data()
    <<"', Pad='"<<m_Pad.toUtf8().data()
    <<"', Route="<<CConnect::toString(m_Route).toUtf8().data()
    <<", RouteTag='"<<m_RouteTag.toUtf8().data()
    <<"'}"<<std::endl;
}

bool CContactRef::readFromXML(const QDomElement& root)
{
 QDomElement e = root;
 if (e.nodeName()!="contactref") e = e.nextSiblingElement("contactref");
 if (!e.isNull())
 {
  QString s = e.attribute("element");
  if (!s.isEmpty())
  {
   m_Element = s;
  }
  s = e.attribute("pad");
  if (!s.isEmpty())
  {
   m_Pad = s;
  }
  s = e.attribute("route");
  if (!s.isEmpty())
  {
   if (s=="all") { m_Route = CConnect::crAll; }
   else if (s=="any") { m_Route = CConnect::crAny; }
  }
  s = e.attribute("routetag");
  if (!s.isEmpty())
  {
   m_RouteTag = s;
  }
  return true;
 }
 return false;
}

bool CContactRef::writeToXML(QDomDocument& host, QDomElement& root, const bool defaults)
{
 if (!host.isNull() && !root.isNull())
 {
  QDomElement e = host.createElement("contactref");
  e.setAttribute("element",m_Element);
  e.setAttribute("pad",m_Pad);
  if (defaults || CContactRef::DEFAULT_ROUTE!=m_Route)
  {
   e.setAttribute("route",CConnect::toString(m_Route));
  }
  if (defaults || !m_RouteTag.isEmpty())
  {
   e.setAttribute("routetag",m_RouteTag);
  }
  root.appendChild(e);
  return true;
 }
 return false;
}

//------------------------------------------------------------------------------

CTechnology::CTechnology(const CTechnology& technology)
{
 assign(technology);
}

CTechnology::CTechnology(void)
{
 clear();
}

CTechnology::~CTechnology(void)
{
 clear();
}

void CTechnology::operator =(const CTechnology& technology)
{
 assign(technology);
}

void CTechnology::clear(void)
{
 for (int i = 0; i < m_Attributes.size(); i++)
 {
  if (0!=m_Attributes[i]) delete m_Attributes[i];
 }
 m_Attributes.clear();
 m_Name.clear();
}

void CTechnology::assign(const CTechnology& technology)
{
 clear();
 for (int i = 0; i < technology.m_Attributes.size(); i++)
 {
  CAttribute *a = new CAttribute(*technology.m_Attributes.at(i));
  m_Attributes.append(a);
 }
 m_Name = technology.m_Name;
}

void CTechnology::scale(const double factor)
{
 for (int i = 0; i < m_Attributes.size(); i++)
 {
  m_Attributes[i]->scale(factor);
 }
}

void CTechnology::show(std::ostream& out, const int level)
{
 CEntity::show(out,level); out<<"Technology:"<<std::endl;
 CEntity::show(out,level); out<<"{"<<std::endl;
 CEntity::show(out,level); out<<"\tName='"<<m_Name.toUtf8().data()<<"',"<<std::endl;
 CEntity::show(out,level); out<<"\tAttributes="<<std::endl;
 CEntity::show(out,level); out<<"\t{"<<std::endl;
 for (int i = 0; i < m_Attributes.size(); i++)
 {
  m_Attributes[i]->show(out,level+2);
 }
 CEntity::show(out,level); out<<"\t}"<<std::endl;
 CEntity::show(out,level); out<<"}"<<std::endl;
}

bool CTechnology::readFromXML(const QDomElement& root)
{
 QDomElement e = root;
 if (e.nodeName()!="technology") e = e.nextSiblingElement("technology");
 if (!e.isNull())
 {
  QDomElement c = e.firstChildElement("attributes");
  if (!c.isNull())
  {
   QDomElement cc = c.firstChildElement("attribute");
   while (!cc.isNull())
   {
    CAttribute *a = new CAttribute();
    m_Attributes.append(a);
    a->readFromXML(cc);
    cc = cc.nextSiblingElement("attribute");
   }
  }
  QString s = e.attribute("name");
  if (!s.isEmpty())
  {
   m_Name = s;
  }
  return true;
 }
 return false;
}

bool CTechnology::writeToXML(QDomDocument& host, QDomElement& root, const bool defaults)
{
 if (!host.isNull() && !root.isNull())
 {
  QDomElement e = host.createElement("technology");
  e.setAttribute("name",m_Name);
  for (int i = 0; i < m_Attributes.size(); i++)
  {
   m_Attributes[i]->writeToXML(host,e,defaults);
  }
  root.appendChild(e);
  return true;
 }
 return false;
}

//------------------------------------------------------------------------------

CInstance::CInstance(const CInstance& instance)
{
 assign(instance);
}

CInstance::CInstance(void)
{
 clear();
}

CInstance::~CInstance(void)
{
 clear();
}

void CInstance::operator =(const CInstance& instance)
{
 assign(instance);
}

void CInstance::clear(void)
{
 for (int i = 0; i < m_Attributes.size(); i++)
 {
  if (0!=m_Attributes[i]) delete m_Attributes[i];
 }
 m_Attributes.clear();
 m_Part.clear();
 m_Gate.clear();
 m_X = 0.0;
 m_Y = 0.0;
 m_Smashed = false;
 m_Rotation = 0.0;
 m_Reflection = false;
 m_Spin = false;
}

void CInstance::assign(const CInstance& instance)
{
 clear();
 for (int i = 0; i < instance.m_Attributes.size(); i++)
 {
  CAttribute *a = new CAttribute(*instance.m_Attributes.at(i));
  m_Attributes.append(a);
 }
 m_Part = instance.m_Part;
 m_Gate = instance.m_Gate;
 m_X = instance.m_X;
 m_Y = instance.m_Y;
 m_Smashed = instance.m_Smashed;
 m_Rotation = instance.m_Rotation;
 m_Spin = instance.m_Spin;
}

void CInstance::scale(const double factor)
{
 for (int i = 0; i < m_Attributes.size(); i++)
 {
  m_Attributes[i]->scale(factor);
 }
 m_X *= factor;
 m_Y *= factor;
}

void CInstance::show(std::ostream& out, const int level)
{
 CEntity::show(out,level);
 out<<"Instance:{Part='"<<m_Part.toUtf8().data()<<"', Gate='"<<m_Gate.toUtf8().data()
    <<"', X="<<m_X<<", Y="<<m_Y<<CEntity::toString(m_Smashed).toUtf8().data()
    <<", Rotation="<<m_Rotation<<","<<std::endl;
 CEntity::show(out,level); out<<"\tAttributes="<<std::endl;
 CEntity::show(out,level); out<<"\t{"<<std::endl;
 for (int i = 0; i < m_Attributes.size(); i++)
 {
  m_Attributes[i]->show(out,level+2);
 }
 CEntity::show(out,level); out<<"\t}"<<std::endl;
 CEntity::show(out,level); out<<"}"<<std::endl;
}

bool CInstance::readFromXML(const QDomElement& root)
{
 QDomElement e = root;
 if (e.nodeName()!="instance") e = e.nextSiblingElement("instance");
 if (!e.isNull())
 {
  QDomElement c = e.firstChildElement("attribute");
  while (!c.isNull())
  {
   CAttribute *a = new CAttribute();
   m_Attributes.append(a);
   a->readFromXML(c);
   c = c.nextSiblingElement("attribute");
  }
  QString s = e.attribute("part");
  if (!s.isEmpty())
  {
   m_Part = s;
  }
  s = e.attribute("gate");
  if (!s.isEmpty())
  {
   m_Gate = s;
  }
  s = e.attribute("x");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toDouble(&ok);
   if (ok) { m_X = value; }
  }
  s = e.attribute("y");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toDouble(&ok);
   if (ok) { m_Y = value; }
  }
  s = e.attribute("smashed");
  if (!s.isEmpty())
  {
   if (s=="yes") { m_Smashed = true; }
   else if (s=="no") { m_Smashed = false; }
  }
  s = e.attribute("rot");
  if (!s.isEmpty())
  {
   decodeTransformation(s,m_Rotation,m_Reflection,m_Spin);
   /*
   if (s.at(0)=='R')
   {
    bool ok = false;
    QString ss = s.right(s.length()-1);
    double value = ss.toDouble(&ok);
    if (ok) { m_Rotation = value; }
   }
   */
  }
  return true;
 }
 return false;
}

bool CInstance::writeToXML(QDomDocument& host, QDomElement& root, const bool defaults)
{
 if (!host.isNull() && !root.isNull())
 {
  QDomElement e = host.createElement("instance");
  e.setAttribute("part",m_Part);
  e.setAttribute("gate",m_Gate);
  e.setAttribute("x",QString("%1").arg(m_X));
  e.setAttribute("y",QString("%1").arg(m_Y));
  if (defaults || m_Smashed)
  {
   e.setAttribute("smashed",CEntity::toString(m_Smashed));
  }
  if (defaults || CAttribute::DEFAULT_ROTATION!=m_Rotation || m_Reflection || m_Spin)
  {
   //e.setAttribute("rot",QString("R%1").arg(m_Rotation));
   e.setAttribute("rot",encodeTransformation(m_Rotation,m_Reflection,m_Spin));
  }
  for (int i = 0; i < m_Attributes.size(); i++)
  {
   m_Attributes[i]->writeToXML(host,e,defaults);
  }
  root.appendChild(e);
  return true;
 }
 return false;
}

void CInstance::setRotation(const double value)
{
 if ((value<45.0) || (value>=315.0)) { m_Rotation = 0.0; }
 else if (value<135.0) { m_Rotation = 90.0; }
 else if (value<225.0) { m_Rotation = 180.0; }
 else if (value<315.0) { m_Rotation = 270.0; }
}

//------------------------------------------------------------------------------

CPart::CPart(const CPart& part)
{
 assign(part);
}

CPart::CPart(void)
{
 clear();
}

CPart::~CPart(void)
{
 clear();
}

void CPart::operator =(const CPart& part)
{
 assign(part);
}

void CPart::clear(void)
{
 for (int i = 0; i < m_Attributes.size(); i++)
 {
  if (0!=m_Attributes[i]) delete m_Attributes[i];
 }
 for (int i = 0; i < m_Variants.size(); i++)
 {
  if (0!=m_Variants[i]) delete m_Variants[i];
 }
 m_Attributes.clear();
 m_Variants.clear();
 m_Name.clear();
 m_Library.clear();
 m_DeviceSet.clear();
 m_Device.clear();
 m_Technology.clear();
 m_Value.clear();
}

void CPart::assign(const CPart& part)
{
 clear();
 for (int i = 0; i < part.m_Attributes.size(); i++)
 {
  CAttribute *a = new CAttribute(*part.m_Attributes.at(i));
  m_Attributes.append(a);
 }
 for (int i = 0; i < part.m_Variants.size(); i++)
 {
  CVariant *v = new CVariant(*part.m_Variants.at(i));
  m_Variants.append(v);
 }
 m_Name = part.m_Name;
 m_Library = part.m_Library;
 m_DeviceSet = part.m_DeviceSet;
 m_Device = part.m_Device;
 m_Technology = part.m_Technology;
 m_Value = part.m_Value;
}

void CPart::scale(const double factor)
{
 for (int i = 0; i < m_Attributes.size(); i++)
 {
  m_Attributes[i]->scale(factor);
 }
}

void CPart::show(std::ostream& out, const int level)
{
 CEntity::show(out,level);
 out<<"Part:{Name='"<<m_Name.toUtf8().data()<<"', Library='"<<m_Library.toUtf8().data()
    <<"', DeviceSet='"<<m_DeviceSet.toUtf8().data()<<"', Device='"<<m_Device.toUtf8().data()
    <<"', Technology='"<<m_Technology.toUtf8().data()<<"', Value='"<<m_Value.toUtf8().data()
    <<"',"<<std::endl;
 CEntity::show(out,level); out<<"\tAttributes"<<std::endl;
 CEntity::show(out,level); out<<"\t{"<<std::endl;
 for (int i = 0; i < m_Attributes.size(); i++)
 {
  m_Attributes[i]->show(out,level+1);
 }
 CEntity::show(out,level); out<<"\t}"<<std::endl;
 CEntity::show(out,level); out<<"\tVariants="<<std::endl;
 CEntity::show(out,level); out<<"\t{"<<std::endl;
 for (int i = 0; i < m_Variants.size(); i++)
 {
  m_Variants[i]->show(out,level+1);
 }
 CEntity::show(out,level); out<<"\t}"<<std::endl;
 CEntity::show(out,level); out<<"}"<<std::endl;
}

bool CPart::readFromXML(const QDomElement& root)
{
 QDomElement e = root;
 if (e.nodeName()!="part") e = e.nextSiblingElement("part");
 if (!e.isNull())
 {
  QDomElement c = e.firstChildElement("attibute");
  while (!c.isNull())
  {
   CAttribute *a = new CAttribute();
   m_Attributes.append(a);
   a->readFromXML(c);
   c = c.nextSiblingElement("attribute");
  }
  c = e.firstChildElement("variant");
  while (!c.isNull())
  {
   CVariant *v = new CVariant();
   m_Variants.append(v);
   v->readFromXML(c);
   c = c.nextSiblingElement("variant");
  }
  QString s = e.attribute("name");
  if (!s.isEmpty())
  {
   m_Name = s;
  }
  s = e.attribute("library");
  if (!s.isEmpty())
  {
   m_Library = s;
  }
  s = e.attribute("deviceset");
  if (!s.isEmpty())
  {
   m_DeviceSet = s;
  }
  s = e.attribute("device");
  if (!s.isEmpty())
  {
   m_Device = s;
  }
  s = e.attribute("technology");
  if (!s.isEmpty())
  {
   m_Technology = s;
  }
  s = e.attribute("value");
  if (!s.isEmpty())
  {
   m_Value = s;
  }
  return true;
 }
 return false;
}

bool CPart::writeToXML(QDomDocument& host, QDomElement& root, const bool defaults)
{
 if (!host.isNull() && !root.isNull())
 {
  QDomElement e = host.createElement("part");
  e.setAttribute("name",m_Name);
  e.setAttribute("library",m_Library);
  e.setAttribute("deviceset",m_DeviceSet);
  e.setAttribute("device",m_Device);
  if (defaults || !m_Technology.isEmpty())
  {
   e.setAttribute("technology",m_Technology);
  }
  if (defaults || !m_Value.isEmpty())
  {
   e.setAttribute("value",m_Value);
  }
  for (int i = 0; i < m_Attributes.size(); i++)
  {
   m_Attributes[i]->writeToXML(host,e,defaults);
  }
  for (int i = 0; i < m_Variants.size(); i++)
  {
   m_Variants[i]->writeToXML(host,e,defaults);
  }
  root.appendChild(e);
  return true;
 }
 return false;
}

//------------------------------------------------------------------------------

CPolygon::CPolygon(const QList<CVertex*>& vertices, const TDimension width,
 const TLayer layer, const TDimension spacing,
 const CPolygon::Pour pour, const TDimension isolate,
 const bool orphans, const bool thermals, const int rank)
{
 clear();
 m_Vertices.append(vertices);
 m_Width = width;
 m_Layer = layer;
 m_Spacing = spacing;
 m_Pour = pour;
 m_Isolate = isolate;
 m_Orphans = orphans;
 m_Thermals = thermals;
 m_Rank = rank;
}

CPolygon::CPolygon(const CPolygon& polygon)
{
 assign(polygon);
}

CPolygon::CPolygon(void)
{
 clear();
}

CPolygon::~CPolygon(void)
{
 clear();
}

QString CPolygon::toString(const CPolygon::Pour value)
{
 switch (value)
 {
  case CPolygon::ppSolid: return "solid";
  case CPolygon::ppHatch: return "hatch";
  case CPolygon::ppCutout: return "cutout";
 }
 return "";
}

void CPolygon::operator =(const CPolygon& polygon)
{
 assign(polygon);
}

void CPolygon::clear(void)
{
 for (int i = 0; i < m_Vertices.size(); i++)
 {
  if (0!=m_Vertices[i]) delete m_Vertices[i];
 }
 m_Vertices.clear();
 m_Width = 0.1;
 m_Layer = 1;
 m_Spacing = 0.1;
 m_Pour = CPolygon::ppSolid;
 m_Isolate = 0.0;
 m_Orphans = false;
 m_Thermals = true;
 m_Rank = 0;
}

void CPolygon::assign(const CPolygon& polygon)
{
 clear();
 for (int i = 0; i < polygon.m_Vertices.size(); i++)
 {
  CVertex *v = new CVertex(*polygon.m_Vertices.at(i));
  m_Vertices.append(v);
 }
 m_Width = polygon.m_Width;
 m_Layer = polygon.m_Layer;
 m_Spacing = polygon.m_Spacing;
 m_Pour = polygon.m_Pour;
 m_Isolate = polygon.m_Isolate;
 m_Orphans = polygon.m_Orphans;
 m_Thermals = polygon.m_Thermals;
 m_Rank = polygon.m_Rank;
}

void CPolygon::scale(const double factor)
{
 for (int i = 0; i < m_Vertices.size(); i++)
 {
  m_Vertices[i]->scale(factor);
 }
 m_Width *= factor;
 m_Spacing *= factor;
 m_Isolate *= factor;
}

void CPolygon::show(std::ostream& out, const int level)
{
 CEntity::show(out,level);
 out<<"Polygon:{Width="<<m_Width<<", Layer="<<m_Layer<<", Spacing="<<m_Spacing
    <<", Pour="<<toString(m_Pour).toUtf8().data()<<", Isolate="<<m_Isolate
    <<", Orphans="<<CEntity::toString(m_Orphans).toUtf8().data()
    <<", Thermals="<<CEntity::toString(m_Thermals).toUtf8().data()
    <<", Rank="<<m_Rank<<","<<std::endl;
 CEntity::show(out,level); out<<"\tVertices="<<std::endl;
 CEntity::show(out,level); out<<"\t{"<<std::endl;
 for (int i = 0; i < m_Vertices.size(); i++)
 {
  m_Vertices[i]->show(out,level+2);
 }
 CEntity::show(out,level); out<<"\t}"<<std::endl;
 CEntity::show(out,level); out<<"}"<<std::endl;
}

bool CPolygon::readFromXML(const QDomElement& root)
{
 QDomElement e = root;
 if (e.nodeName()!="polygon") e = e.nextSiblingElement("polygon");
 if (!e.isNull())
 {
  QDomElement c = e.firstChildElement("vertex");
  while (!c.isNull())
  {
   CVertex *v = new CVertex();
   m_Vertices.append(v);
   v->readFromXML(c);
   c = c.nextSiblingElement("vertex");
  }
  QString s = e.attribute("width");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toDouble(&ok);
   if (ok) m_Width = value;
  }
  s = e.attribute("layer");
  if (!s.isEmpty())
  {
   bool ok = false;
   int value = s.toInt(&ok);
   if (ok) m_Layer = value;
  }
  s = e.attribute("spacing");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toDouble(&ok);
   if (ok) m_Spacing = value;
  }
  s = e.attribute("pour");
  if (!s.isEmpty())
  {
   if (s=="solid") { m_Pour = CPolygon::ppSolid; }
   else if (s=="hatch") { m_Pour = CPolygon::ppHatch; }
   else if (s=="cutout") { m_Pour = CPolygon::ppCutout; }
  }
  s = e.attribute("isolate");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toDouble(&ok);
   if (ok) m_Isolate = value;
  }
  s = e.attribute("orphans");
  if (!s.isEmpty())
  {
   if (s=="yes") { m_Orphans = true; }
   else if (s=="no") { m_Orphans = false; }
  }
  s = e.attribute("thermals");
  if (!s.isEmpty())
  {
   if (s=="yes") { m_Thermals = true; }
   else if (s=="no") { m_Thermals = false; }
  }
  s = e.attribute("rank");
  if (!s.isEmpty())
  {
   bool ok = false;
   int value = s.toInt(&ok);
   if (ok) m_Rank = value;
  }
  return true;
 }
 return false;
}

bool CPolygon::writeToXML(QDomDocument& host, QDomElement& root, const bool defaults)
{
 if (!host.isNull() && !root.isNull())
 {
  QDomElement e = host.createElement("polygon");
  e.setAttribute("width",QString("%1").arg(m_Width));
  e.setAttribute("layer",QString("%1").arg(m_Layer));
  e.setAttribute("spacing",QString("%1").arg(m_Spacing));
  if (defaults || CPolygon::ppSolid!=m_Pour)
  {
   e.setAttribute("pour",toString(m_Pour));
  }
  e.setAttribute("isolate",QString("%1").arg(m_Isolate));
  if (defaults || m_Orphans)
  {
   e.setAttribute("orphans",CEntity::toString(m_Orphans));
  }
  if (defaults || !m_Thermals)
  {
   e.setAttribute("thermals",CEntity::toString(m_Thermals));
  }
  if (defaults || 0!=m_Rank)
  {
   e.setAttribute("rank",QString("%1").arg(m_Rank));
  }
  for (int i = 0; i < m_Vertices.size(); i++)
  {
   m_Vertices[i]->writeToXML(host,e,defaults);
  }
  root.appendChild(e);
  return true;
 }
 return false;
}

//------------------------------------------------------------------------------

CElement::CElement(const CElement& element)
{
 assign(element);
}

CElement::CElement(void)
{
 clear();
}

CElement::~CElement(void)
{
 clear();
}

void CElement::operator =(const CElement& element)
{
 assign(element);
}

void CElement::clear(void)
{
 for (int i = 0; i < m_Attributes.size(); i++)
 {
  if (0!=m_Attributes[i]) delete m_Attributes[i];
 }
 for (int i = 0; i < m_Variants.size(); i++)
 {
  if (0!=m_Variants[i]) delete m_Variants[i];
 }
 m_Attributes.clear();
 m_Variants.clear();
 m_Name.clear();
 m_Library.clear();
 m_Package.clear();
 m_Value.clear();
 m_X = 0.0;
 m_Y = 0.0;
 m_Locked = false;
 m_Smashed = false;
 m_Rotation = 0.0;
 m_Reflection = false;
}

void CElement::assign(const CElement& element)
{
 clear();
 for (int i = 0; i < element.m_Attributes.size(); i++)
 {
  CAttribute *a = new CAttribute(*element.m_Attributes.at(i));
  m_Attributes.append(a);
 }
 for (int i = 0; i < element.m_Variants.size(); i++)
 {
  CVariant *v = new CVariant(*element.m_Variants.at(i));
  m_Variants.append(v);
 }
 m_Name = element.m_Name;
 m_Library = element.m_Library;
 m_Package = element.m_Package;
 m_Value = element.m_Value;
 m_X = element.m_X;
 m_Y = element.m_Y;
 m_Locked = element.m_Locked;
 m_Smashed = element.m_Smashed;
 m_Rotation = element.m_Rotation;
 m_Reflection = element.m_Reflection;
}

void CElement::scale(const double factor)
{
 for (int i = 0; i < m_Attributes.size(); i++)
 {
  m_Attributes[i]->scale(factor);
 }
 m_X *= factor;
 m_Y *= factor;
}

void CElement::show(std::ostream& out, const int level)
{
 CEntity::show(out,level);
 out<<"Element:{Name='"<<m_Name.toUtf8().data()
    <<"', Library='"<<m_Library.toUtf8().data()
    <<"', Package='"<<m_Package.toUtf8().data()
    <<"', Value='"<<m_Value.toUtf8().data()
    <<"', X="<<m_X<<", Y="<<m_Y
    <<", Locked="<<CEntity::toString(m_Locked).toUtf8().data()
    <<", Smashed="<<CEntity::toString(m_Smashed).toUtf8().data()
    <<", Rotation="<<m_Rotation<<","<<std::endl;
 CEntity::show(out,level); out<<"\tAttributes="<<std::endl;
 CEntity::show(out,level); out<<"\t{"<<std::endl;
 for (int i = 0; i < m_Attributes.size(); i++)
 {
  m_Attributes[i]->show(out,level+2);
 }
 CEntity::show(out,level); out<<"\t}"<<std::endl;
 CEntity::show(out,level); out<<"\tVariants="<<std::endl;
 CEntity::show(out,level); out<<"\t{"<<std::endl;
 for (int i = 0; i < m_Variants.size(); i++)
 {
  m_Variants[i]->show(out,level+2);
 }
 CEntity::show(out,level); out<<"\t}"<<std::endl;
 CEntity::show(out,level); out<<"}"<<std::endl;
}

bool CElement::readFromXML(const QDomElement& root)
{
 QDomElement e = root;
 if (e.nodeName()!="element") e = e.nextSiblingElement("element");
 if (!e.isNull())
 {
  QDomElement c = e.firstChildElement("attribute");
  while (!c.isNull())
  {
   CAttribute *a = new CAttribute();
   m_Attributes.append(a);
   a->readFromXML(c);
   c = c.nextSiblingElement("attribute");
  }
  c = e.firstChildElement("variant");
  while (!c.isNull())
  {
   CVariant *v = new CVariant();
   m_Variants.append(v);
   v->readFromXML(c);
   c = c.nextSiblingElement("variant");
  }
  QString s = e.attribute("name");
  if (!s.isEmpty())
  {
   m_Name = s;
  }
  s = e.attribute("library");
  if (!s.isEmpty())
  {
   m_Library = s;
  }
  s = e.attribute("package");
  if (!s.isEmpty())
  {
   m_Package = s;
  }
  s = e.attribute("value");
  if (!s.isEmpty())
  {
   m_Value = s;
  }
  s = e.attribute("x");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toDouble(&ok);
   if (ok) { m_X = value; }
  }
  s = e.attribute("y");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toDouble(&ok);
   if (ok) { m_Y = value; }
  }
  s = e.attribute("locked");
  if (!s.isEmpty())
  {
   if (s=="yes") { m_Locked = true; }
   else if (s=="no") { m_Locked = false; }
  }
  s = e.attribute("smashed");
  if (!s.isEmpty())
  {
   if (s=="yes") { m_Smashed = true; }
   else if (s=="no") { m_Smashed = false; }
  }
  s = e.attribute("rot");
  if (!s.isEmpty())
  {
   bool spin;
   decodeTransformation(s,m_Rotation,m_Reflection,spin);
   /*
   if (s.at(0)=='R')
   {
    bool ok = false;
    QString ss = s.right(s.length()-1);
    double value = ss.toDouble(&ok);
    if (ok) { m_Rotation = value; }
   }
   */
  }
  return true;
 }
 return false;
}

bool CElement::writeToXML(QDomDocument& host, QDomElement& root, const bool defaults)
{
 if (!host.isNull() && !root.isNull())
 {
  QDomElement e = host.createElement("element");
  e.setAttribute("name",m_Name);
  e.setAttribute("library",m_Library);
  e.setAttribute("package",m_Package);
  e.setAttribute("value",m_Value);
  e.setAttribute("x",QString("%1").arg(m_X));
  e.setAttribute("y",QString("%1").arg(m_Y));
  if (defaults || m_Locked)
  {
   e.setAttribute("locked",CEntity::toString(m_Locked));
  }
  if (defaults || m_Smashed)
  {
   e.setAttribute("smashed",CEntity::toString(m_Smashed));
  }
  if (defaults || CElement::DEFAULT_ROTATION!=m_Rotation || m_Reflection)
  {
   //e.setAttribute("rot",QString("R%1").arg(m_Rotation));
   e.setAttribute("rot",encodeTransformation(m_Rotation,m_Reflection,false));
  }
  for (int i = 0; i < m_Attributes.size(); i++)
  {
   m_Attributes[i]->writeToXML(host,e,defaults);
  }
  for (int i = 0; i < m_Variants.size(); i++)
  {
   m_Variants[i]->writeToXML(host,e,defaults);
  }
  root.appendChild(e);
  return true;
 }
 return false;
}

//------------------------------------------------------------------------------

CSignal::CSignal(const CSignal& signal)
{
 assign(signal);
}

CSignal::CSignal(void)
{
 clear();
}

CSignal::~CSignal(void)
{
 clear();
}

void CSignal::operator =(const CSignal& signal)
{
 assign(signal);
}

void CSignal::clear(void)
{
 for (int i = 0; i < m_ContactRefs.size(); i++)
 {
  if (0!=m_ContactRefs[i]) delete m_ContactRefs[i];
 }
 for (int i = 0; i < m_Polygons.size(); i++)
 {
  if (0!=m_Polygons[i]) delete m_Polygons[i];
 }
 for (int i = 0; i < m_Wires.size(); i++)
 {
  if (0!=m_Wires[i]) delete m_Wires[i];
 }
 for (int i = 0; i < m_Vias.size(); i++)
 {
  if (0!=m_Vias[i]) delete m_Vias[i];
 }
 m_ContactRefs.clear();
 m_Polygons.clear();
 m_Wires.clear();
 m_Vias.clear();
 m_Name.clear();
 m_Class = 0;
 m_AirWiresHidden = false;
}

void CSignal::assign(const CSignal& signal)
{
 clear();
 for (int i = 0; i < signal.m_ContactRefs.size(); i++)
 {
  CContactRef *c = new CContactRef(*signal.m_ContactRefs.at(i));
  m_ContactRefs.append(c);
 }
 for (int i = 0; i < signal.m_Polygons.size(); i++)
 {
  CPolygon *p = new CPolygon(*signal.m_Polygons.at(i));
  m_Polygons.append(p);
 }
 for (int i = 0; i < signal.m_Wires.size(); i++)
 {
  CWire *w = new CWire(*signal.m_Wires.at(i));
  m_Wires.append(w);
 }
 for (int i = 0; i < signal.m_Vias.size(); i++)
 {
  CVia *v = new CVia(*signal.m_Vias.at(i));
  m_Vias.append(v);
 }
 m_Name.clear();
 m_Class = 0;
 m_AirWiresHidden = false;
}

void CSignal::scale(const double factor)
{
 for (int i = 0; i < m_Polygons.size(); i++)
 {
  m_Polygons[i]->scale(factor);
 }
 for (int i = 0; i < m_Wires.size(); i++)
 {
  m_Wires[i]->scale(factor);
 }
 for (int i = 0; i < m_Vias.size(); i++)
 {
  m_Vias[i]->scale(factor);
 }
}

void CSignal::show(std::ostream& out, const int level)
{
 CEntity::show(out,level); out<<"Signal:"<<std::endl;
 CEntity::show(out,level); out<<"{"<<std::endl;
 CEntity::show(out,level); out<<"\tName='"<<m_Name.toUtf8().data()
    <<"', Class="<<m_Class
    <<", AirWiresHidden="<<CEntity::toString(m_AirWiresHidden).toUtf8().data()
    <<std::endl;
 CEntity::show(out,level); out<<"\tContactRefs="<<std::endl;
 CEntity::show(out,level); out<<"\t{"<<std::endl;
 for (int i = 0; i < m_ContactRefs.size(); i++)
 {
  m_ContactRefs[i]->show(out,level+2);
 }
 CEntity::show(out,level); out<<"\t}"<<std::endl;
 CEntity::show(out,level); out<<"\tPolygons="<<std::endl;
 CEntity::show(out,level); out<<"\t{"<<std::endl;
 for (int i = 0; i < m_Polygons.size(); i++)
 {
  m_Polygons[i]->show(out,level+2);
 }
 CEntity::show(out,level); out<<"\t}"<<std::endl;
 CEntity::show(out,level); out<<"\tWires="<<std::endl;
 CEntity::show(out,level); out<<"\t{"<<std::endl;
 for (int i = 0; i < m_Wires.size(); i++)
 {
  m_Wires[i]->show(out,level+2);
 }
 CEntity::show(out,level); out<<"\t}"<<std::endl;
 CEntity::show(out,level); out<<"\tVias="<<std::endl;
 CEntity::show(out,level); out<<"\t{"<<std::endl;
 for (int i = 0; i < m_Vias.size(); i++)
 {
  m_Vias[i]->show(out,level+2);
 }
 CEntity::show(out,level); out<<"\t}"<<std::endl;
 CEntity::show(out,level); out<<"}"<<std::endl;
}

bool CSignal::readFromXML(const QDomElement& root)
{
 QDomElement e = root;
 if (e.nodeName()!="signal") e = e.nextSiblingElement("signal");
 if (!e.isNull())
 {
  QDomElement c = e.firstChildElement("contactref");
  while (!c.isNull())
  {
   CContactRef *r = new CContactRef();
   m_ContactRefs.append(r);
   r->readFromXML(c);
   c = c.nextSiblingElement("contactref");
  }
  c = e.firstChildElement("polygon");
  while (!c.isNull())
  {
   CPolygon *p = new CPolygon();
   m_Polygons.append(p);
   p->readFromXML(c);
   c = c.nextSiblingElement("polygon");
  }
  c = e.firstChildElement("wire");
  while (!c.isNull())
  {
   CWire *w = new CWire();
   m_Wires.append(w);
   w->readFromXML(c);
   c = c.nextSiblingElement("wire");
  }
  c = e.firstChildElement("via");
  while (!c.isNull())
  {
   CVia *v = new CVia();
   m_Vias.append(v);
   v->readFromXML(c);
   c = c.nextSiblingElement("via");
  }
  QString s = e.attribute("name");
  if (!s.isEmpty())
  {
   m_Name = s;
  }
  s = e.attribute("class");
  if (!s.isEmpty())
  {
   bool ok = false;
   int value = s.toInt(&ok);
   if (ok) { m_Class = value; }
  }
  s = e.attribute("airwireshidden");
  if (!s.isEmpty())
  {
   if (s=="yes") { m_AirWiresHidden = true; }
   else if (s=="no") { m_AirWiresHidden = false; }
  }
  return true;
 }
 return false;
}

bool CSignal::writeToXML(QDomDocument& host, QDomElement& root, const bool defaults)
{
 if (!host.isNull() && !root.isNull())
 {
  QDomElement e = host.createElement("signal");
  e.setAttribute("name",m_Name);
  if (defaults || CSignal::DEFAULT_CLASS!=m_Class)
  {
   e.setAttribute("class",QString("%1").arg(m_Class));
  }
  if (defaults || m_AirWiresHidden)
  {
   e.setAttribute("airwireshidden",CEntity::toString(m_AirWiresHidden));
  }
  for (int i = 0; i < m_ContactRefs.size(); i++)
  {
   m_ContactRefs[i]->writeToXML(host,e,defaults);
  }
  for (int i = 0; i < m_Polygons.size(); i++)
  {
   m_Polygons[i]->writeToXML(host,e,defaults);
  }
  for (int i = 0; i < m_Wires.size(); i++)
  {
   m_Wires[i]->writeToXML(host,e,defaults);
  }
  for (int i = 0; i < m_Vias.size(); i++)
  {
   m_Vias[i]->writeToXML(host,e,defaults);
  }
  root.appendChild(e);
  return true;
 }
 return false;
}

//------------------------------------------------------------------------------

CSegment::CSegment(const CSegment& segment)
{
 assign(segment);
}

CSegment::CSegment(void)
{
 clear();
}

CSegment::~CSegment(void)
{
 clear();
}

void CSegment::operator =(const CSegment& segment)
{
 assign(segment);
}

void CSegment::clear(void)
{
 for (int i = 0; i < m_PinRefs.size(); i++)
 {
  if (0!=m_PinRefs[i]) delete m_PinRefs[i];
 }
 for (int i = 0; i < m_Wires.size(); i++)
 {
  if (0!=m_Wires[i]) delete m_Wires[i];
 }
 for (int i = 0; i < m_Junctions.size(); i++)
 {
  if (0!=m_Junctions[i]) delete m_Junctions[i];
 }
 for (int i = 0; i < m_Labels.size(); i++)
 {
  if (0!=m_Labels[i]) delete m_Labels[i];
 }
 m_PinRefs.clear();
 m_Wires.clear();
 m_Junctions.clear();
 m_Labels.clear();
}

void CSegment::assign(const CSegment& segment)
{
 clear();
 for (int i = 0; i < segment.m_PinRefs.size(); i++)
 {
  CPinRef *r = new CPinRef(*segment.m_PinRefs.at(i));
  m_PinRefs.append(r);
 }
 for (int i = 0; i < segment.m_Wires.size(); i++)
 {
  CWire *w = new CWire(*segment.m_Wires.at(i));
  m_Wires.append(w);
 }
 for (int i = 0; i < segment.m_Junctions.size(); i++)
 {
  CJunction *j = new CJunction(*segment.m_Junctions.at(i));
  m_Junctions.append(j);
 }
 for (int i = 0; i < segment.m_Labels.size(); i++)
 {
  CLabel *l = new CLabel(*segment.m_Labels.at(i));
  m_Labels.append(l);
 }
}

void CSegment::scale(const double factor)
{
 for (int i = 0; i < m_Wires.size(); i++)
 {
  m_Wires[i]->scale(factor);
 }
 for (int i = 0; i < m_Junctions.size(); i++)
 {
  m_Junctions[i]->scale(factor);
 }
 for (int i = 0; i < m_Labels.size(); i++)
 {
  m_Labels[i]->scale(factor);
 }
}

void CSegment::show(std::ostream& out, const int level)
{
 CEntity::show(out,level); out<<"Segment:"<<std::endl;
 CEntity::show(out,level); out<<"{"<<std::endl;
 CEntity::show(out,level); out<<"\tPinRefs="<<std::endl;
 CEntity::show(out,level); out<<"\t{"<<std::endl;
 for (int i = 0; i < m_PinRefs.size(); i++)
 {
  m_PinRefs[i]->show(out,level+2);
 }
 CEntity::show(out,level); out<<"\t}"<<std::endl;
 CEntity::show(out,level); out<<"\tWires="<<std::endl;
 CEntity::show(out,level); out<<"\t{"<<std::endl;
 for (int i = 0; i < m_Wires.size(); i++)
 {
  m_Wires[i]->show(out,level+2);
 }
 CEntity::show(out,level); out<<"\t}"<<std::endl;
 CEntity::show(out,level); out<<"\tJunctions="<<std::endl;
 CEntity::show(out,level); out<<"\t{"<<std::endl;
 for (int i = 0; i < m_Junctions.size(); i++)
 {
  m_Junctions[i]->show(out,level+2);
 }
 CEntity::show(out,level); out<<"\t}"<<std::endl;
 CEntity::show(out,level); out<<"\tLabels="<<std::endl;
 CEntity::show(out,level); out<<"\t{"<<std::endl;
 for (int i = 0; i < m_Labels.size(); i++)
 {
  m_Labels[i]->show(out,level+2);
 }
 CEntity::show(out,level); out<<"\t}"<<std::endl;
 CEntity::show(out,level); out<<"}"<<std::endl;
}

bool CSegment::readFromXML(const QDomElement& root)
{
 QDomElement e = root;
 if (e.nodeName()!="segment") e = e.nextSiblingElement("segment");
 if (!e.isNull())
 {
  QDomElement c = e.firstChildElement("pinref");
  while (!c.isNull())
  {
   CPinRef *r = new CPinRef();
   m_PinRefs.append(r);
   r->readFromXML(c);
   c = c.nextSiblingElement("pinref");
  }
  c = e.firstChildElement("wire");
  while (!c.isNull())
  {
   CWire *w = new CWire();
   m_Wires.append(w);
   w->readFromXML(c);
   c = c.nextSiblingElement("wire");
  }
  c = e.firstChildElement("junction");
  while (!c.isNull())
  {
   CJunction *j = new CJunction();
   m_Junctions.append(j);
   j->readFromXML(c);
   c = c.nextSiblingElement("junction");
  }
  c = e.firstChildElement("label");
  while (!c.isNull())
  {
   CLabel *l = new CLabel();
   m_Labels.append(l);
   l->readFromXML(c);
   c = c.nextSiblingElement("label");
  }
  return true;
 }
 return false;
}

bool CSegment::writeToXML(QDomDocument& host, QDomElement& root, const bool defaults)
{
 if (!host.isNull() && !root.isNull())
 {
  QDomElement e = host.createElement("segment");
  for (int i = 0; i < m_PinRefs.size(); i++)
  {
   m_PinRefs[i]->writeToXML(host,e,defaults);
  }
  for (int i = 0; i < m_Wires.size(); i++)
  {
   m_Wires[i]->writeToXML(host,e,defaults);
  }
  for (int i = 0; i < m_Junctions.size(); i++)
  {
   m_Junctions[i]->writeToXML(host,e,defaults);
  }
  for (int i = 0; i < m_Labels.size(); i++)
  {
   m_Labels[i]->writeToXML(host,e,defaults);
  }
  root.appendChild(e);
  return true;
 }
 return false;
}

//------------------------------------------------------------------------------

CNet::CNet(const CNet& net)
{
 assign(net);
}

CNet::CNet(void)
{
 clear();
}

CNet::~CNet(void)
{
 clear();
}

void CNet::operator =(const CNet& net)
{
 assign(net);
}

void CNet::clear(void)
{
 for (int i = 0; i < m_Segments.size(); i++)
 {
  if (0!=m_Segments[i]) delete m_Segments[i];
 }
 m_Segments.clear();
 m_Name.clear();
 m_Class = 0;
}

void CNet::assign(const CNet& net)
{
 clear();
 for (int i = 0; i < net.m_Segments.size(); i++)
 {
  CSegment *s = new CSegment(*net.m_Segments.at(i));
  m_Segments.append(s);
 }
 m_Name = net.m_Name;
 m_Class = net.m_Class;
}

void CNet::scale(const double factor)
{
 for (int i = 0; i < m_Segments.size(); i++)
 {
  m_Segments[i]->scale(factor);
 }
}

void CNet::show(std::ostream& out, const int level)
{
 CEntity::show(out,level); out<<"Net:"<<std::endl;
 CEntity::show(out,level); out<<"{"<<std::endl;
 CEntity::show(out,level); out<<"\tName='"<<m_Name.toUtf8().data()
                              <<"' Class="<<m_Class<<", "<<std::endl;
 CEntity::show(out,level); out<<"\tSegments="<<std::endl;
 CEntity::show(out,level); out<<"\t{"<<std::endl;
 for (int i = 0; i < m_Segments.size(); i++)
 {
  m_Segments[i]->show(out,level+2);
 }
 CEntity::show(out,level); out<<"\t}"<<std::endl;
 CEntity::show(out,level); out<<"}"<<std::endl;
}

bool CNet::readFromXML(const QDomElement& root)
{
 QDomElement e = root;
 if (e.nodeName()!="net") e = e.nextSiblingElement("net");
 if (!e.isNull())
 {
  QDomElement c = e.firstChildElement("segment");
  while (!c.isNull())
  {
   CSegment *g = new CSegment();
   m_Segments.append(g);
   g->readFromXML(c);
   c = c.nextSiblingElement("segment");
  }
  QString s = e.attribute("name");
  if (!s.isEmpty())
  {
   m_Name = s;
  }
  s = e.attribute("class");
  if (!s.isEmpty())
  {
   bool ok = false;
   int value = s.toInt(&ok);
   if (ok) { m_Class = value; }
  }
  return true;
 }
 return false;
}

bool CNet::writeToXML(QDomDocument& host, QDomElement& root, const bool defaults)
{
 if (!host.isNull() && !root.isNull())
 {
  QDomElement e = host.createElement("net");
  e.setAttribute("name",m_Name);
  if (defaults || CNet::DEFAULT_CLASS!=m_Class)
  {
   e.setAttribute("class",QString("%1").arg(m_Class));
  }
  for (int i = 0; i < m_Segments.size(); i++)
  {
   m_Segments[i]->writeToXML(host,e,defaults);
  }
  root.appendChild(e);
  return true;
 }
 return false;
}

//------------------------------------------------------------------------------

CBus::CBus(const CBus& bus)
{
 assign(bus);
}

CBus::CBus(void)
{
 clear();
}

CBus::~CBus(void)
{
 clear();
}

void CBus::operator =(const CBus& bus)
{
 assign(bus);
}

void CBus::clear(void)
{
 for (int i = 0; i < m_Segments.size(); i++)
 {
  if (0!=m_Segments[i]) delete m_Segments[i];
 }
 m_Segments.clear();
 m_Name.clear();
}

void CBus::assign(const CBus& bus)
{
 clear();
 for (int i = 0; i < bus.m_Segments.size(); i++)
 {
  CSegment *s = new CSegment(*bus.m_Segments.at(i));
  m_Segments.append(s);
 }
 m_Name = bus.m_Name;
}

void CBus::scale(const double factor)
{
 for (int i = 0; i < m_Segments.size(); i++)
 {
  m_Segments[i]->scale(factor);
 }
}

void CBus::show(std::ostream& out, const int level)
{
 CEntity::show(out,level);
 out<<"Bus:{Name='"<<m_Name.toUtf8().data()<<"', "<<std::endl;
 CEntity::show(out,level); out<<"\tSegments="<<std::endl;
 CEntity::show(out,level); out<<"\t{"<<std::endl;
 for (int i = 0; i < m_Segments.size(); i++)
 {
  m_Segments[i]->show(out,level+1);
 }
 CEntity::show(out,level); out<<"\t}"<<std::endl;
 CEntity::show(out,level); out<<"}"<<std::endl;
}

bool CBus::readFromXML(const QDomElement& root)
{
 QDomElement e = root;
 if (e.nodeName()!="bus") e = e.nextSiblingElement("bus");
 if (!e.isNull())
 {
  QDomElement c = e.firstChildElement("segment");
  while (!c.isNull())
  {
   CSegment *g = new CSegment();
   m_Segments.append(g);
   g->readFromXML(c);
   c = c.nextSiblingElement("segment");
  }
  QString s = e.attribute("name");
  if (!s.isEmpty())
  {
   m_Name = s;
  }
  return true;
 }
 return false;
}

bool CBus::writeToXML(QDomDocument& host, QDomElement& root, const bool defaults)
{
 if (!host.isNull() && !root.isNull())
 {
  QDomElement e = host.createElement("bus");
  e.setAttribute("name",m_Name);
  for (int i = 0; i < m_Segments.size(); i++)
  {
   m_Segments[i]->writeToXML(host,e,defaults);
  }
  root.appendChild(e);
  return true;
 }
 return false;
}

//------------------------------------------------------------------------------

CDevice::CDevice(const QString& name, const QString& package)
{
 setName(name);
 setPackage(package);
}

CDevice::CDevice(const CDevice& device)
{
 assign(device);
}

CDevice::CDevice(void)
{
 clear();
}

CDevice::~CDevice(void)
{
 clear();
}

void CDevice::operator =(const CDevice& device)
{
 assign(device);
}

void CDevice::clear(void)
{
 for (int i = 0; i < m_Connects.size(); i++)
 {
  if (0!=m_Connects[i]) delete m_Connects[i];
 }
 for (int i = 0; i < m_Technologies.size(); i++)
 {
  if (0!=m_Technologies[i]) delete m_Technologies[i];
 }
 m_Connects.clear();
 m_Technologies.clear();
 m_Name.clear();
 m_Package.clear();
}

void CDevice::assign(const CDevice& device)
{
 clear();
 for (int i = 0; i < device.m_Connects.size(); i++)
 {
  CConnect *c = new CConnect(*device.m_Connects.at(i));
  m_Connects.append(c);
 }
 for (int i = 0; i < device.m_Technologies.size(); i++)
 {
  CTechnology *t = new CTechnology(*device.m_Technologies.at(i));
  m_Technologies.append(t);
 }
 m_Name = device.m_Name;
 m_Package = device.m_Package;
}

void CDevice::scale(const double factor)
{
 for (int i = 0; i < m_Technologies.size(); i++)
 {
  m_Technologies[i]->scale(factor);
 }
}

void CDevice::show(std::ostream& out, const int level)
{
 CEntity::show(out,level); out<<"Device:"<<std::endl;
 CEntity::show(out,level); out<<"{"<<std::endl;
 CEntity::show(out,level); out<<"\tName='"<<m_Name.toUtf8().data()
    <<"', Package='"<<m_Package.toUtf8().data()<<"', "<<std::endl;
 CEntity::show(out,level); out<<"\tGates="<<std::endl;
 CEntity::show(out,level); out<<"\t{"<<std::endl;
 for (int i = 0; i < m_Connects.size(); i++)
 {
  m_Connects[i]->show(out,level+2);
 }
 CEntity::show(out,level); out<<"\t}"<<std::endl;
 CEntity::show(out,level); out<<"\tTechnologies="<<std::endl;
 CEntity::show(out,level); out<<"\t{"<<std::endl;
 for (int i = 0; i < m_Technologies.size(); i++)
 {
  m_Technologies[i]->show(out,level+2);
 }
 CEntity::show(out,level); out<<"\t}"<<std::endl;
 CEntity::show(out,level); out<<"}"<<std::endl;
}

bool CDevice::readFromXML(const QDomElement& root)
{
 QDomElement e = root;
 if (e.nodeName()!="device") e = e.nextSiblingElement("device");
 if (!e.isNull())
 {
  QDomElement c = e.firstChildElement("connects");
  if (!c.isNull())
  {
   QDomElement cc = c.firstChildElement("connect");
   while (!cc.isNull())
   {
    CConnect *n = new CConnect();
    m_Connects.append(n);
    n->readFromXML(cc);
    cc = cc.nextSiblingElement("connect");
   }
  }
  c = e.firstChildElement("technologies");
  if (!c.isNull())
  {
   QDomElement cc = c.firstChildElement("technology");
   while (!cc.isNull())
   {
    CTechnology *t = new CTechnology();
    m_Technologies.append(t);
    t->readFromXML(cc);
    cc = cc.nextSiblingElement("technology");
   }
  }
  QString s = e.attribute("name");
  if (!s.isEmpty())
  {
   m_Name = s;
  }
  s = e.attribute("package");
  if (!s.isEmpty())
  {
   m_Package = s;
  }
  return true;
 }
 return false;
}

bool CDevice::writeToXML(QDomDocument& host, QDomElement& root, const bool defaults)
{
 if (!host.isNull() && !root.isNull())
 {
  QDomElement e = host.createElement("device");
  e.setAttribute("name",m_Name);
  if (defaults || !m_Package.isEmpty())
  {
   e.setAttribute("package",m_Package);
  }
  QDomElement c = host.createElement("connects");
  e.appendChild(c);
  for (int i = 0; i < m_Connects.size(); i++)
  {
   m_Connects[i]->writeToXML(host,c,defaults);
  }
  c = host.createElement("technologies");
  e.appendChild(c);
  for (int i = 0; i < m_Technologies.size(); i++)
  {
   m_Technologies[i]->writeToXML(host,c,defaults);
  }
  root.appendChild(e);
  return true;
 }
 return false;
}

//------------------------------------------------------------------------------

CDeviceSet::CDeviceSet(const QString& name, const QString& prefix, const bool userValue)
{
 setName(name);
 setPrefix(prefix);
 setUserValue(userValue);
}

CDeviceSet::CDeviceSet(const CDeviceSet& deviceSet)
{
 assign(deviceSet);
}

CDeviceSet::CDeviceSet(void)
{
 clear();
}

CDeviceSet::~CDeviceSet(void)
{
 clear();
}

void CDeviceSet::operator =(const CDeviceSet& deviceSet)
{
 assign(deviceSet);
}

void CDeviceSet::clear(void)
{
 m_Description.clear();
 for (int i = 0; i < m_Gates.size(); i++)
 {
  if (0!=m_Gates[i]) delete m_Gates[i];
 }
 for (int i = 0; i < m_Devices.size(); i++)
 {
  if (0!=m_Devices[i]) delete m_Devices[i];
 }
 m_Gates.clear();
 m_Devices.clear();
 m_Name.clear();
 m_Prefix.clear();
 m_UserValue = false;
}

void CDeviceSet::assign(const CDeviceSet& deviceSet)
{
 m_Description = deviceSet.m_Description;
 for (int i = 0; i < deviceSet.m_Gates.size(); i++)
 {
  CGate *g = new CGate(*deviceSet.m_Gates.at(i));
  m_Gates.append(g);
 }
 for (int i = 0; i < deviceSet.m_Devices.size(); i++)
 {
  CDevice *d = new CDevice(*deviceSet.m_Devices.at(i));
  m_Devices.append(d);
 }
 m_Name = deviceSet.m_Name;
 m_Prefix = deviceSet.m_Prefix;
 m_UserValue = deviceSet.m_UserValue;
}

void CDeviceSet::scale(const double factor)
{
 for (int i = 0; i < m_Gates.size(); i++)
 {
  m_Gates[i]->scale(factor);
 }
 for (int i = 0; i < m_Devices.size(); i++)
 {
  m_Devices[i]->scale(factor);
 }
}

void CDeviceSet::show(std::ostream& out, const int level)
{
 CEntity::show(out,level); out<<"DeviceSet:"<<std::endl;
 CEntity::show(out,level); out<<"{"<<std::endl;
 CEntity::show(out,level); out<<"\tName='"<<m_Name.toUtf8().data()
    <<"', Prefix='"<<m_Prefix.toUtf8().data()
    <<"', UserValue="<<CEntity::toString(m_UserValue).toUtf8().data()
    <<std::endl;
 m_Description.show(out,level+1);
 CEntity::show(out,level); out<<"\tGates="<<std::endl;
 CEntity::show(out,level); out<<"\t{"<<std::endl;
 for (int i = 0; i < m_Gates.size(); i++)
 {
  m_Gates[i]->show(out,level+2);
 }
 CEntity::show(out,level); out<<"\t}"<<std::endl;
 CEntity::show(out,level); out<<"\tDevices="<<std::endl;
 CEntity::show(out,level); out<<"\t{"<<std::endl;
 for (int i = 0; i < m_Devices.size(); i++)
 {
  m_Devices[i]->show(out,level+2);
 }
 CEntity::show(out,level); out<<"\t}"<<std::endl;
 CEntity::show(out,level); out<<"}"<<std::endl;
}

bool CDeviceSet::readFromXML(const QDomElement& root)
{
 QDomElement e = root;
 if (e.nodeName()!="deviceset") e = e.nextSiblingElement("deviceset");
 if (!e.isNull())
 {
  QDomElement c = e.firstChildElement("description");
  if (!c.isNull())
  {
   m_Description.readFromXML(c);
  }
  c = e.firstChildElement("gates");
  if (!c.isNull())
  {
   QDomElement cc = c.firstChildElement("gate");
   while (!cc.isNull())
   {
    CGate *g = new CGate();
    m_Gates.append(g);
    g->readFromXML(cc);
    cc = cc.nextSiblingElement("gate");
   }
  }
  c = e.firstChildElement("devices");
  if (!c.isNull())
  {
   QDomElement cc = c.firstChildElement("device");
   while (!cc.isNull())
   {
    CDevice *d = new CDevice();
    m_Devices.append(d);
    d->readFromXML(cc);
    cc = cc.nextSiblingElement("device");
   }
  }
  QString s = e.attribute("name");
  if (!s.isEmpty())
  {
   m_Name = s;
  }
  s = e.attribute("prefix");
  if (!s.isEmpty())
  {
   m_Prefix = s;
  }
  s = e.attribute("uservalue");
  if (!s.isEmpty())
  {
   if (s=="yes") { m_UserValue = true; }
   else if (s=="no") { m_UserValue = false; }
  }
 }
 return true;
}

bool CDeviceSet::writeToXML(QDomDocument& host, QDomElement& root, const bool defaults)
{
 if (!host.isNull() && !root.isNull())
 {
  QDomElement e = host.createElement("deviceset");
  e.setAttribute("name",m_Name);
  if (defaults || !m_Prefix.isEmpty())
  {
   e.setAttribute("prefix",m_Prefix);
  }
  if (defaults || m_UserValue)
  {
   e.setAttribute("uservalue",CEntity::toString(m_UserValue));
  }
  m_Description.writeToXML(host,e,defaults);
  QDomElement c = host.createElement("gates");
  e.appendChild(c);
  for (int i = 0; i < m_Gates.size(); i++)
  {
   m_Gates[i]->writeToXML(host,c,defaults);
  }
  c = host.createElement("devices");
  e.appendChild(c);
  for (int i = 0; i < m_Devices.size(); i++)
  {
   m_Devices[i]->writeToXML(host,c,defaults);
  }
  root.appendChild(e);
  return true;
 }
 return false;
}

CGate *CDeviceSet::findGateByName(const QString& name)
{
 CGate *result = 0;
 for (size_t i = 0; i < m_Gates.size(); i++)
 {
  CGate *g = m_Gates.at(i);
  if (g->name()==name)
  {
   result = g;
   break;
  }
 }
 return result;
}

CDevice *CDeviceSet::findDeviceByName(const QString& name)
{
 CDevice *result = 0;
 for (size_t i = 0; i < m_Devices.size(); i++)
 {
  CDevice *d = m_Devices.at(i);
  if (d->name()==name)
  {
   result = d;
   break;
  }
 }
 return result;
}


//------------------------------------------------------------------------------

CSymbol::CSymbol(const QString& name)
{
 m_Name = name;
}

CSymbol::CSymbol(const CSymbol& symbol)
{
 assign(symbol);
}

CSymbol::CSymbol(void)
{
 clear();
}

CSymbol::~CSymbol(void)
{
 clear();
}

void CSymbol::operator =(const CSymbol& symbol)
{
 assign(symbol);
}

void CSymbol::clear(void)
{
 m_Description.clear();
 for (int i = 0; i < m_Polygons.size(); i++)
 {
  if (0!=m_Polygons[i]) delete m_Polygons[i];
 }
 for (int i = 0; i < m_Wires.size(); i++)
 {
  if (0!=m_Wires[i]) delete m_Wires[i];
 }
 for (int i = 0; i < m_Texts.size(); i++)
 {
  if (0!=m_Texts[i]) delete m_Texts[i];
 }
 for (int i = 0; i < m_Dimensions.size(); i++)
 {
  if (0!=m_Dimensions[i]) delete m_Dimensions[i];
 }
 for (int i = 0; i < m_Pins.size(); i++)
 {
  if (0!=m_Pins[i]) delete m_Pins[i];
 }
 for (int i = 0; i < m_Circles.size(); i++)
 {
  if (0!=m_Circles[i]) delete m_Circles[i];
 }
 for (int i = 0; i < m_Rectangles.size(); i++)
 {
  if (0!=m_Rectangles[i]) delete m_Rectangles[i];
 }
 for (int i = 0; i < m_Frames.size(); i++)
 {
  if (0!=m_Frames[i]) delete m_Frames[i];
 }
 m_Polygons.clear();
 m_Wires.clear();
 m_Texts.clear();
 m_Dimensions.clear();
 m_Pins.clear();
 m_Circles.clear();
 m_Rectangles.clear();
 m_Frames.clear();
 m_Name.clear();
}

void CSymbol::assign(const CSymbol& symbol)
{
 clear();
 m_Description = symbol.m_Description;
 for (int i = 0; i < symbol.m_Polygons.size(); i++)
 {
  CPolygon *p = new CPolygon(*symbol.m_Polygons.at(i));
  m_Polygons.append(p);
 }
 for (int i = 0; i < symbol.m_Wires.size(); i++)
 {
  CWire *w = new CWire(*symbol.m_Wires.at(i));
  m_Wires.append(w);
 }
 for (int i = 0; i < symbol.m_Texts.size(); i++)
 {
  CText *t = new CText(*symbol.m_Texts.at(i));
  m_Texts.append(t);
 }
 for (int i = 0; i < symbol.m_Dimensions.size(); i++)
 {
  CDimension *d = new CDimension(*symbol.m_Dimensions.at(i));
  m_Dimensions.append(d);
 }
 for (int i = 0; i < symbol.m_Pins.size(); i++)
 {
  CPin *p = new CPin(*symbol.m_Pins.at(i));
  m_Pins.append(p);
 }
 for (int i = 0; i < symbol.m_Circles.size(); i++)
 {
  CCircle *c = new CCircle(*symbol.m_Circles.at(i));
  m_Circles.append(c);
 }
 for (int i = 0; i < symbol.m_Rectangles.size(); i++)
 {
  CRectangle *r = new CRectangle(*symbol.m_Rectangles.at(i));
  m_Rectangles.append(r);
 }
 for (int i = 0; i < symbol.m_Frames.size(); i++)
 {
  CFrame *f = new CFrame(*symbol.m_Frames.at(i));
  m_Frames.append(f);
 }
 m_Name = symbol.m_Name;
}

void CSymbol::scale(const double factor)
{
 for (int i = 0; i < m_Polygons.size(); i++)
 {
  m_Polygons[i]->scale(factor);
 }
 for (int i = 0; i < m_Wires.size(); i++)
 {
  m_Wires[i]->scale(factor);
 }
 for (int i = 0; i < m_Texts.size(); i++)
 {
  m_Texts[i]->scale(factor);
 }
 for (int i = 0; i < m_Dimensions.size(); i++)
 {
  m_Dimensions[i]->scale(factor);
 }
 for (int i = 0; i < m_Pins.size(); i++)
 {
  m_Pins[i]->scale(factor);
 }
 for (int i = 0; i < m_Circles.size(); i++)
 {
  m_Circles[i]->scale(factor);
 }
 for (int i = 0; i < m_Rectangles.size(); i++)
 {
  m_Rectangles[i]->scale(factor);
 }
 for (int i = 0; i < m_Frames.size(); i++)
 {
  m_Frames[i]->scale(factor);
 }
}

void CSymbol::show(std::ostream& out, const int level)
{
 CEntity::show(out,level); out<<"Symbol:"<<std::endl;
 CEntity::show(out,level); out<<"{"<<std::endl;
 CEntity::show(out,level); out<<"\tName='"<<m_Name.toUtf8().data()<<"'"<<std::endl;
 m_Description.show(out,level+1);
 CEntity::show(out,level); out<<"\t}"<<std::endl;
 CEntity::show(out,level); out<<"\tPolygons="<<std::endl;
 CEntity::show(out,level); out<<"\t{"<<std::endl;
 for (int i = 0; i < m_Polygons.size(); i++)
 {
  m_Polygons[i]->show(out,level+2);
 }
 CEntity::show(out,level); out<<"\t}"<<std::endl;
 CEntity::show(out,level); out<<"\tWires="<<std::endl;
 CEntity::show(out,level); out<<"\t{"<<std::endl;
 for (int i = 0; i < m_Wires.size(); i++)
 {
  m_Wires[i]->show(out,level+2);
 }
 CEntity::show(out,level); out<<"\t}"<<std::endl;
 CEntity::show(out,level); out<<"\tTexts="<<std::endl;
 CEntity::show(out,level); out<<"\t{"<<std::endl;
 for (int i = 0; i < m_Texts.size(); i++)
 {
  m_Texts[i]->show(out,level+2);
 }
 CEntity::show(out,level); out<<"\t}"<<std::endl;
 CEntity::show(out,level); out<<"\tDimensions="<<std::endl;
 CEntity::show(out,level); out<<"\t{"<<std::endl;
 for (int i = 0; i < m_Dimensions.size(); i++)
 {
  m_Dimensions[i]->show(out,level+2);
 }
 CEntity::show(out,level); out<<"\t}"<<std::endl;
 CEntity::show(out,level); out<<"\tPins="<<std::endl;
 CEntity::show(out,level); out<<"\t{"<<std::endl;
 for (int i = 0; i < m_Pins.size(); i++)
 {
  m_Pins[i]->show(out,level+2);
 }
 CEntity::show(out,level); out<<"\t}"<<std::endl;
 CEntity::show(out,level); out<<"\tCircles="<<std::endl;
 CEntity::show(out,level); out<<"\t{"<<std::endl;
 for (int i = 0; i < m_Circles.size(); i++)
 {
  m_Circles[i]->show(out,level+2);
 }
 CEntity::show(out,level); out<<"\t}"<<std::endl;
 CEntity::show(out,level); out<<"\tRectangles="<<std::endl;
 CEntity::show(out,level); out<<"\t{"<<std::endl;
 for (int i = 0; i < m_Rectangles.size(); i++)
 {
  m_Rectangles[i]->show(out,level+2);
 }
 CEntity::show(out,level); out<<"\t}"<<std::endl;
 CEntity::show(out,level); out<<"\tFrames="<<std::endl;
 CEntity::show(out,level); out<<"\t{"<<std::endl;
 for (int i = 0; i < m_Frames.size(); i++)
 {
  m_Frames[i]->show(out,level+2);
 }
 CEntity::show(out,level); out<<"\t}"<<std::endl;
 CEntity::show(out,level); out<<"}"<<std::endl;
}

bool CSymbol::readFromXML(const QDomElement& root)
{
 QDomElement e = root;
 if (e.nodeName()!="symbol") e = e.nextSiblingElement("symbol");
 if (!e.isNull())
 {
  QDomElement c = e.firstChildElement("description");
  if (!c.isNull())
  {
   m_Description.readFromXML(c);
  }
  c = e.firstChildElement("polygon");
  while (!c.isNull())
  {
   CPolygon *p = new CPolygon();
   m_Polygons.append(p);
   p->readFromXML(c);
   c = c.nextSiblingElement("polygon");
  }
  c = e.firstChildElement("wire");
  while (!c.isNull())
  {
   CWire *w = new CWire();
   m_Wires.append(w);
   w->readFromXML(c);
   c = c.nextSiblingElement("wire");
  }
  c = e.firstChildElement("text");
  while (!c.isNull())
  {
   CText *t = new CText();
   m_Texts.append(t);
   t->readFromXML(c);
   c = c.nextSiblingElement("text");
  }
  c = e.firstChildElement("dimension");
  while (!c.isNull())
  {
   CDimension *d = new CDimension();
   m_Dimensions.append(d);
   d->readFromXML(c);
   c = c.nextSiblingElement("dimension");
  }
  c = e.firstChildElement("pin");
  while (!c.isNull())
  {
   CPin *p = new CPin();
   m_Pins.append(p);
   p->readFromXML(c);
   c = c.nextSiblingElement("pin");
  }
  c = e.firstChildElement("circle");
  while (!c.isNull())
  {
   CCircle *l = new CCircle();
   m_Circles.append(l);
   l->readFromXML(c);
   c = c.nextSiblingElement("circle");
  }
  c = e.firstChildElement("rectangle");
  while (!c.isNull())
  {
   CRectangle *r = new CRectangle();
   m_Rectangles.append(r);
   r->readFromXML(c);
   c = c.nextSiblingElement("rectangle");
  }
  c = e.firstChildElement("frame");
  while (!c.isNull())
  {
   CFrame *f = new CFrame();
   m_Frames.append(f);
   f->readFromXML(c);
   c = c.nextSiblingElement("frame");
  }
  QString s = e.attribute("name");
  if (!s.isEmpty())
  {
   m_Name = s;
  }
 }
 return true;
}

bool CSymbol::writeToXML(QDomDocument& host, QDomElement& root, const bool defaults)
{
 if (!host.isNull() && !root.isNull())
 {
  QDomElement e = host.createElement("symbol");
  e.setAttribute("name",m_Name);
  m_Description.writeToXML(host,e,defaults);
  for (int i = 0; i < m_Polygons.size(); i++)
  {
   m_Polygons[i]->writeToXML(host,e,defaults);
  }
  for (int i = 0; i < m_Wires.size(); i++)
  {
   m_Wires[i]->writeToXML(host,e,defaults);
  }
  for (int i = 0; i < m_Texts.size(); i++)
  {
   m_Texts[i]->writeToXML(host,e,defaults);
  }
  for (int i = 0; i < m_Dimensions.size(); i++)
  {
   m_Dimensions[i]->writeToXML(host,e,defaults);
  }
  for (int i = 0; i < m_Pins.size(); i++)
  {
   m_Pins[i]->writeToXML(host,e,defaults);
  }
  for (int i = 0; i < m_Circles.size(); i++)
  {
   m_Circles[i]->writeToXML(host,e,defaults);
  }
  for (int i = 0; i < m_Rectangles.size(); i++)
  {
   m_Rectangles[i]->writeToXML(host,e,defaults);
  }
  for (int i = 0; i < m_Frames.size(); i++)
  {
   m_Frames[i]->writeToXML(host,e,defaults);
  }
  root.appendChild(e);
  return true;
 }
 return false;
}

//------------------------------------------------------------------------------

CPackage::CPackage(const CPackage& package)
{
 assign(package);
}

CPackage::CPackage(void)
{
 clear();
}

CPackage::~CPackage(void)
{
 clear();
}

void CPackage::operator =(const CPackage& package)
{
 assign(package);
}

void CPackage::clear(void)
{
 m_Description.clear();
 for (int i = 0; i < m_Polygons.size(); i++)
 {
  if (0!=m_Polygons[i]) delete m_Polygons[i];
 }
 for (int i = 0; i < m_Wires.size(); i++)
 {
  if (0!=m_Wires[i]) delete m_Wires[i];
 }
 for (int i = 0; i < m_Texts.size(); i++)
 {
  if (0!=m_Texts[i]) delete m_Texts[i];
 }
 for (int i = 0; i < m_Dimensions.size(); i++)
 {
  if (0!=m_Dimensions[i]) delete m_Dimensions[i];
 }
 for (int i = 0; i < m_Circles.size(); i++)
 {
  if (0!=m_Circles[i]) delete m_Circles[i];
 }
 for (int i = 0; i < m_Rectangles.size(); i++)
 {
  if (0!=m_Rectangles[i]) delete m_Rectangles[i];
 }
 for (int i = 0; i < m_Frames.size(); i++)
 {
  if (0!=m_Frames[i]) delete m_Frames[i];
 }
 for (int i = 0; i < m_Holes.size(); i++)
 {
  if (0!=m_Holes[i]) delete m_Holes[i];
 }
 for (int i = 0; i < m_Pads.size(); i++)
 {
  if (0!=m_Pads[i]) delete m_Pads[i];
 }
 for (int i = 0; i < m_SMDs.size(); i++)
 {
  if (0!=m_SMDs[i]) delete m_SMDs[i];
 }
 m_Polygons.clear();
 m_Wires.clear();
 m_Texts.clear();
 m_Dimensions.clear();
 m_Circles.clear();
 m_Rectangles.clear();
 m_Frames.clear();
 m_Holes.clear();
 m_Pads.clear();
 m_SMDs.clear();
 m_Name.clear();
}

void CPackage::assign(const CPackage& package)
{
 clear();
 m_Description.clear();
 for (int i = 0; i < package.m_Polygons.size(); i++)
 {
  CPolygon *p = new CPolygon(*package.m_Polygons.at(i));
  m_Polygons.append(p);
 }
 for (int i = 0; i < package.m_Wires.size(); i++)
 {
  CWire *w = new CWire(*package.m_Wires.at(i));
  m_Wires.append(w);
 }
 for (int i = 0; i < package.m_Texts.size(); i++)
 {
  CText *t = new CText(*package.m_Texts.at(i));
  m_Texts.append(t);
 }
 for (int i = 0; i < package.m_Dimensions.size(); i++)
 {
  CDimension *d = new CDimension(*package.m_Dimensions.at(i));
  m_Dimensions.append(d);
 }
 for (int i = 0; i < package.m_Circles.size(); i++)
 {
  CCircle *c = new CCircle(*package.m_Circles.at(i));
  m_Circles.append(c);
 }
 for (int i = 0; i < package.m_Rectangles.size(); i++)
 {
  CRectangle *r = new CRectangle(*package.m_Rectangles.at(i));
  m_Rectangles.append(r);
 }
 for (int i = 0; i < package.m_Frames.size(); i++)
 {
  CFrame *f = new CFrame(*package.m_Frames.at(i));
  m_Frames.append(f);
 }
 for (int i = 0; i < package.m_Holes.size(); i++)
 {
  CHole *h = new CHole(*package.m_Holes.at(i));
  m_Holes.append(h);
 }
 for (int i = 0; i < package.m_Pads.size(); i++)
 {
  CPad *p = new CPad(*package.m_Pads.at(i));
  m_Pads.append(p);
 }
 for (int i = 0; i < package.m_SMDs.size(); i++)
 {
  CSMD *s = new CSMD(*package.m_SMDs.at(i));
  m_SMDs.append(s);
 }
 m_Name = package.m_Name;
}

void CPackage::scale(const double factor)
{
 for (int i = 0; i < m_Polygons.size(); i++)
 {
  m_Polygons[i]->scale(factor);
 }
 for (int i = 0; i < m_Wires.size(); i++)
 {
  m_Wires[i]->scale(factor);
 }
 for (int i = 0; i < m_Texts.size(); i++)
 {
  m_Texts[i]->scale(factor);
 }
 for (int i = 0; i < m_Dimensions.size(); i++)
 {
  m_Dimensions[i]->scale(factor);
 }
 for (int i = 0; i < m_Circles.size(); i++)
 {
  m_Circles[i]->scale(factor);
 }
 for (int i = 0; i < m_Rectangles.size(); i++)
 {
  m_Rectangles[i]->scale(factor);
 }
 for (int i = 0; i < m_Frames.size(); i++)
 {
  m_Frames[i]->scale(factor);
 }
 for (int i = 0; i < m_Holes.size(); i++)
 {
  m_Holes[i]->scale(factor);
 }
 for (int i = 0; i < m_Pads.size(); i++)
 {
  m_Pads[i]->scale(factor);
 }
 for (int i = 0; i < m_SMDs.size(); i++)
 {
  m_SMDs[i]->scale(factor);
 }
}

void CPackage::show(std::ostream& out, const int level)
{
 CEntity::show(out,level); out<<"Package:"<<std::endl;
 CEntity::show(out,level); out<<"{"<<std::endl;
 CEntity::show(out,level); out<<"\tName='"<<m_Name.toUtf8().data()<<"'"<<std::endl;
 m_Description.show(out,level+1);
 CEntity::show(out,level); out<<"\tPolygons="<<std::endl;
 CEntity::show(out,level); out<<"\t{"<<std::endl;
 for (int i = 0; i < m_Polygons.size(); i++)
 {
  m_Polygons[i]->show(out,level+2);
 }
 CEntity::show(out,level); out<<"\t}"<<std::endl;
 CEntity::show(out,level); out<<"\tWires="<<std::endl;
 CEntity::show(out,level); out<<"\t{"<<std::endl;
 for (int i = 0; i < m_Wires.size(); i++)
 {
  m_Wires[i]->show(out,level+2);
 }
 CEntity::show(out,level); out<<"\t}"<<std::endl;
 CEntity::show(out,level); out<<"\tTexts="<<std::endl;
 CEntity::show(out,level); out<<"\t{"<<std::endl;
 for (int i = 0; i < m_Texts.size(); i++)
 {
  m_Texts[i]->show(out,level+2);
 }
 CEntity::show(out,level); out<<"\t}"<<std::endl;
 CEntity::show(out,level); out<<"\tDimensions="<<std::endl;
 CEntity::show(out,level); out<<"\t{"<<std::endl;
 for (int i = 0; i < m_Dimensions.size(); i++)
 {
  m_Dimensions[i]->show(out,level+2);
 }
 CEntity::show(out,level); out<<"\t}"<<std::endl;
 CEntity::show(out,level); out<<"\tCircles="<<std::endl;
 CEntity::show(out,level); out<<"\t{"<<std::endl;
 for (int i = 0; i < m_Circles.size(); i++)
 {
  m_Circles[i]->show(out,level+2);
 }
 CEntity::show(out,level); out<<"\t}"<<std::endl;
 CEntity::show(out,level); out<<"\tRectangles="<<std::endl;
 CEntity::show(out,level); out<<"\t{"<<std::endl;
 for (int i = 0; i < m_Rectangles.size(); i++)
 {
  m_Rectangles[i]->show(out,level+2);
 }
 CEntity::show(out,level); out<<"\t}"<<std::endl;
 CEntity::show(out,level); out<<"\tFrames="<<std::endl;
 CEntity::show(out,level); out<<"\t{"<<std::endl;
 for (int i = 0; i < m_Frames.size(); i++)
 {
  m_Frames[i]->show(out,level+2);
 }
 CEntity::show(out,level); out<<"\t}"<<std::endl;
 CEntity::show(out,level); out<<"\tHoles="<<std::endl;
 CEntity::show(out,level); out<<"\t{"<<std::endl;
 for (int i = 0; i < m_Holes.size(); i++)
 {
  m_Holes[i]->show(out,level+2);
 }
 CEntity::show(out,level); out<<"\t}"<<std::endl;
 CEntity::show(out,level); out<<"\tPads="<<std::endl;
 CEntity::show(out,level); out<<"\t{"<<std::endl;
 for (int i = 0; i < m_Pads.size(); i++)
 {
  m_Pads[i]->show(out,level+2);
 }
 CEntity::show(out,level); out<<"\t}"<<std::endl;
 CEntity::show(out,level); out<<"\tSMDs="<<std::endl;
 CEntity::show(out,level); out<<"\t{"<<std::endl;
 for (int i = 0; i < m_SMDs.size(); i++)
 {
  m_SMDs[i]->show(out,level+2);
 }
 CEntity::show(out,level);
 out<<"\t}"<<std::endl;
 CEntity::show(out,level);
 out<<"}"<<std::endl;
}

bool CPackage::readFromXML(const QDomElement& root)
{
 QDomElement e = root;
 if (e.nodeName()!="package") e = e.nextSiblingElement("package");
 if (!e.isNull())
 {
  QDomElement c = e.firstChildElement("description");
  if (!c.isNull())
  {
   m_Description.readFromXML(c);
  }
  c = e.firstChildElement("polygon");
  while (!c.isNull())
  {
   CPolygon *p = new CPolygon();
   m_Polygons.append(p);
   p->readFromXML(c);
   c = c.nextSiblingElement("polygon");
  }
  c = e.firstChildElement("wire");
  while (!c.isNull())
  {
   CWire *w = new CWire();
   m_Wires.append(w);
   w->readFromXML(c);
   c = c.nextSiblingElement("wire");
  }
  c = e.firstChildElement("text");
  while (!c.isNull())
  {
   CText *t = new CText();
   m_Texts.append(t);
   t->readFromXML(c);
   c = c.nextSiblingElement("text");
  }
  c = e.firstChildElement("dimension");
  while (!c.isNull())
  {
   CDimension *d = new CDimension();
   m_Dimensions.append(d);
   d->readFromXML(c);
   c = c.nextSiblingElement("dimension");
  }
  c = e.firstChildElement("circle");
  while (!c.isNull())
  {
   CCircle *l = new CCircle();
   m_Circles.append(l);
   l->readFromXML(c);
   c = c.nextSiblingElement("circle");
  }
  c = e.firstChildElement("rectangle");
  while (!c.isNull())
  {
   CRectangle *r = new CRectangle();
   m_Rectangles.append(r);
   r->readFromXML(c);
   c = c.nextSiblingElement("rectangle");
  }
  c = e.firstChildElement("frame");
  while (!c.isNull())
  {
   CFrame *f = new CFrame();
   m_Frames.append(f);
   f->readFromXML(c);
   c = c.nextSiblingElement("frame");
  }
  c = e.firstChildElement("hole");
  while (!c.isNull())
  {
   CHole *h = new CHole();
   m_Holes.append(h);
   h->readFromXML(c);
   c = c.nextSiblingElement("hole");
  }
  c = e.firstChildElement("pad");
  while (!c.isNull())
  {
   CPad *p = new CPad();
   m_Pads.append(p);
   p->readFromXML(c);
   c = c.nextSiblingElement("pad");
  }
  c = e.firstChildElement("smd");
  while (!c.isNull())
  {
   CSMD *s = new CSMD();
   m_SMDs.append(s);
   s->readFromXML(c);
   c = c.nextSiblingElement("smd");
  }
  QString s = e.attribute("name");
  if (!s.isEmpty())
  {
   m_Name = s;
  }
 }
 return true;
}

bool CPackage::writeToXML(QDomDocument& host, QDomElement& root, const bool defaults)
{
 if (!host.isNull() && !root.isNull())
 {
  QDomElement e = host.createElement("package");
  e.setAttribute("name",m_Name);
  m_Description.writeToXML(host,e,defaults);
  for (int i = 0; i < m_Polygons.size(); i++)
  {
   m_Polygons[i]->writeToXML(host,e,defaults);
  }
  for (int i = 0; i < m_Wires.size(); i++)
  {
   m_Wires[i]->writeToXML(host,e,defaults);
  }
  for (int i = 0; i < m_Texts.size(); i++)
  {
   m_Texts[i]->writeToXML(host,e,defaults);
  }
  for (int i = 0; i < m_Dimensions.size(); i++)
  {
   m_Dimensions[i]->writeToXML(host,e,defaults);
  }
  for (int i = 0; i < m_Circles.size(); i++)
  {
   m_Circles[i]->writeToXML(host,e,defaults);
  }
  for (int i = 0; i < m_Rectangles.size(); i++)
  {
   m_Rectangles[i]->writeToXML(host,e,defaults);
  }
  for (int i = 0; i < m_Frames.size(); i++)
  {
   m_Frames[i]->writeToXML(host,e,defaults);
  }
  for (int i = 0; i < m_Holes.size(); i++)
  {
   m_Holes[i]->writeToXML(host,e,defaults);
  }
  for (int i = 0; i < m_Pads.size(); i++)
  {
   m_Pads[i]->writeToXML(host,e,defaults);
  }
  for (int i = 0; i < m_SMDs.size(); i++)
  {
   m_SMDs[i]->writeToXML(host,e,defaults);
  }
  root.appendChild(e);
  return true;
 }
 return false;
}

//------------------------------------------------------------------------------

CPlain::CPlain(const CPlain& plain)
{
 assign(plain);
}

CPlain::CPlain(void)
{
 clear();
}

CPlain::~CPlain(void)
{
 clear();
}

void CPlain::operator =(const CPlain& plain)
{
 assign(plain);
}

void CPlain::clear(void)
{
 for (int i = 0; i < m_Polygons.size(); i++)
 {
  if (0!=m_Polygons[i]) delete m_Polygons[i];
 }
 for (int i = 0; i < m_Wires.size(); i++)
 {
  if (0!=m_Wires[i]) delete m_Wires[i];
 }
 for (int i = 0; i < m_Texts.size(); i++)
 {
  if (0!=m_Texts[i]) delete m_Texts[i];
 }
 for (int i = 0; i < m_Dimensions.size(); i++)
 {
  if (0!=m_Dimensions[i]) delete m_Dimensions[i];
 }
 for (int i = 0; i < m_Circles.size(); i++)
 {
  if (0!=m_Circles[i]) delete m_Circles[i];
 }
 for (int i = 0; i < m_Rectangles.size(); i++)
 {
  if (0!=m_Rectangles[i]) delete m_Rectangles[i];
 }
 for (int i = 0; i < m_Frames.size(); i++)
 {
  if (0!=m_Frames[i]) delete m_Frames[i];
 }
 for (int i = 0; i < m_Holes.size(); i++)
 {
  if (0!=m_Holes[i]) delete m_Holes[i];
 }
 m_Polygons.clear();
 m_Wires.clear();
 m_Texts.clear();
 m_Dimensions.clear();
 m_Circles.clear();
 m_Rectangles.clear();
 m_Frames.clear();
 m_Holes.clear();
}

void CPlain::assign(const CPlain& plain)
{
 clear();
 for (int i = 0; i < plain.m_Polygons.size(); i++)
 {
  CPolygon *p = new CPolygon(*plain.m_Polygons.at(i));
  m_Polygons.append(p);
 }
 for (int i = 0; i < plain.m_Wires.size(); i++)
 {
  CWire *w = new CWire(*plain.m_Wires.at(i));
  m_Wires.append(w);
 }
 for (int i = 0; i < plain.m_Texts.size(); i++)
 {
  CText *t = new CText(*plain.m_Texts.at(i));
  m_Texts.append(t);
 }
 for (int i = 0; i < plain.m_Dimensions.size(); i++)
 {
  CDimension *d = new CDimension(*plain.m_Dimensions.at(i));
  m_Dimensions.append(d);
 }
 for (int i = 0; i < plain.m_Circles.size(); i++)
 {
  CCircle *c = new CCircle(*plain.m_Circles.at(i));
  m_Circles.append(c);
 }
 for (int i = 0; i < plain.m_Rectangles.size(); i++)
 {
  CRectangle *r = new CRectangle(*plain.m_Rectangles.at(i));
  m_Rectangles.append(r);
 }
 for (int i = 0; i < plain.m_Frames.size(); i++)
 {
  CFrame *f = new CFrame(*plain.m_Frames.at(i));
  m_Frames.append(f);
 }
 for (int i = 0; i < plain.m_Holes.size(); i++)
 {
  CHole *h = new CHole(*plain.m_Holes.at(i));
  m_Holes.append(h);
 }
}

void CPlain::scale(const double factor)
{
 for (int i = 0; i < m_Polygons.size(); i++)
 {
  m_Polygons[i]->scale(factor);
 }
 for (int i = 0; i < m_Wires.size(); i++)
 {
  m_Wires[i]->scale(factor);
 }
 for (int i = 0; i < m_Texts.size(); i++)
 {
  m_Texts[i]->scale(factor);
 }
 for (int i = 0; i < m_Dimensions.size(); i++)
 {
  m_Dimensions[i]->scale(factor);
 }
 for (int i = 0; i < m_Circles.size(); i++)
 {
  m_Circles[i]->scale(factor);
 }
 for (int i = 0; i < m_Rectangles.size(); i++)
 {
  m_Rectangles[i]->scale(factor);
 }
 for (int i = 0; i < m_Frames.size(); i++)
 {
  m_Frames[i]->scale(factor);
 }
 for (int i = 0; i < m_Holes.size(); i++)
 {
  m_Holes[i]->scale(factor);
 }
}

void CPlain::show(std::ostream& out, const int level)
{
 CEntity::show(out,level); out<<"Plain:"<<std::endl;
 CEntity::show(out,level); out<<"{"<<std::endl;
 CEntity::show(out,level); out<<"\tPolygons="<<std::endl;
 CEntity::show(out,level); out<<"\t{"<<std::endl;
 for (int i = 0; i < m_Polygons.size(); i++)
 {
  m_Polygons[i]->show(out,level+2);
 }
 CEntity::show(out,level); out<<"\t}"<<std::endl;
 CEntity::show(out,level); out<<"\tWires="<<std::endl;
 CEntity::show(out,level); out<<"\t{"<<std::endl;
 for (int i = 0; i < m_Wires.size(); i++)
 {
  m_Wires[i]->show(out,level+2);
 }
 CEntity::show(out,level); out<<"\t}"<<std::endl;
 CEntity::show(out,level); out<<"\tTexts="<<std::endl;
 CEntity::show(out,level); out<<"\t{"<<std::endl;
 for (int i = 0; i < m_Texts.size(); i++)
 {
  m_Texts[i]->show(out,level+2);
 }
 CEntity::show(out,level); out<<"\t}"<<std::endl;
 CEntity::show(out,level); out<<"\tDimensions="<<std::endl;
 CEntity::show(out,level); out<<"\t{"<<std::endl;
 for (int i = 0; i < m_Dimensions.size(); i++)
 {
  m_Dimensions[i]->show(out,level+2);
 }
 CEntity::show(out,level); out<<"\t}"<<std::endl;
 CEntity::show(out,level); out<<"\tCircles="<<std::endl;
 CEntity::show(out,level); out<<"\t{"<<std::endl;
 for (int i = 0; i < m_Circles.size(); i++)
 {
  m_Circles[i]->show(out,level+2);
 }
 CEntity::show(out,level); out<<"\t}"<<std::endl;
 CEntity::show(out,level); out<<"\tRectangles="<<std::endl;
 CEntity::show(out,level); out<<"\t{"<<std::endl;
 for (int i = 0; i < m_Rectangles.size(); i++)
 {
  m_Rectangles[i]->show(out,level+2);
 }
 CEntity::show(out,level); out<<"\t}"<<std::endl;
 CEntity::show(out,level); out<<"\tFrames="<<std::endl;
 CEntity::show(out,level); out<<"\t{"<<std::endl;
 for (int i = 0; i < m_Frames.size(); i++)
 {
  m_Frames[i]->show(out,level+2);
 }
 CEntity::show(out,level); out<<"\t}"<<std::endl;
 CEntity::show(out,level); out<<"\tHoles="<<std::endl;
 CEntity::show(out,level); out<<"\t{"<<std::endl;
 for (int i = 0; i < m_Holes.size(); i++)
 {
  m_Holes[i]->show(out,level+2);
 }
 CEntity::show(out,level); out<<"\t}"<<std::endl;
 CEntity::show(out,level); out<<"\tDimensions="<<std::endl;
 CEntity::show(out,level); out<<"\t{"<<std::endl;
 for (int i = 0; i < m_Dimensions.size(); i++)
 {
  m_Dimensions[i]->show(out,level+2);
 }
 CEntity::show(out,level); out<<"\t}"<<std::endl;
 CEntity::show(out,level); out<<"}"<<std::endl;
}

bool CPlain::readFromXML(const QDomElement& root)
{
 QDomElement e = root;
 if (e.nodeName()!="plain") e = e.nextSiblingElement("plain");
 if (!e.isNull())
 {
  QDomElement c = e.firstChildElement("polygon");
  while (!c.isNull())
  {
   CPolygon *p = new CPolygon();
   m_Polygons.append(p);
   p->readFromXML(c);
   c = c.nextSiblingElement("polygon");
  }
  c = e.firstChildElement("wire");
  while (!c.isNull())
  {
   CWire *w = new CWire();
   m_Wires.append(w);
   w->readFromXML(c);
   c = c.nextSiblingElement("wire");
  }
  c = e.firstChildElement("text");
  while (!c.isNull())
  {
   CText *t = new CText();
   m_Texts.append(t);
   t->readFromXML(c);
   c = c.nextSiblingElement("text");
  }
  c = e.firstChildElement("dimension");
  while (!c.isNull())
  {
   CDimension *d = new CDimension();
   m_Dimensions.append(d);
   d->readFromXML(c);
   c = c.nextSiblingElement("dimension");
  }
  c = e.firstChildElement("circle");
  while (!c.isNull())
  {
   CCircle *l = new CCircle();
   m_Circles.append(l);
   l->readFromXML(c);
   c = c.nextSiblingElement("circle");
  }
  c = e.firstChildElement("rectangle");
  while (!c.isNull())
  {
   CRectangle *r = new CRectangle();
   m_Rectangles.append(r);
   r->readFromXML(c);
   c = c.nextSiblingElement("rectangle");
  }
  c = e.firstChildElement("frame");
  while (!c.isNull())
  {
   CFrame *f = new CFrame();
   m_Frames.append(f);
   f->readFromXML(c);
   c = c.nextSiblingElement("frame");
  }
  c = e.firstChildElement("hole");
  while (!c.isNull())
  {
   CHole *h = new CHole();
   m_Holes.append(h);
   h->readFromXML(c);
   c = c.nextSiblingElement("hole");
  }
  c = e.firstChildElement("dimension");
  while (!c.isNull())
  {
   CDimension *d = new CDimension();
   m_Dimensions.append(d);
   d->readFromXML(c);
   c = c.nextSiblingElement("dimension");
  }
 }
 return true;
}

bool CPlain::writeToXML(QDomDocument& host, QDomElement& root, const bool defaults)
{
 if (!host.isNull() && !root.isNull())
 {
  QDomElement e = host.createElement("plain");
  for (int i = 0; i < m_Polygons.size(); i++)
  {
   m_Polygons[i]->writeToXML(host,e,defaults);
  }
  for (int i = 0; i < m_Wires.size(); i++)
  {
   m_Wires[i]->writeToXML(host,e,defaults);
  }
  for (int i = 0; i < m_Texts.size(); i++)
  {
   m_Texts[i]->writeToXML(host,e,defaults);
  }
  for (int i = 0; i < m_Dimensions.size(); i++)
  {
   m_Dimensions[i]->writeToXML(host,e,defaults);
  }
  for (int i = 0; i < m_Circles.size(); i++)
  {
   m_Circles[i]->writeToXML(host,e,defaults);
  }
  for (int i = 0; i < m_Rectangles.size(); i++)
  {
   m_Rectangles[i]->writeToXML(host,e,defaults);
  }
  for (int i = 0; i < m_Frames.size(); i++)
  {
   m_Frames[i]->writeToXML(host,e,defaults);
  }
  for (int i = 0; i < m_Holes.size(); i++)
  {
   m_Holes[i]->writeToXML(host,e,defaults);
  }
  for (int i = 0; i < m_Dimensions.size(); i++)
  {
   m_Dimensions[i]->writeToXML(host,e,defaults);
  }
  root.appendChild(e);
  return true;
 }
 return false;
}

//------------------------------------------------------------------------------

CSheet::CSheet(const CSheet& sheet)
{
 assign(sheet);
}

CSheet::CSheet(void)
{
 clear();
}

CSheet::~CSheet(void)
{
 clear();
}

void CSheet::operator =(const CSheet& sheet)
{
 assign(sheet);
}

void CSheet::clear(void)
{
 m_Description.clear();
 m_Plain.clear();
 for (int i = 0; i < m_Instances.size(); i++)
 {
  if (0!=m_Instances[i]) delete m_Instances[i];
 }
 for (int i = 0; i < m_Busses.size(); i++)
 {
  if (0!=m_Busses[i]) delete m_Busses[i];
 }
 for (int i = 0; i < m_Nets.size(); i++)
 {
  if (0!=m_Nets[i]) delete m_Nets[i];
 }
 m_Instances.clear();
 m_Busses.clear();
 m_Nets.clear();
}

void CSheet::assign(const CSheet& sheet)
{
 clear();
 m_Description = sheet.m_Description;
 m_Plain = sheet.m_Plain;
 for (int i = 0; i < sheet.m_Instances.size(); i++)
 {
  CInstance *n = new CInstance(*sheet.m_Instances.at(i));
  m_Instances.append(n);
 }
 for (int i = 0; i < sheet.m_Busses.size(); i++)
 {
  CBus *b = new CBus(*sheet.m_Busses.at(i));
  m_Busses.append(b);
 }
 for (int i = 0; i < sheet.m_Nets.size(); i++)
 {
  CNet *n = new CNet(*sheet.m_Nets.at(i));
  m_Nets.append(n);
 }
}

void CSheet::scale(const double factor)
{
 m_Plain.scale(factor);
 for (int i = 0; i < m_Instances.size(); i++)
 {
  m_Instances[i]->scale(factor);
 }
 for (int i = 0; i < m_Busses.size(); i++)
 {
  m_Busses[i]->scale(factor);
 }
 for (int i = 0; i < m_Nets.size(); i++)
 {
  m_Nets[i]->scale(factor);
 }
}

void CSheet::show(std::ostream& out, const int level)
{
 CEntity::show(out,level); out<<"Sheet:"<<std::endl;
 CEntity::show(out,level); out<<"{"<<std::endl;
 m_Description.show(out,level+1);
 m_Plain.show(out,level+1);
 CEntity::show(out,level); out<<"\tInstances="<<std::endl;
 CEntity::show(out,level); out<<"\t{"<<std::endl;
 for (int i = 0; i < m_Instances.size(); i++)
 {
  m_Instances[i]->show(out,level+2);
 }
 CEntity::show(out,level); out<<"\t}"<<std::endl;
 CEntity::show(out,level); out<<"\tBusses="<<std::endl;
 CEntity::show(out,level); out<<"\t{"<<std::endl;
 for (int i = 0; i < m_Busses.size(); i++)
 {
  m_Busses[i]->show(out,level+2);
 }
 CEntity::show(out,level); out<<"\t}"<<std::endl;
 CEntity::show(out,level); out<<"\tNets="<<std::endl;
 CEntity::show(out,level); out<<"\t{"<<std::endl;
 for (int i = 0; i < m_Nets.size(); i++)
 {
  m_Nets[i]->show(out,level+2);
 }
 CEntity::show(out,level); out<<"\t}"<<std::endl;
 CEntity::show(out,level); out<<"}"<<std::endl;
}

bool CSheet::readFromXML(const QDomElement& root)
{
 QDomElement e = root;
 if (e.nodeName()!="sheet") e = e.nextSiblingElement("sheet");
 if (!e.isNull())
 {
  QDomElement c = e.firstChildElement("description");
  if (!c.isNull())
  {
   m_Description.readFromXML(c);
  }
  c = e.firstChildElement("plain");
  if (!c.isNull())
  {
   m_Plain.readFromXML(c);
  }
  c = e.firstChildElement("instances");
  if (!c.isNull())
  {
   QDomElement cc = c.firstChildElement("instance");
   while (!cc.isNull())
   {
    CInstance *i = new CInstance();
    m_Instances.append(i);
    i->readFromXML(cc);
    cc = cc.nextSiblingElement("instance");
   }
  }
  c = e.firstChildElement("busses");
  if (!c.isNull())
  {
   QDomElement cc = c.firstChildElement("bus");
   while (!cc.isNull())
   {
    CBus *b = new CBus();
    m_Busses.append(b);
    b->readFromXML(cc);
    cc = cc.nextSiblingElement("bus");
   }
  }
  c = e.firstChildElement("nets");
  if (!c.isNull())
  {
   QDomElement cc = c.firstChildElement("net");
   while (!cc.isNull())
   {
    CNet *n = new CNet();
    m_Nets.append(n);
    n->readFromXML(cc);
    cc = cc.nextSiblingElement("net");
   }
  }
 }
 return true;
}

bool CSheet::writeToXML(QDomDocument& host, QDomElement& root, const bool defaults)
{
 if (!host.isNull() && !root.isNull())
 {
  QDomElement e = host.createElement("sheet");
  m_Description.writeToXML(host,e,defaults);
  m_Plain.writeToXML(host,e,defaults);
  QDomElement c = host.createElement("instances");
  e.appendChild(c);
  for (int i = 0; i < m_Instances.size(); i++)
  {
   m_Instances[i]->writeToXML(host,c,defaults);
  }
  c = host.createElement("busses");
  e.appendChild(c);
  for (int i = 0; i < m_Busses.size(); i++)
  {
   m_Busses[i]->writeToXML(host,c,defaults);
  }
  c = host.createElement("nets");
  e.appendChild(c);
  for (int i = 0; i < m_Nets.size(); i++)
  {
   m_Nets[i]->writeToXML(host,c,defaults);
  }
  root.appendChild(e);
  return true;
 }
 return false;
}

//------------------------------------------------------------------------------

CLibrary::CLibrary(const CLibrary& library)
{
}

CLibrary::CLibrary(void)
{
 clear();
}

CLibrary::~CLibrary(void)
{
 clear();
}

void CLibrary::operator =(const CLibrary& library)
{
 assign(library);
}

void CLibrary::clear(void)
{
 m_Description.clear();
 for (int i = 0; i < m_Packages.size(); i++)
 {
  if (0!=m_Packages[i]) delete m_Packages[i];
 }
 for (int i = 0; i < m_Symbols.size(); i++)
 {
  if (0!=m_Symbols[i]) delete m_Symbols[i];
 }
 for (int i = 0; i < m_DeviceSets.size(); i++)
 {
  if (0!=m_DeviceSets[i]) delete m_DeviceSets[i];
 }
 m_Packages.clear();
 m_Symbols.clear();
 m_DeviceSets.clear();
 m_Name.clear();
}

void CLibrary::assign(const CLibrary& library)
{
 clear();
 m_Description = library.m_Description;
 for (int i = 0; i < library.m_Packages.size(); i++)
 {
  CPackage *p = new CPackage(*library.m_Packages.at(i));
  m_Packages.append(p);
 }
 for (int i = 0; i < library.m_Symbols.size(); i++)
 {
  CSymbol *s = new CSymbol(*library.m_Symbols.at(i));
  m_Symbols.append(s);
 }
 for (int i = 0; i < library.m_DeviceSets.size(); i++)
 {
  CDeviceSet *d = new CDeviceSet(*library.m_DeviceSets.at(i));
  m_DeviceSets.append(d);
 }
 m_Name = library.m_Name;
}

void CLibrary::scalePackages(const double factor)
{
 for (int i = 0; i < m_Packages.size(); i++)
 {
  m_Packages[i]->scale(factor);
 }
}

void CLibrary::scaleSymbols(const double factor)
{
 for (int i = 0; i < m_Symbols.size(); i++)
 {
  m_Symbols[i]->scale(factor);
 }
}

void CLibrary::scale(const double factor)
{
 scalePackages(factor);
 scaleSymbols(factor);
 for (int i = 0; i < m_DeviceSets.size(); i++)
 {
  m_DeviceSets[i]->scale(factor);
 }
}

void CLibrary::show(std::ostream& out, const int level)
{
 CEntity::show(out,level); out<<"Library:"<<std::endl;
 CEntity::show(out,level); out<<"{"<<std::endl;
 CEntity::show(out,level); out<<"\tName='"<<m_Name.toUtf8().data()<<"'"<<std::endl;
 m_Description.show(out,level+1);
 CEntity::show(out,level); out<<"\tPackages="<<std::endl;
 CEntity::show(out,level); out<<"\t{"<<std::endl;
 for (int i = 0; i < m_Packages.size(); i++)
 {
  m_Packages[i]->show(out,level+2);
 }
 CEntity::show(out,level); out<<"\t}"<<std::endl;
 CEntity::show(out,level); out<<"\tSymbols="<<std::endl;
 CEntity::show(out,level); out<<"\t{"<<std::endl;
 for (int i = 0; i < m_Symbols.size(); i++)
 {
  m_Symbols[i]->show(out,level+2);
 }
 CEntity::show(out,level); out<<"\t}"<<std::endl;
 CEntity::show(out,level); out<<"\tDeviceSets="<<std::endl;
 CEntity::show(out,level); out<<"\t{"<<std::endl;
 for (int i = 0; i < m_DeviceSets.size(); i++)
 {
  m_DeviceSets[i]->show(out,level+2);
 }
 CEntity::show(out,level); out<<"\t}"<<std::endl;
 CEntity::show(out,level); out<<"}"<<std::endl;
}

bool CLibrary::readFromXML(const QDomElement& root)
{
 bool result = false;
 QDomElement e = root;
 if (e.nodeName()!="library") e = e.nextSiblingElement("library");
 if (!e.isNull())
 {
  result = true;
  QDomElement c = e.firstChildElement("description");
  if (!c.isNull())
  {
   /*result &=*/ m_Description.readFromXML(c);
  }
  c = e.firstChildElement("packages");
  if (!c.isNull())
  {
   QDomElement cc = c.firstChildElement("package");
   while (!cc.isNull())
   {
    CPackage *p = new CPackage();
    m_Packages.append(p);
    result &= p->readFromXML(cc);
    cc = cc.nextSiblingElement("package");
   }
  }
  c = e.firstChildElement("symbols");
  if (!c.isNull())
  {
   QDomElement cc = c.firstChildElement("symbol");
   while (!cc.isNull())
   {
    CSymbol *s = new CSymbol();
    m_Symbols.append(s);
    result &= s->readFromXML(cc);
    cc = cc.nextSiblingElement("symbol");
   }
  }
  c = e.firstChildElement("devicesets");
  if (!c.isNull())
  {
   QDomElement cc = c.firstChildElement("deviceset");
   while (!cc.isNull())
   {
    CDeviceSet *d = new CDeviceSet();
    m_DeviceSets.append(d);
    result &= d->readFromXML(cc);
    cc = cc.nextSiblingElement("deviceset");
   }
  }
  QString s = e.attribute("name");
  if (!s.isEmpty())
  {
   m_Name = s;
  }
 }
 return result;
}

bool CLibrary::writeToXML(QDomDocument& host, QDomElement& root, const bool defaults)
{
 if (!host.isNull() && !root.isNull())
 {
  QDomElement e = host.createElement("library");
  e.setAttribute("name",m_Name);
  m_Description.writeToXML(host,e,defaults);
  QDomElement c = host.createElement("packages");
  e.appendChild(c);
  for (int i = 0; i < m_Packages.size(); i++)
  {
   m_Packages[i]->writeToXML(host,c,defaults);
  }
  c = host.createElement("symbols");
  e.appendChild(c);
  for (int i = 0; i < m_Symbols.size(); i++)
  {
   m_Symbols[i]->writeToXML(host,c,defaults);
  }
  c = host.createElement("devicesets");
  e.appendChild(c);
  for (int i = 0; i < m_DeviceSets.size(); i++)
  {
   m_DeviceSets[i]->writeToXML(host,c,defaults);
  }
  root.appendChild(e);
  return true;
 }
 return false;
}

CPackage *CLibrary::findPackageByName(const QString& name)
{
 CPackage *result = 0;
 for (size_t i = 0; i < m_Packages.size(); i++)
 {
  CPackage *p = m_Packages.at(i);
  if (p->name()==name)
  {
   result = p;
   break;
  }
 }
 return result;
}

CSymbol *CLibrary::findSymbolByName(const QString& name)
{
 CSymbol *result = 0;
 for (size_t i = 0; i < m_Symbols.size(); i++)
 {
  CSymbol *s = m_Symbols.at(i);
  if (s->name()==name)
  {
   result = s;
   break;
  }
 }
 return result;
}

CDeviceSet *CLibrary::findDeviceSetByName(const QString& name)
{
 CDeviceSet *result = 0;
 for (size_t i = 0; i < m_DeviceSets.size(); i++)
 {
  CDeviceSet *s = m_DeviceSets.at(i);
  if (s->name()==name)
  {
   result = s;
   break;
  }
 }
 return result;
}

//------------------------------------------------------------------------------

CBoard::CBoard(const CBoard& board)
{
 assign(board);
}

CBoard::CBoard(void)
{
 clear();
}

CBoard::~CBoard(void)
{
 clear();
}

void CBoard::operator =(const CBoard& board)
{
 assign(board);
}

void CBoard::clear(void)
{
 m_Description.clear();
 m_Plain.clear();
 //
 for (int i = 0; i < m_Libraries.size(); i++)
 {
  if (0!=m_Libraries[i]) delete m_Libraries[i];
 }
 for (int i = 0; i < m_Attributes.size(); i++)
 {
  if (0!=m_Attributes[i]) delete m_Attributes[i];
 }
 for (int i = 0; i < m_VariantDefs.size(); i++)
 {
  if (0!=m_VariantDefs[i]) delete m_VariantDefs[i];
 }
 for (int i = 0; i < m_Classes.size(); i++)
 {
  if (0!=m_Classes[i]) delete m_Classes[i];
 }
 /*
 for (int i = 0; i < m_DesignRules.size(); i++)
 {
  if (0!=m_DesignRules[i]) delete m_DesignRules[i];
 }
 */
 // autorouter
 for (int i = 0; i < m_Passes.size(); i++)
 {
  if (0!=m_Passes[i]) delete m_Passes[i];
 }
 //
 for (int i = 0; i < m_Elements.size(); i++)
 {
  if (0!=m_Elements[i]) delete m_Elements[i];
 }
 for (int i = 0; i < m_Signals.size(); i++)
 {
  if (0!=m_Signals[i]) delete m_Signals[i];
 }
 for (int i = 0; i < m_Errors.size(); i++)
 {
  if (0!=m_Errors[i]) delete m_Errors[i];
 }
 //
 m_Libraries.clear();
 m_Attributes.clear();
 m_VariantDefs.clear();
 m_Classes.clear();
 m_DesignRules.clear();
 // autorouter
 m_Passes.clear();
 //
 m_Elements.clear();
 m_Signals.clear();
 m_Errors.clear();
}

void CBoard::assign(const CBoard& board)
{
 clear();
 m_Description = board.m_Description;
 m_Plain = board.m_Plain;
 //
 for (int i = 0; i < board.m_Libraries.size(); i++)
 {
  CLibrary *l = new CLibrary(*board.m_Libraries.at(i));
  m_Libraries.append(l);
 }
 for (int i = 0; i < board.m_Attributes.size(); i++)
 {
  CAttribute *a = new CAttribute(*board.m_Attributes.at(i));
  m_Attributes.append(a);
 }
 for (int i = 0; i < board.m_VariantDefs.size(); i++)
 {
  CVariantDef *v = new CVariantDef(*board.m_VariantDefs.at(i));
  m_VariantDefs.append(v);
 }
 for (int i = 0; i < board.m_Classes.size(); i++)
 {
  CClass *c = new CClass(*board.m_Classes.at(i));
  m_Classes.append(c);
 }
 // autorouter
 for (int i = 0; i < board.m_Passes.size(); i++)
 {
  CPass *p = new CPass(*board.m_Passes.at(i));
  m_Passes.append(p);
 }
 //
 for (int i = 0; i < board.m_Elements.size(); i++)
 {
  CElement *e = new CElement(*board.m_Elements.at(i));
  m_Elements.append(e);
 }
 for (int i = 0; i < board.m_Signals.size(); i++)
 {
  CSignal *s = new CSignal(*board.m_Signals.at(i));
  m_Signals.append(s);
 }
 for (int i = 0; i < board.m_Errors.size(); i++)
 {
  CApproved *a = new CApproved(*board.m_Errors.at(i));
  m_Errors.append(a);
 }
}

void CBoard::scale(const double factor)
{
 m_Plain.scale(factor);
 //
 for (int i = 0; i < m_Libraries.size(); i++)
 {
  m_Libraries[i]->scalePackages(factor);
 }
 for (int i = 0; i < m_Attributes.size(); i++)
 {
  m_Attributes[i]->scale(factor);
 }
 for (int i = 0; i < m_Elements.size(); i++)
 {
  m_Elements[i]->scale(factor);
 }
 for (int i = 0; i < m_Signals.size(); i++)
 {
  m_Signals[i]->scale(factor);
 }
}

void CBoard::show(std::ostream& out, const int level)
{
 CEntity::show(out,level); out<<"Board:"<<std::endl;
 CEntity::show(out,level); out<<"{"<<std::endl;
 m_Description.show(out,level+1);
 m_Plain.show(out,level+1);
 CEntity::show(out,level); out<<"\tLibraries="<<std::endl;
 CEntity::show(out,level); out<<"\t{"<<std::endl;
 for (int i = 0; i < m_Libraries.size(); i++)
 {
  m_Libraries[i]->show(out,level+2);
 }
 CEntity::show(out,level); out<<"\t}"<<std::endl;
 CEntity::show(out,level); out<<"\tAttributes="<<std::endl;
 CEntity::show(out,level); out<<"\t{"<<std::endl;
 for (int i = 0; i < m_Attributes.size(); i++)
 {
  m_Attributes[i]->show(out,level+2);
 }
 CEntity::show(out,level); out<<"\t}"<<std::endl;
 CEntity::show(out,level); out<<"\tVariantDefs="<<std::endl;
 CEntity::show(out,level); out<<"\t{"<<std::endl;
 for (int i = 0; i < m_VariantDefs.size(); i++)
 {
  m_VariantDefs[i]->show(out,level+2);
 }
 CEntity::show(out,level); out<<"\t}"<<std::endl;
 CEntity::show(out,level); out<<"\tClasses="<<std::endl;
 CEntity::show(out,level); out<<"\t{"<<std::endl;
 for (int i = 0; i < m_Classes.size(); i++)
 {
  m_Classes[i]->show(out,level+2);
 }
 CEntity::show(out,level); out<<"\t}"<<std::endl;
 m_DesignRules.show(out,level+1);
 /*
 CEntity::show(out,level); out<<"\tDesignRules="<<std::endl;
 CEntity::show(out,level); out<<"\t{"<<std::endl;
 for (int i = 0; i < m_DesignRules.size(); i++)
 {
  m_DesignRules[i]->show(out,level+2);
 }
 */
 CEntity::show(out,level); out<<"\t}"<<std::endl;
 CEntity::show(out,level); out<<"\tAutorouter="<<std::endl;
 CEntity::show(out,level); out<<"\t{"<<std::endl;
 for (int i = 0; i < m_Passes.size(); i++)
 {
  m_Passes[i]->show(out,level+2);
 }
 CEntity::show(out,level); out<<"\t}"<<std::endl;
 CEntity::show(out,level); out<<"\tElements="<<std::endl;
 CEntity::show(out,level); out<<"\t{"<<std::endl;
 for (int i = 0; i < m_Elements.size(); i++)
 {
  m_Elements[i]->show(out,level+2);
 }
 CEntity::show(out,level); out<<"\t}"<<std::endl;
 CEntity::show(out,level); out<<"\tSignals="<<std::endl;
 CEntity::show(out,level); out<<"\t{"<<std::endl;
 for (int i = 0; i < m_Signals.size(); i++)
 {
  m_Signals[i]->show(out,level+2);
 }
 CEntity::show(out,level); out<<"\t}"<<std::endl;
 CEntity::show(out,level); out<<"\tErrors="<<std::endl;
 CEntity::show(out,level); out<<"\t{"<<std::endl;
 for (int i = 0; i < m_Errors.size(); i++)
 {
  m_Errors[i]->show(out,level+2);
 }
 CEntity::show(out,level); out<<"\t}"<<std::endl;
 CEntity::show(out,level); out<<"}"<<std::endl;
}

bool CBoard::readFromXML(const QDomElement& root)
{
 QDomElement e = root;
 if (e.nodeName()!="board") e = e.nextSiblingElement("board");
 if (!e.isNull())
 {
  QDomElement c = e.firstChildElement("description");
  if (!c.isNull())
  {
   m_Description.readFromXML(c);
  }
  c = e.firstChildElement("plain");
  if (!c.isNull())
  {
   m_Plain.readFromXML(c);
  }
  c = e.firstChildElement("libraries");
  if (!c.isNull())
  {
   QDomElement cc = c.firstChildElement("library");
   while (!cc.isNull())
   {
    CLibrary *l = new CLibrary();
    m_Libraries.append(l);
    l->readFromXML(cc);
    cc = cc.nextSiblingElement("library");
   }
  }
  c = e.firstChildElement("attributes");
  if (!c.isNull())
  {
   QDomElement cc = c.firstChildElement("attribute");
   while (!cc.isNull())
   {
    CAttribute *a = new CAttribute();
    m_Attributes.append(a);
    a->readFromXML(cc);
    cc = cc.nextSiblingElement("attribute");
   }
  }
  c = e.firstChildElement("variantdefs");
  if (!c.isNull())
  {
   QDomElement cc = c.firstChildElement("variantdef");
   while (!cc.isNull())
   {
    CVariantDef *v = new CVariantDef();
    m_VariantDefs.append(v);
    v->readFromXML(cc);
    cc = cc.nextSiblingElement("variantdef");
   }
  }
  c = e.firstChildElement("classes");
  if (!c.isNull())
  {
   QDomElement cc = c.firstChildElement("class");
   while (!cc.isNull())
   {
    CClass *l = new CClass();
    m_Classes.append(l);
    l->readFromXML(cc);
    cc = cc.nextSiblingElement("class");
   }
  }
  c = e.firstChildElement("designrules");
  if (!c.isNull())
  {
   m_DesignRules.readFromXML(c);
   /*
   QDomElement cc = c.firstChildElement("designrule");
   while (!cc.isNull())
   {
    CDesignRule *r = new CDesignRule();
    m_DesignRules.append(r);
    r->readFromXML(cc);
    cc = cc.nextSiblingElement("designrule");
   }
   */
  }
  c = e.firstChildElement("autorouter");
  if (!c.isNull())
  {
   QDomElement cc = c.firstChildElement("pass");
   while (!cc.isNull())
   {
    CPass *p = new CPass();
    m_Passes.append(p);
    p->readFromXML(cc);
    cc = cc.nextSiblingElement("pass");
   }
  }
  c = e.firstChildElement("elements");
  if (!c.isNull())
  {
   QDomElement cc = c.firstChildElement("element");
   while (!cc.isNull())
   {
    CElement *m = new CElement();
    m_Elements.append(m);
    m->readFromXML(cc);
    cc = cc.nextSiblingElement("element");
   }
  }
  c = e.firstChildElement("signals");
  if (!c.isNull())
  {
   QDomElement cc = c.firstChildElement("signal");
   while (!cc.isNull())
   {
    CSignal *n = new CSignal();
    m_Signals.append(n);
    n->readFromXML(cc);
    cc = cc.nextSiblingElement("signal");
   }
  }
  c = e.firstChildElement("errors");
  if (!c.isNull())
  {
   QDomElement cc = c.firstChildElement("approved");
   while (!cc.isNull())
   {
    CApproved *a = new CApproved();
    m_Errors.append(a);
    a->readFromXML(cc);
    cc = cc.nextSiblingElement("approved");
   }
  }
 }
 return true;
}

bool CBoard::writeToXML(QDomDocument& host, QDomElement& root, const bool defaults)
{
 if (!host.isNull() && !root.isNull())
 {
  QDomElement e = host.createElement("board");
  m_Description.writeToXML(host,e,defaults);
  m_Plain.writeToXML(host,e,defaults);
  QDomElement c = host.createElement("libraries");
  e.appendChild(c);
  for (int i = 0; i < m_Libraries.size(); i++)
  {
   m_Libraries[i]->writeToXML(host,c,defaults);
  }
  c = host.createElement("attributes");
  e.appendChild(c);
  for (int i = 0; i < m_Attributes.size(); i++)
  {
   m_Attributes[i]->writeToXML(host,c,defaults);
  }
  c = host.createElement("variantdefs");
  e.appendChild(c);
  for (int i = 0; i < m_VariantDefs.size(); i++)
  {
   m_VariantDefs[i]->writeToXML(host,c,defaults);
  }
  c = host.createElement("classes");
  e.appendChild(c);
  for (int i = 0; i < m_Classes.size(); i++)
  {
   m_Classes[i]->writeToXML(host,c,defaults);
  }
  c = host.createElement("designrules");
  m_DesignRules.writeToXML(host,c,defaults);
  e.appendChild(c);
  /*
  for (int i = 0; i < m_DesignRules.size(); i++)
  {
   m_DesignRules[i]->writeToXML(host,c,defaults);
  }
  */
  c = host.createElement("autorouter");
  e.appendChild(c);
  for (int i = 0; i < m_Passes.size(); i++)
  {
   m_Passes[i]->writeToXML(host,c,defaults);
  }
  c = host.createElement("elements");
  e.appendChild(c);
  for (int i = 0; i < m_Elements.size(); i++)
  {
   m_Elements[i]->writeToXML(host,c,defaults);
  }
  c = host.createElement("signals");
  e.appendChild(c);
  for (int i = 0; i < m_Signals.size(); i++)
  {
   m_Signals[i]->writeToXML(host,c,defaults);
  }
  c = host.createElement("errors");
  e.appendChild(c);
  for (int i = 0; i < m_Errors.size(); i++)
  {
   m_Errors[i]->writeToXML(host,c,defaults);
  }
  root.appendChild(e);
  return true;
 }
 return false;
}

//------------------------------------------------------------------------------

CSchematic::CSchematic(const CSchematic& schematic)
{
 assign(schematic);
}

CSchematic::CSchematic(void)
{
 clear();
}

CSchematic::~CSchematic(void)
{
 clear();
}

void CSchematic::operator =(const CSchematic& schematic)
{
 assign(schematic);
}

void CSchematic::clear(void)
{
 m_Description.clear();
 for (int i = 0; i < m_Libraries.size(); i++)
 {
  if (0!=m_Libraries[i]) delete m_Libraries[i];
 }
 for (int i = 0; i < m_Attributes.size(); i++)
 {
  if (0!=m_Attributes[i]) delete m_Attributes[i];
 }
 for (int i = 0; i < m_VariantDefs.size(); i++)
 {
  if (0!=m_VariantDefs[i]) delete m_VariantDefs[i];
 }
 for (int i = 0; i < m_Classes.size(); i++)
 {
  if (0!=m_Classes[i]) delete m_Classes[i];
 }
 for (int i = 0; i < m_Parts.size(); i++)
 {
  if (0!=m_Parts[i]) delete m_Parts[i];
 }
 for (int i = 0; i < m_Sheets.size(); i++)
 {
  if (0!=m_Sheets[i]) delete m_Sheets[i];
 }
 for (int i = 0; i < m_Errors.size(); i++)
 {
  if (0!=m_Errors[i]) delete m_Errors[i];
 }
 m_Libraries.clear();
 m_Attributes.clear();
 m_VariantDefs.clear();
 m_Classes.clear();
 m_Parts.clear();
 m_Sheets.clear();
 m_Errors.clear();
 m_XRefLabel.clear();
 m_XRefPart.clear();
}

void CSchematic::assign(const CSchematic& schematic)
{
 clear();
 m_Description = schematic.m_Description;
 for (int i = 0; i < schematic.m_Libraries.size(); i++)
 {
  CLibrary *l = new CLibrary(*schematic.m_Libraries.at(i));
  m_Libraries.append(l);
 }
 for (int i = 0; i < schematic.m_Attributes.size(); i++)
 {
  CAttribute *a = new CAttribute(*schematic.m_Attributes.at(i));
  m_Attributes.append(a);
 }
 for (int i = 0; i < schematic.m_VariantDefs.size(); i++)
 {
  CVariantDef *v = new CVariantDef(*schematic.m_VariantDefs.at(i));
  m_VariantDefs.append(v);
 }
 for (int i = 0; i < schematic.m_Classes.size(); i++)
 {
  CClass *c = new CClass(*schematic.m_Classes.at(i));
  m_Classes.append(c);
 }
 for (int i = 0; i < schematic.m_Parts.size(); i++)
 {
  CPart *p = new CPart(*schematic.m_Parts.at(i));
  m_Parts.append(p);
 }
 for (int i = 0; i < schematic.m_Sheets.size(); i++)
 {
  CSheet *s = new CSheet(*schematic.m_Sheets.at(i));
  m_Sheets.append(s);
 }
 for (int i = 0; i < schematic.m_Errors.size(); i++)
 {
  CApproved *a = new CApproved(*schematic.m_Errors.at(i));
  m_Errors.append(a);
 }
 m_XRefLabel = schematic.m_XRefLabel;
 m_XRefPart = schematic.m_XRefPart;
}

void CSchematic::scale(const double factor)
{
 for (int i = 0; i < m_Libraries.size(); i++)
 {
  m_Libraries[i]->scalePackages(factor);
 }
 for (int i = 0; i < m_Attributes.size(); i++)
 {
  m_Attributes[i]->scale(factor);
 }
 for (int i = 0; i < m_Parts.size(); i++)
 {
  m_Parts[i]->scale(factor);
 }
 for (int i = 0; i < m_Sheets.size(); i++)
 {
  m_Sheets[i]->scale(factor);
 }
}

void CSchematic::scalePackages(const double factor)
{
 for (int i = 0; i < m_Libraries.size(); i++)
 {
  m_Libraries[i]->scalePackages(factor);
 }
}

void CSchematic::show(std::ostream& out, const int level)
{
 CEntity::show(out,level); out<<"Schematic:"<<std::endl;
 CEntity::show(out,level); out<<"{"<<std::endl;
 CEntity::show(out,level); out<<"\tXRefLabel='"<<m_XRefLabel.toUtf8().data()
    <<"', XRefPart='"<<m_XRefPart.toUtf8().data()<<"',"<<std::endl;
 m_Description.show(out,level+1);
 CEntity::show(out,level); out<<"\tLibraries="<<std::endl;
 CEntity::show(out,level); out<<"\t{"<<std::endl;
 for (int i = 0; i < m_Libraries.size(); i++)
 {
  m_Libraries[i]->show(out,level+2);
 }
 CEntity::show(out,level); out<<"\t}"<<std::endl;
 CEntity::show(out,level); out<<"\tAttributes="<<std::endl;
 CEntity::show(out,level); out<<"\t{"<<std::endl;
 for (int i = 0; i < m_Attributes.size(); i++)
 {
  m_Attributes[i]->show(out,level+2);
 }
 CEntity::show(out,level); out<<"\t}"<<std::endl;
 CEntity::show(out,level); out<<"\tVariantDefs="<<std::endl;
 CEntity::show(out,level); out<<"\t{"<<std::endl;
 for (int i = 0; i < m_VariantDefs.size(); i++)
 {
  m_VariantDefs[i]->show(out,level+2);
 }
 CEntity::show(out,level); out<<"\t}"<<std::endl;
 CEntity::show(out,level); out<<"\tClasses="<<std::endl;
 CEntity::show(out,level); out<<"\t{"<<std::endl;
 for (int i = 0; i < m_Classes.size(); i++)
 {
  m_Classes[i]->show(out,level+2);
 }
 CEntity::show(out,level); out<<"\t}"<<std::endl;
 CEntity::show(out,level); out<<"\tParts="<<std::endl;
 CEntity::show(out,level); out<<"\t{"<<std::endl;
 for (int i = 0; i < m_Parts.size(); i++)
 {
  m_Parts[i]->show(out,level+2);
 }
 CEntity::show(out,level); out<<"\t}"<<std::endl;
 CEntity::show(out,level); out<<"\tSheets="<<std::endl;
 CEntity::show(out,level); out<<"\t{"<<std::endl;
 for (int i = 0; i < m_Sheets.size(); i++)
 {
  m_Sheets[i]->show(out,level+2);
 }
 CEntity::show(out,level); out<<"\t}"<<std::endl;
 CEntity::show(out,level); out<<"\tErrors="<<std::endl;
 CEntity::show(out,level); out<<"\t{"<<std::endl;
 for (int i = 0; i < m_Errors.size(); i++)
 {
  m_Errors[i]->show(out,level+2);
 }
 CEntity::show(out,level); out<<"\t}"<<std::endl;
 CEntity::show(out,level); out<<"}"<<std::endl;
}

bool CSchematic::readFromXML(const QDomElement& root)
{
 QDomElement e = root;
 if (e.nodeName()!="schematic") e = e.nextSiblingElement("schematic");
 if (!e.isNull())
 {
  QDomElement c = e.firstChildElement("description");
  if (!c.isNull())
  {
   m_Description.readFromXML(c);
  }
  c = e.firstChildElement("libraries");
  if (!c.isNull())
  {
   QDomElement cc = c.firstChildElement("library");
   while (!cc.isNull())
   {
    CLibrary *l = new CLibrary();
    m_Libraries.append(l);
    l->readFromXML(cc);
    cc = cc.nextSiblingElement("library");
   }
  }
  c = e.firstChildElement("attributes");
  if (!c.isNull())
  {
   QDomElement cc = c.firstChildElement("attribute");
   while (!cc.isNull())
   {
    CAttribute *a = new CAttribute();
    m_Attributes.append(a);
    a->readFromXML(cc);
    cc = cc.nextSiblingElement("attribute");
   }
  }
  c = e.firstChildElement("variantdefs");
  if (!c.isNull())
  {
   QDomElement cc = c.firstChildElement("variantdef");
   while (!cc.isNull())
   {
    CVariantDef *v = new CVariantDef();
    m_VariantDefs.append(v);
    v->readFromXML(cc);
    cc = cc.nextSiblingElement("variantdef");
   }
  }
  c = e.firstChildElement("classes");
  if (!c.isNull())
  {
   QDomElement cc = c.firstChildElement("class");
   while (!cc.isNull())
   {
    CClass *l = new CClass();
    m_Classes.append(l);
    l->readFromXML(cc);
    cc = cc.nextSiblingElement("class");
   }
  }
  c = e.firstChildElement("parts");
  if (!c.isNull())
  {
   QDomElement cc = c.firstChildElement("part");
   while (!cc.isNull())
   {
    CPart *p = new CPart();
    m_Parts.append(p);
    p->readFromXML(cc);
    cc = cc.nextSiblingElement("part");
   }
  }
  c = e.firstChildElement("sheets");
  if (!c.isNull())
  {
   QDomElement cc = c.firstChildElement("sheet");
   while (!cc.isNull())
   {
    CSheet *h = new CSheet();
    m_Sheets.append(h);
    h->readFromXML(cc);
    cc = cc.nextSiblingElement("sheet");
   }
  }
  c = e.firstChildElement("errors");
  if (!c.isNull())
  {
   QDomElement cc = c.firstChildElement("approved");
   while (!cc.isNull())
   {
    CApproved *a = new CApproved();
    m_Errors.append(a);
    a->readFromXML(cc);
    cc = cc.nextSiblingElement("approved");
   }
  }
  QString s = c.attribute("xreflabel");
  if (!s.isEmpty())
  {
   m_XRefLabel = s;
  }
  s = c.attribute("xrefpart");
  if (!s.isEmpty())
  {
   m_XRefPart = s;
  }
 }
 return true;
}

bool CSchematic::writeToXML(QDomDocument& host, QDomElement& root, const bool defaults)
{
 if (!host.isNull() && !root.isNull())
 {
  QDomElement e = host.createElement("schematic");
  e.setAttribute("xreflabel",m_XRefLabel);
  e.setAttribute("xrefpart",m_XRefPart);
  m_Description.writeToXML(host,e,defaults);
  QDomElement c = host.createElement("libraries");
  e.appendChild(c);
  for (int i = 0; i < m_Libraries.size(); i++)
  {
   m_Libraries[i]->writeToXML(host,c,defaults);
  }
  c = host.createElement("attributes");
  e.appendChild(c);
  for (int i = 0; i < m_Attributes.size(); i++)
  {
   m_Attributes[i]->writeToXML(host,c,defaults);
  }
  c = host.createElement("variantdefs");
  e.appendChild(c);
  for (int i = 0; i < m_VariantDefs.size(); i++)
  {
   m_VariantDefs[i]->writeToXML(host,c,defaults);
  }
  c = host.createElement("classes");
  e.appendChild(c);
  for (int i = 0; i < m_Classes.size(); i++)
  {
   m_Classes[i]->writeToXML(host,c,defaults);
  }
  c = host.createElement("parts");
  e.appendChild(c);
  for (int i = 0; i < m_Parts.size(); i++)
  {
   m_Parts[i]->writeToXML(host,c,defaults);
  }
  c = host.createElement("sheets");
  e.appendChild(c);
  for (int i = 0; i < m_Sheets.size(); i++)
  {
   m_Sheets[i]->writeToXML(host,c,defaults);
  }
  c = host.createElement("errors");
  e.appendChild(c);
  for (int i = 0; i < m_Errors.size(); i++)
  {
   m_Errors[i]->writeToXML(host,c,defaults);
  }
  root.appendChild(e);
  return true;
 }
 return false;
}

CLibrary *CSchematic::findLibraryByName(const QString& name)
{
 CLibrary *result = 0;
 for (size_t i = 0; i < m_Libraries.size(); i++)
 {
  CLibrary *l = m_Libraries.at(i);
  if (l->name()==name)
  {
   result = l;
   break;
  }
 }
 return result;
}

CPart *CSchematic::findPartByName(const QString& name)
{
 CPart *result = 0;
 for (size_t i = 0; i < m_Parts.size(); i++)
 {
  CPart *p = m_Parts.at(i);
  if (p->name()==name)
  {
   result = p;
   break;
  }
 }
 return result;
}

//------------------------------------------------------------------------------

CDrawing::CDrawing(const CDrawing& drawing)
{
 assign(drawing);
}

CDrawing::CDrawing(void)
{
 clear();
}

CDrawing::~CDrawing(void)
{
 clear();
}

void CDrawing::operator =(const CDrawing& drawing)
{
 assign(drawing);
}

void CDrawing::clear(void)
{
 m_Grid.clear();
 m_Library.clear();
 m_Schematic.clear();
 m_Board.clear();
 /*
 for (int i = 0; i < m_Settings.size(); i++)
 {
  if (0!=m_Settings[i]) delete m_Settings[i];
 }
 */
 for (int i = 0; i < m_Layers.size(); i++)
 {
  if (0!=m_Layers[i]) delete m_Layers[i];
 }
 m_Settings.clear();
 m_Layers.clear();
 //
 m_Mode = CDrawing::dmMixed;
}

void CDrawing::assign(const CDrawing& drawing)
{
 clear();
 m_Grid = drawing.m_Grid;
 m_Library = drawing.m_Library;
 m_Schematic = drawing.m_Schematic;
 m_Board = drawing.m_Board;
 for (int i = 0; i < drawing.m_Layers.size(); i++)
 {
  CLayer *l = new CLayer(*drawing.m_Layers.at(i));
  m_Layers.append(l);
 }
 m_Settings = drawing.m_Settings;
 m_Layers = drawing.m_Layers;
 //
 m_Mode = drawing.m_Mode;
}

void CDrawing::initDefaultLayers()
{
 for (int i = 0; i < m_Layers.size(); i++)
 {
  if (0!=m_Layers[i]) delete m_Layers[i];
 }
 m_Layers.clear();
 m_Layers.append(new CLayer(CLayer::LAYER_TOP,CLayer::layerName(CLayer::LAYER_TOP), 4, 1, true, true));
 m_Layers.append(new CLayer(CLayer::LAYER_LAYER2,CLayer::layerName(CLayer::LAYER_LAYER2), 1, 1, true, true));
 m_Layers.append(new CLayer(CLayer::LAYER_LAYER3,CLayer::layerName(CLayer::LAYER_LAYER3), 1, 1, true, true));
 m_Layers.append(new CLayer(CLayer::LAYER_LAYER4,CLayer::layerName(CLayer::LAYER_LAYER4), 1, 1, true, true));
 m_Layers.append(new CLayer(CLayer::LAYER_LAYER5,CLayer::layerName(CLayer::LAYER_LAYER5), 1, 1, true, true));
 m_Layers.append(new CLayer(CLayer::LAYER_LAYER6,CLayer::layerName(CLayer::LAYER_LAYER6), 1, 1, true, true));
 m_Layers.append(new CLayer(CLayer::LAYER_LAYER7,CLayer::layerName(CLayer::LAYER_LAYER7), 1, 1, true, true));
 m_Layers.append(new CLayer(CLayer::LAYER_LAYER8,CLayer::layerName(CLayer::LAYER_LAYER8), 1, 1, true, true));
 m_Layers.append(new CLayer(CLayer::LAYER_LAYER9,CLayer::layerName(CLayer::LAYER_LAYER9), 1, 1, true, true));
 m_Layers.append(new CLayer(CLayer::LAYER_LAYER10,CLayer::layerName(CLayer::LAYER_LAYER10), 1, 1, true, true));
 m_Layers.append(new CLayer(CLayer::LAYER_LAYER11,CLayer::layerName(CLayer::LAYER_LAYER11), 1, 1, true, true));
 m_Layers.append(new CLayer(CLayer::LAYER_LAYER12,CLayer::layerName(CLayer::LAYER_LAYER12), 1, 1, true, true));
 m_Layers.append(new CLayer(CLayer::LAYER_LAYER13,CLayer::layerName(CLayer::LAYER_LAYER13), 1, 1, true, true));
 m_Layers.append(new CLayer(CLayer::LAYER_LAYER14,CLayer::layerName(CLayer::LAYER_LAYER14), 1, 1, true, true));
 m_Layers.append(new CLayer(CLayer::LAYER_LAYER15,CLayer::layerName(CLayer::LAYER_LAYER15), 1, 1, true, true));
 m_Layers.append(new CLayer(CLayer::LAYER_BOTTOM,CLayer::layerName(CLayer::LAYER_BOTTOM), 1, 1, true, true));
 m_Layers.append(new CLayer(CLayer::LAYER_PADS,CLayer::layerName(CLayer::LAYER_PADS), 2, 1, true, true));
 m_Layers.append(new CLayer(CLayer::LAYER_VIAS,CLayer::layerName(CLayer::LAYER_VIAS), 2, 1, true, true));
 m_Layers.append(new CLayer(CLayer::LAYER_UNROUTED,CLayer::layerName(CLayer::LAYER_UNROUTED), 6, 1, true, true));
 m_Layers.append(new CLayer(CLayer::LAYER_DIMENSION,CLayer::layerName(CLayer::LAYER_DIMENSION), 15, 1, true, true));
 m_Layers.append(new CLayer(CLayer::LAYER_TPLACE,CLayer::layerName(CLayer::LAYER_TPLACE), 7, 1, true, true));
 m_Layers.append(new CLayer(CLayer::LAYER_BPLACE,CLayer::layerName(CLayer::LAYER_BPLACE), 7, 1, true, true));
 m_Layers.append(new CLayer(CLayer::LAYER_TORIGINS,CLayer::layerName(CLayer::LAYER_TORIGINS), 15, 1, true, true));
 m_Layers.append(new CLayer(CLayer::LAYER_BORIGINS,CLayer::layerName(CLayer::LAYER_BORIGINS), 15, 1, true, true));
 m_Layers.append(new CLayer(CLayer::LAYER_TNAMES,CLayer::layerName(CLayer::LAYER_TNAMES), 7, 1, true, true));
 m_Layers.append(new CLayer(CLayer::LAYER_BNAMES,CLayer::layerName(CLayer::LAYER_BNAMES), 7, 1, true, true));
 m_Layers.append(new CLayer(CLayer::LAYER_TVALUES,CLayer::layerName(CLayer::LAYER_TVALUES), 7, 1, true, true));
 m_Layers.append(new CLayer(CLayer::LAYER_BVALUES,CLayer::layerName(CLayer::LAYER_BVALUES), 7, 1, true, true));
 m_Layers.append(new CLayer(CLayer::LAYER_TSTOP,CLayer::layerName(CLayer::LAYER_TSTOP), 7, 3, false, true));
 m_Layers.append(new CLayer(CLayer::LAYER_BSTOP,CLayer::layerName(CLayer::LAYER_BSTOP), 7, 6, false, true));
 m_Layers.append(new CLayer(CLayer::LAYER_TCREAM,CLayer::layerName(CLayer::LAYER_TCREAM), 7, 4, false, true));
 m_Layers.append(new CLayer(CLayer::LAYER_BCREAM,CLayer::layerName(CLayer::LAYER_BCREAM), 7, 5, false, true));
 m_Layers.append(new CLayer(CLayer::LAYER_TFINISH,CLayer::layerName(CLayer::LAYER_TFINISH), 6, 3, false, true));
 m_Layers.append(new CLayer(CLayer::LAYER_BFINISH,CLayer::layerName(CLayer::LAYER_BFINISH), 6, 6, false, true));
 m_Layers.append(new CLayer(CLayer::LAYER_TGLUE,CLayer::layerName(CLayer::LAYER_TGLUE), 7, 4, false, true));
 m_Layers.append(new CLayer(CLayer::LAYER_BGLUE,CLayer::layerName(CLayer::LAYER_BGLUE), 7, 5, false, true));
 m_Layers.append(new CLayer(CLayer::LAYER_TTEST,CLayer::layerName(CLayer::LAYER_TTEST), 7, 1, false, true));
 m_Layers.append(new CLayer(CLayer::LAYER_BTEST,CLayer::layerName(CLayer::LAYER_BTEST), 7, 1, false, true));
 m_Layers.append(new CLayer(CLayer::LAYER_TKEEPOUT,CLayer::layerName(CLayer::LAYER_TKEEPOUT), 4, 11, true, true));
 m_Layers.append(new CLayer(CLayer::LAYER_BKEEPOUT,CLayer::layerName(CLayer::LAYER_BKEEPOUT), 1, 11, true, true));
 m_Layers.append(new CLayer(CLayer::LAYER_TRESTRICT,CLayer::layerName(CLayer::LAYER_TRESTRICT), 4, 10, true, true));
 m_Layers.append(new CLayer(CLayer::LAYER_BRESTRICT,CLayer::layerName(CLayer::LAYER_BRESTRICT), 1, 10, true, true));
 m_Layers.append(new CLayer(CLayer::LAYER_VRESTRICT,CLayer::layerName(CLayer::LAYER_VRESTRICT), 2, 10, true, true));
 m_Layers.append(new CLayer(CLayer::LAYER_DRILLS,CLayer::layerName(CLayer::LAYER_DRILLS), 7, 1, false, true));
 m_Layers.append(new CLayer(CLayer::LAYER_HOLES,CLayer::layerName(CLayer::LAYER_HOLES), 7, 1, false, true));
 m_Layers.append(new CLayer(CLayer::LAYER_MILLING,CLayer::layerName(CLayer::LAYER_MILLING), 3, 1, false, true));
 m_Layers.append(new CLayer(CLayer::LAYER_MEASURES,CLayer::layerName(CLayer::LAYER_MEASURES), 7, 1, false, true));
 m_Layers.append(new CLayer(CLayer::LAYER_DOCUMENT,CLayer::layerName(CLayer::LAYER_DOCUMENT), 7, 1, true, true));
 m_Layers.append(new CLayer(CLayer::LAYER_REFERENCE,CLayer::layerName(CLayer::LAYER_REFERENCE), 7, 1, true, true));
 m_Layers.append(new CLayer(CLayer::LAYER_TDOCU,CLayer::layerName(CLayer::LAYER_TDOCU), 7, 1, true, true));
 m_Layers.append(new CLayer(CLayer::LAYER_BDOCU,CLayer::layerName(CLayer::LAYER_BDOCU), 7, 1, true, true));
 m_Layers.append(new CLayer(CLayer::LAYER_NETS,CLayer::layerName(CLayer::LAYER_NETS), 7, 1, true, true));
 m_Layers.append(new CLayer(CLayer::LAYER_BUSSES,CLayer::layerName(CLayer::LAYER_BUSSES), 7, 1, true, true));
 m_Layers.append(new CLayer(CLayer::LAYER_PINS,CLayer::layerName(CLayer::LAYER_PINS), 2, 1, false, true));
 m_Layers.append(new CLayer(CLayer::LAYER_SYMBOLS,CLayer::layerName(CLayer::LAYER_SYMBOLS), 4, 1, true, true));
 m_Layers.append(new CLayer(CLayer::LAYER_NAMES,CLayer::layerName(CLayer::LAYER_NAMES), 7, 1, true, true));
 m_Layers.append(new CLayer(CLayer::LAYER_VALUES,CLayer::layerName(CLayer::LAYER_VALUES), 7, 1, true, true));
 m_Layers.append(new CLayer(CLayer::LAYER_INFO,CLayer::layerName(CLayer::LAYER_INFO), 7, 1, true, true));
 m_Layers.append(new CLayer(CLayer::LAYER_GUIDE,CLayer::layerName(CLayer::LAYER_GUIDE), 6, 1, true, true));
 //m_Layers.append(new CLayer(CLayer::LAYER_,CLayer::layerName(CLayer::LAYER_), 1, 1, true, true));
}

CLayer *CDrawing::findLayerByID(const TLayer layer)
{
 for (int i = 0; i < m_Layers.size(); i++)
 {
  CLayer *l = m_Layers[i];
  if (l->layer()==layer) return l;
 }
 return 0;
}

void CDrawing::scale(const double factor)
{
 m_Grid.scale(factor);
 m_Library.scale(factor);
 m_Schematic.scale(factor);
 m_Board.scale(factor);
}

void CDrawing::show(std::ostream& out, const int level)
{
 CEntity::show(out,level); out<<"Drawing:"<<std::endl;
 CEntity::show(out,level); out<<"{"<<std::endl;
 m_Settings.show(out,level+1);
 m_Grid.show(out,level+1);
 CEntity::show(out,level); out<<"\tLayers="<<std::endl;
 CEntity::show(out,level); out<<"\t{"<<std::endl;
 for (int i = 0; i < m_Layers.size(); i++)
 {
  m_Layers[i]->show(out,level+2);
 }
 CEntity::show(out,level); out<<"\t}"<<std::endl;
 m_Library.show(out,level+1);
 m_Schematic.show(out,level+1);
 m_Board.show(out,level+1);
 CEntity::show(out,level); out<<"}"<<std::endl;
}

bool CDrawing::readFromXML(const QDomElement &root)
{
 bool result = true;
 QDomElement e = root;
 if (e.nodeName()!="drawing") e = e.nextSiblingElement("drawing");
 if (!e.isNull())
 {
  QDomElement c = e.firstChildElement("settings");
  if (!c.isNull())
  {
   result &= m_Settings.readFromXML(c);
  }
  c = e.firstChildElement("grid");
  if (!c.isNull())
  {
   result &= m_Grid.readFromXML(c);
  }
  c = e.firstChildElement("layers");
  if (!c.isNull())
  {
   QDomElement cc = c.firstChildElement("layer");
   while (!cc.isNull())
   {
    CLayer *l = new CLayer();
    m_Layers.append(l);
    result &= l->readFromXML(cc);
    cc = cc.nextSiblingElement("layer");
   }
  }
  bool is_library = false;
  c = e.firstChildElement("library");
  if (!c.isNull())
  {
   result &= m_Library.readFromXML(c);
   is_library = true;
  }
  bool is_schematic = false;
  c = e.firstChildElement("schematic");
  if (!c.isNull())
  {
   result &= m_Schematic.readFromXML(c);
   is_schematic = true;
  }
  bool is_board = false;
  c = e.firstChildElement("board");
  if (!c.isNull())
  {
   result &= m_Board.readFromXML(c);
   is_board = true;
  }
  if (is_library && !is_schematic && !is_board) m_Mode = CDrawing::dmLibrary;
  else if (!is_library && is_schematic && !is_board) m_Mode = CDrawing::dmSchematic;
  else if (!is_library && !is_schematic && is_board) m_Mode = CDrawing::dmBoard;
  else m_Mode = CDrawing::dmMixed;
 }
 return result;
}

bool CDrawing::writeToXML(QDomDocument& host, QDomElement& root, const bool defaults)
{
 if (!host.isNull() && !root.isNull())
 {
  QDomElement e = host.createElement("drawing");
  m_Settings.writeToXML(host,e,defaults);
  m_Grid.writeToXML(host,e,defaults);
  QDomElement c = host.createElement("layers");
  e.appendChild(c);
  for (int i = 0; i < m_Layers.size(); i++)
  {
   m_Layers[i]->writeToXML(host,c,defaults);
  }
  if ((CDrawing::dmLibrary==m_Mode) || (CDrawing::dmMixed==m_Mode))
  {
   m_Library.writeToXML(host,e,defaults);
  }
  if ((CDrawing::dmSchematic==m_Mode) || (CDrawing::dmMixed==m_Mode))
  {
   m_Schematic.writeToXML(host,e,defaults);
  }
  if ((CDrawing::dmBoard==m_Mode) || (CDrawing::dmMixed==m_Mode))
  {
   m_Board.writeToXML(host,e,defaults);
  }
  root.appendChild(e);
  return true;
 }
 return false;
}

//------------------------------------------------------------------------------

CNote::CNote(const CNote& note)
{
 assign(note);
}

CNote::CNote(void)
{
 clear();
}

CNote::~CNote(void)
{
 clear();
}

QString CNote::toString(const CNote::Severity value)
{
 switch (value)
 {
  case CNote::nsInfo: return "info";
  case CNote::nsWarning: return "warning";
  case CNote::nsError: return "error";
 }
 return "";
}

void CNote::operator =(const CNote& note)
{
 assign(note);
}

void CNote::clear(void)
{
 m_Note.clear();
 m_Version = EAGLE_DTD_VERSION;
 m_Severity = CNote::nsInfo;
}
\
void CNote::assign(const CNote& note)
{
 m_Note = note.m_Note;
 m_Version = note.m_Version;
 m_Severity = note.m_Severity;
}

void CNote::show(std::ostream& out, const int level)
{
 CEntity::show(out,level);
 out<<"Note:{Version="<<m_Version
    <<", Severity="<<toString(m_Severity).toUtf8().data()
    <<", {"<<m_Note.toUtf8().data()<<"} }"
    <<std::endl;
}

bool CNote::readFromXML(const QDomElement &root)
{
 QDomElement e = root;
 if (e.nodeName()!="note") e = e.nextSiblingElement("note");
 if (!e.isNull())
 {
  QDomNode n = e.firstChild();
  while (!n.isNull())
  {
   if (n.isText())
   {
    m_Note = n.nodeValue();
    break;
   }
   n = n.nextSibling();
  }
  QString s = s = e.attribute("version");
  if (!s.isEmpty())
  {
   bool ok = false;
   double value = s.toDouble(&ok);
   if (ok) { m_Version = value; }
  }
  s = e.attribute("severity");
  if (!s.isEmpty())
  {
   if (s=="info") { m_Severity = CNote::nsInfo; }
   else if (s=="warning") { m_Severity = CNote::nsWarning; }
   else if (s=="error") { m_Severity = CNote::nsError; }
  }
  return true;
 }
 return false;
}

bool CNote::writeToXML(QDomDocument& host, QDomElement& root, const bool defaults)
{
 if (!host.isNull() && !root.isNull())
 {
  QDomElement e = host.createElement("note");
  e.setAttribute("version",QString("%1").arg(m_Version));
  e.setAttribute("severity",toString(m_Severity));
  QDomNode n = host.createTextNode(m_Note);
  e.appendChild(n);
  root.appendChild(e);
  return true;
 }
 return false;
}


//------------------------------------------------------------------------------

CCompatibility::CCompatibility(const CCompatibility& compatibility)
{
 assign(compatibility);
}

CCompatibility::CCompatibility(void)
{
 clear();
}

CCompatibility::~CCompatibility(void)
{
 clear();
}

void CCompatibility::operator =(const CCompatibility& compatibility)
{
 assign(compatibility);
}

void CCompatibility::clear(void)
{
 for (int i = 0; i < m_Notes.size(); i++)
 {
  if (0!=m_Notes[i]) delete m_Notes[i];
 }
 m_Notes.clear();
}

void CCompatibility::assign(const CCompatibility& compatibility)
{
 clear();
 for (int i = 0; i < compatibility.m_Notes.size(); i++)
 {
  CNote *n = new CNote(*compatibility.m_Notes.at(i));
  m_Notes.append(n);
 }
 m_Notes = compatibility.m_Notes;
}

void CCompatibility::show(std::ostream& out, const int level)
{
 CEntity::show(out,level);
 out<<"Compatibility:"<<std::endl;
 out<<"\t{"<<std::endl;
 for (int i = 0; i < m_Notes.size(); i++)
 {
  m_Notes[i]->show(out,level+1);
 }
 out<<"\t}"<<std::endl;
}

bool CCompatibility::readFromXML(const QDomElement &root)
{
 QDomElement e = root;
 if (e.nodeName()!="compatibility") e = e.nextSiblingElement("compatibility");
 if (!e.isNull())
 {
  QDomElement c = e.firstChildElement("note");
  while (!c.isNull())
  {
   CNote *n = new CNote();
   m_Notes.append(n);
   n->readFromXML(c);
   c = c.nextSiblingElement("note");
  }
  return true;
 }
 return false;
}

bool CCompatibility::writeToXML(QDomDocument& host, QDomElement& root, const bool defaults)
{
 if (!host.isNull() && !root.isNull())
 {
  if (m_Notes.size()>0)
  {
   QDomElement e = host.createElement("compatibility");
   for (int i = 0; i < m_Notes.size(); i++)
   {
    m_Notes[i]->writeToXML(host,e,defaults);
   }
   root.appendChild(e);
  }
  return true;
 }
 return false;
}

//------------------------------------------------------------------------------

CEagleDocument::CEagleDocument(const CEagleDocument& document)
{
 assign(document);
}

CEagleDocument::CEagleDocument(void)
{
 clear();
}

CEagleDocument::~CEagleDocument(void)
{
 clear();
}

void CEagleDocument::operator =(const CEagleDocument& document)
{
 assign(document);
}

void CEagleDocument::clear(void)
{
 m_PreNotes.clear();
 m_Drawing.clear();
 m_PostNotes.clear();
 m_Version = EAGLE_DTD_VERSION;
 //
 m_VerifyDocType = true;
 m_Indentation = 0;
 m_WriteDefaults = true;
}

void CEagleDocument::assign(const CEagleDocument& document)
{
 m_PreNotes = document.m_PreNotes;
 m_Drawing = document.m_Drawing;
 m_PostNotes = document.m_PostNotes;
 m_Version = document.m_Version;
 //
 m_VerifyDocType = document.m_VerifyDocType;
 m_Indentation = document.m_Indentation;
 m_WriteDefaults = document.m_WriteDefaults;
}

void CEagleDocument::show(std::ostream& out, const int level)
{
 CEntity::show(out,level); out<<"Eagle:"<<std::endl;
 CEntity::show(out,level); out<<"{"<<std::endl;
 CEntity::show(out,level); out<<"\tVersion="<<m_Version<<std::endl;
 m_PreNotes.show(out,level+1);
 m_Drawing.show(out,level+1);
 m_PostNotes.show(out,level+1);
 CEntity::show(out,level); out<<"}"<<std::endl;
}

bool CEagleDocument::loadFromFile(const QString& fileName)
{
 QDomDocument d;
 QFile f(fileName);
 if (f.open(QIODevice::ReadOnly))
 {
  if (!d.setContent(&f,1))
  {
   f.close();
   return false;
  }
  f.close();
  QDomDocumentType dt = d.doctype();
#ifdef DEBUG_MSG
  std::cout<<"DOCTYPE:name "<<dt.name().toUtf8().data()<<std::endl;
  std::cout<<"DOCTYPE:publicID "<<dt.publicId().toUtf8().data()<<std::endl;
  std::cout<<"DOCTYPE:systemID "<<dt.systemId().toUtf8().data()<<std::endl;
#endif
  if (m_VerifyDocType)
  {
   m_ValidDocType = (dt.name()=="eagle") && (dt.systemId()=="eagle.dtd");
  }
  m_ValidXMLdata = readFromXML(d.documentElement());
  return true;
 }
 return false;
}

bool CEagleDocument::saveToFile(const QString& fileName)
{
 QDomImplementation di;
 QDomDocumentType dt = di.createDocumentType("eagle","eagle.dtd","eagle.dtd");
 QDomDocument d(dt);
 QDomElement e = d.documentElement();
 QDomNode n(d.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\""));
 d.insertBefore(n,d.firstChild());
 if (writeToXML(d,e,m_WriteDefaults))
 {
  QFile f(fileName);
  if (f.open(QIODevice::WriteOnly | QIODevice::Text))
  {
   QTextStream ts(&f);
   d.save(ts,m_Indentation);
   f.close();
   return true;
  }
  /*
  QFile f(fileName);
  if (f.open(QIODevice::WriteOnly))
  {
   QByteArray a = d.toByteArray(m_Indentation);
   f.write(a);
   f.close();
   return true;
  }
  */
 }
 return false;
}

bool CEagleDocument::readFromXML(const QDomElement& root)
{
 bool result = false;
 QDomElement e = root;
 do
 {
  if (e.nodeName()=="eagle")
  {
   QString version_string = e.attribute("version",QString("%1").arg(m_Version));
   m_Version = version_string.toDouble();
   e = e.firstChildElement("drawing");
   //m_PreNotes.readFromXML(root) &&
   if (!e.isNull())
   {
    result = m_Drawing.readFromXML(e);
   }
   //m_PostNotes.readFromXML(root);
   break;
  }
  e = e.nextSiblingElement("eagle");
 }
 while (!e.isNull());
#ifdef DEBUG_MSG
 std::cout<<"Document:root.name "<<root.nodeName().toUtf8().data()<<std::endl;
 std::cout<<"Document:version "<<m_Version<<std::endl;
#endif
 return result;
}

bool CEagleDocument::writeToXML(QDomDocument& host, QDomElement& root, const bool defaults)
{
 QDomElement e = host.createElement("eagle");
 e.setAttribute("version",QString("%1").arg(m_Version));
 host.appendChild(e);
 m_PreNotes.writeToXML(host,e,defaults);
 m_Drawing.writeToXML(host,e,defaults);
 m_PostNotes.writeToXML(host,e,defaults);
 return true;
}

}
//------------------------------------------------------------------------------

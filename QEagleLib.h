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
#ifndef QEAGLELIB_H
#define QEAGLELIB_H
//------------------------------------------------------------------------------
#include <QList>
#include <QString>
#include <QtXml>
//------------------------------------------------------------------------------
//#include ""
//------------------------------------------------------------------------------
#define EAGLE_DTD_VERSION 6.4

/*
This file implements Eagle CAD file data structures as described in "eagle.dtd"
*/

namespace Eagle
{

typedef int TLayer;
typedef int TClass;
typedef double TCoord; // coordinates, given in millimeters
typedef double TDimension; // dimensions, given in millimeters

class CEntity
{
 public:
  CEntity(void);
  virtual ~CEntity(void);
 public:
  static QString toString(const bool value);
 public:
  virtual void clear(void);
  virtual void show(std::ostream& out, const int level = 0);
  virtual bool readFromXML(const QDomElement& root);
  virtual bool writeToXML(QDomDocument& host, QDomElement& root, const bool defaults = false);
};

// miscellaneous objects

class CSettings: public CEntity
{
 public:
  enum VerticalText {vtUp, vtDown};
 public:
  CSettings(const CSettings& settings);
  CSettings(void);
  virtual ~CSettings(void);
 public:
  static QString toString(const CSettings::VerticalText value);
 public:
  virtual void operator =(const CSettings& settings);
  virtual void clear(void);
  virtual void assign(const CSettings& settings);
  virtual void show(std::ostream& out, const int level = 0);
  virtual bool readFromXML(const QDomElement& root);
  virtual bool writeToXML(QDomDocument& host, QDomElement& root, const bool defaults = false);
 public:
  // getters
  bool alwaysVectorFont(void) { return m_AlwaysVectorFont; }
  CSettings::VerticalText verticalText(void) { return m_VerticalText; }
  // setters
  void setAlwaysVectorFont(const bool value) { m_AlwaysVectorFont = value; }
  void setVerticalText(const CSettings::VerticalText value) { m_VerticalText = value; }
 protected:
  bool m_AlwaysVectorFont; // implied
  CSettings::VerticalText m_VerticalText; // default = up
};

class CGrid: public CEntity
{
 public:
  enum Unit {guMic, guMM, guMil, guInch};
  enum Style {gsLines, gsDots};
 public:
  CGrid(const CGrid& grid);
  CGrid(void);
  virtual ~CGrid(void);
 public:
  static QString toString(const CGrid::Unit value);
  static QString toString(const CGrid::Style value);
  static bool stringToUnit(const QString& name, CGrid::Unit& value);
 public:
  virtual void operator =(const CGrid& grid);
  virtual void clear(void);
  virtual void assign(const CGrid& grid);
  virtual void scale(const double factor);
  virtual void show(std::ostream& out, const int level = 0);
  virtual bool readFromXML(const QDomElement& root);
  virtual bool writeToXML(QDomDocument& host, QDomElement& root, const bool defaults = false);
 public:
  // getters
  double distance(void) { return m_Distance; }
  CGrid::Unit unitDist(void) { return m_UnitDist; }
  CGrid::Unit unit(void) { return m_Unit; }
  CGrid::Style style(void) { return m_Style; }
  int multiple(void) { return m_Multiple; }
  bool display(void) { return m_Display; }
  double altDistance(void) { return m_AltDistance; }
  CGrid::Unit altUnitDist(void) { return m_AltUnitDist; }
  CGrid::Unit altUnit(void) { return m_AltUnit; }
  // setters
  void setDistance(const double value) { m_Distance = value; }
  void setUnitDist(const CGrid::Unit value) { m_UnitDist = value; }
  void setUnit(const CGrid::Unit value) { m_Unit = value; }
  void setStyle(const CGrid::Style value) { m_Style = value; }
  void setMultiple(const int value) { m_Multiple = value; }
  void setDisplay(const bool value) { m_Display = value; }
  void setAltDistance(const double value) { m_AltDistance = value; }
  void setAltUnitDist(const CGrid::Unit value) { m_AltUnitDist = value; }
  void setAltUnit(const CGrid::Unit value) { m_AltUnit = value; }
 protected:
  double m_Distance; // implied
  CGrid::Unit m_UnitDist; // implied
  CGrid::Unit m_Unit; // implied
  CGrid::Style m_Style; // default = lines
  int m_Multiple; // default = 1
  bool m_Display; // display = false
  double m_AltDistance; // implied
  CGrid::Unit m_AltUnitDist; // implied
  CGrid::Unit m_AltUnit; // implied
};

class CLayer: public CEntity
{
 public:
  static const TLayer LAYER_TOP       = 1;
  static const TLayer LAYER_LAYER2    = 2;
  static const TLayer LAYER_LAYER3    = 3;
  static const TLayer LAYER_LAYER4    = 4;
  static const TLayer LAYER_LAYER5    = 5;
  static const TLayer LAYER_LAYER6    = 6;
  static const TLayer LAYER_LAYER7    = 7;
  static const TLayer LAYER_LAYER8    = 8;
  static const TLayer LAYER_LAYER9    = 9;
  static const TLayer LAYER_LAYER10   = 10;
  static const TLayer LAYER_LAYER11   = 11;
  static const TLayer LAYER_LAYER12   = 12;
  static const TLayer LAYER_LAYER13   = 13;
  static const TLayer LAYER_LAYER14   = 14;
  static const TLayer LAYER_LAYER15   = 15;
  static const TLayer LAYER_BOTTOM    = 16;
  static const TLayer LAYER_PADS      = 17;
  static const TLayer LAYER_VIAS      = 18;
  static const TLayer LAYER_UNROUTED  = 19;
  static const TLayer LAYER_DIMENSION = 20;
  static const TLayer LAYER_TPLACE    = 21;
  static const TLayer LAYER_BPLACE    = 22;
  static const TLayer LAYER_TORIGINS  = 23;
  static const TLayer LAYER_BORIGINS  = 24;
  static const TLayer LAYER_TNAMES    = 25;
  static const TLayer LAYER_BNAMES    = 26;
  static const TLayer LAYER_TVALUES   = 27;
  static const TLayer LAYER_BVALUES   = 28;
  static const TLayer LAYER_TSTOP     = 29;
  static const TLayer LAYER_BSTOP     = 30;
  static const TLayer LAYER_TCREAM    = 31;
  static const TLayer LAYER_BCREAM    = 32;
  static const TLayer LAYER_TFINISH   = 33;
  static const TLayer LAYER_BFINISH   = 34;
  static const TLayer LAYER_TGLUE     = 35;
  static const TLayer LAYER_BGLUE     = 36;
  static const TLayer LAYER_TTEST     = 37;
  static const TLayer LAYER_BTEST     = 38;
  static const TLayer LAYER_TKEEPOUT  = 39;
  static const TLayer LAYER_BKEEPOUT  = 40;
  static const TLayer LAYER_TRESTRICT = 41;
  static const TLayer LAYER_BRESTRICT = 42;
  static const TLayer LAYER_VRESTRICT = 43;
  static const TLayer LAYER_DRILLS    = 44;
  static const TLayer LAYER_HOLES     = 45;
  static const TLayer LAYER_MILLING   = 46;
  static const TLayer LAYER_MEASURES  = 47;
  static const TLayer LAYER_DOCUMENT  = 48;
  static const TLayer LAYER_REFERENCE = 49;
  static const TLayer LAYER_TDOCU     = 50;
  static const TLayer LAYER_BDOCU     = 51;
  static const TLayer LAYER_NETS      = 91;
  static const TLayer LAYER_BUSSES    = 92;
  static const TLayer LAYER_PINS      = 93;
  static const TLayer LAYER_SYMBOLS   = 94;
  static const TLayer LAYER_NAMES     = 95;
  static const TLayer LAYER_VALUES    = 96;
  static const TLayer LAYER_INFO      = 97;
  static const TLayer LAYER_GUIDE     = 98;
  //
  static const TLayer LAYER__INVALID  = 0;
  static const TLayer LAYER__FIRST    = 1;
  static const TLayer LAYER__LAST     = 98;
 public:
  CLayer(const TLayer layer, const QString& name, const int color,
         const int fill, const bool visible, const bool active);
  CLayer(const CLayer& layer);
  CLayer(void);
  virtual ~CLayer(void);
 public:
  static QString layerName(const TLayer layer);
  static TLayer layerNumber(const QString& name);
  virtual void operator =(const CLayer& layer);
  virtual void clear(void);
  virtual void assign(const CLayer& layer);
  virtual void show(std::ostream& out, const int level = 0);
  virtual bool readFromXML(const QDomElement& root);
  virtual bool writeToXML(QDomDocument& host, QDomElement& root, const bool defaults = false);
 public:
  // getters
  TLayer layer(void) { return m_Layer; }
  QString name(void) { return m_Name; }
  int color(void) { return m_Color; }
  int fill(void) { return m_Fill; }
  bool visible(void) { return m_Visible; }
  bool active(void) { return m_Active; }
  // setters
  void setLayer(const TLayer value) { m_Layer = value; }
  void setName(const QString& value) { m_Name = value; }
  void setColor(const int value) { m_Color = value; }
  void setFill(const int value) { m_Fill = value; }
  void setVisible(const bool value) { m_Visible = value; }
  void setActive(const bool value) { m_Active = value; }
 protected:
  TLayer m_Layer; // required
  QString m_Name; // required
  int m_Color; // required
  int m_Fill; // required
  bool m_Visible; // default = true
  bool m_Active; // default = true
};

class CClearance: public CEntity
{
 public:
  CClearance(const CClearance& clearance);
  CClearance(void);
  virtual ~CClearance(void);
 public:
  virtual void operator =(const CClearance& clearance);
  virtual void clear(void);
  virtual void assign(const CClearance& clearance);
  virtual void show(std::ostream& out, const int level = 0);
  virtual bool readFromXML(const QDomElement& root);
  virtual bool writeToXML(QDomDocument& host, QDomElement& root, const bool defaults = false);
 public:
  // getters
  TClass getClass(void) { return m_Class; }
  TDimension getValue(void) { return m_Value; }
  // setters
  void setClass(const TClass value) { m_Class = value; }
  void setValue(const TDimension value) { m_Value = value; }
 protected:
  TClass m_Class; // required
  TDimension m_Value; // default = 0
};

class CDescription: public CEntity
{
 public:
  CDescription(const CDescription& description);
  CDescription(void);
  virtual ~CDescription(void);
 public:
  virtual void operator =(const CDescription& description);
  virtual void clear(void);
  virtual void assign(const CDescription& description);
  virtual void show(std::ostream& out, const int level = 0);
  virtual bool readFromXML(const QDomElement& root);
  virtual bool writeToXML(QDomDocument& host, QDomElement& root, const bool defaults = false);
 public:
  // getters
  QString description(void) { return m_Description; }
  QString language(void) { return m_Language; }
  // setters
  void setDescription(const QString& value) { m_Description = value; }
  void setLanguage(const QString& value) { m_Language = value; }
 protected:
  QString m_Description;
  QString m_Language; // default = "en"
};

class CParam: public CEntity
{
 public:
  CParam(const CParam& param);
  CParam(void);
  virtual ~CParam(void);
 public:
  virtual void operator =(const CParam& param);
  virtual void clear(void);
  virtual void assign(const CParam& param);
  virtual void show(std::ostream& out, const int level = 0);
  virtual bool readFromXML(const QDomElement& root);
  virtual bool writeToXML(QDomDocument& host, QDomElement& root, const bool defaults = false);
 public:
  // getters
  QString name(void) { return m_Name; }
  QString value(void) { return m_Value; }
  // setters
  void setName(const QString value) { m_Name = value; }
  void setValue(const QString value) { m_Value = value; }
 protected:
  QString m_Name; // required
  QString m_Value; // required
};

class CApproved: public CEntity
{
 public:
  CApproved(const CApproved& error);
  CApproved(void);
  virtual ~CApproved(void);
 public:
  virtual void operator =(const CApproved& error);
  virtual void clear(void);
  virtual void assign(const CApproved& error);
  virtual void show(std::ostream& out, const int level = 0);
  virtual bool readFromXML(const QDomElement& root);
  virtual bool writeToXML(QDomDocument& host, QDomElement& root, const bool defaults = false);
 public:
  // getters
  QString hash(void) { return m_Hash; }
  // setters
  void setHash(const QString& value) { m_Hash = value; }
 protected:
  QString m_Hash; // required
};

class CPass: public CEntity
{
 public:
  CPass(const CPass& pass);
  CPass(void);
  virtual ~CPass(void);
 public:
  virtual void operator =(const CPass& pass);
  virtual void clear(void);
  virtual void assign(const CPass& pass);
  virtual void show(std::ostream& out, const int level = 0);
  virtual bool readFromXML(const QDomElement& root);
  virtual bool writeToXML(QDomDocument& host, QDomElement& root, const bool defaults = false);
 public:
  // getters
  QList<CParam*>& params(void) { return m_Params; }
  CParam* param(const int index) const { return m_Params[index]; }
  QString name(void) { return m_Name; }
  QString refer(void) { return m_Refer; }
  bool active(void) { return m_Active; }
  // setters
  void setName(const QString& value) { m_Name = value; }
  void setRefer(const QString& value) { m_Refer = value; }
  void setActive(const bool value) { m_Active = value; }
 protected:
  QList<CParam*> m_Params;
  QString m_Name; // required
  QString m_Refer; // implied
  bool m_Active; // default = true
};

class CClass: public CEntity
{
 public:
  CClass(const CClass& value);
  CClass(void);
  virtual ~CClass(void);
 public:
  virtual void operator =(const CClass& value);
  virtual void clear(void);
  virtual void assign(const CClass& value);
  virtual void show(std::ostream& out, const int level = 0);
  virtual bool readFromXML(const QDomElement& root);
  virtual bool writeToXML(QDomDocument& host, QDomElement& root, const bool defaults = false);
 public:
  // getters
  QList<CClearance*>& clearances(void) { return m_Clearances; }
  CClearance* clearance(const int index) const { return m_Clearances[index]; }
  TClass number(void) { return m_Number; }
  QString name(void) { return m_Name; }
  TDimension width(void) { return m_Width; }
  TDimension drill(void) { return m_Drill; }
  // setters
  void setNumber(const TClass value) { m_Number = value; }
  void setName(const QString& value) { m_Name = value; }
  void setWidth(const TDimension value) { m_Width = value; }
  void setDrill(const TDimension value) { m_Drill = value; }
 protected:
  QList<CClearance*> m_Clearances;
  TClass m_Number; // required
  QString m_Name; // required
  TDimension m_Width; // default = 0
  TDimension m_Drill; // default = 0
};

class CDesignRule: public CEntity
{
 public:
  CDesignRule(const CDesignRule& designRule);
  CDesignRule(void);
  virtual ~CDesignRule(void);
 public:
  virtual void operator =(const CDesignRule& designRule);
  virtual void clear(void);
  virtual void assign(const CDesignRule& designRule);
  virtual void show(std::ostream& out, const int level = 0);
  virtual bool readFromXML(const QDomElement& root);
  virtual bool writeToXML(QDomDocument& host, QDomElement& root, const bool defaults = false);
 public:
  // getters
  QList<CDescription*>& description(void) { return m_Descriptions; }
  CDescription* description(const int index) { return m_Descriptions[index]; }
  QList<CParam*>& param(void) { return m_Params; }
  CParam* param(const int index) { return m_Params[index]; }
  QString name(void) { return m_Name; }
  // setters
  void setName(const QString& value) { m_Name = value; }
 protected:
  QList<CDescription*> m_Descriptions;
  QList<CParam*> m_Params;
  QString m_Name; // required
};

// basic objects

class CVariantDef: public CEntity
{
 public:
  CVariantDef(const CVariantDef& variantDef);
  CVariantDef(void);
  virtual ~CVariantDef(void);
 public:
  virtual void operator =(const CVariantDef& variantDef);
  virtual void clear(void);
  virtual void assign(const CVariantDef& variantDef);
  virtual void show(std::ostream& out, const int level = 0);
  virtual bool readFromXML(const QDomElement& root);
  virtual bool writeToXML(QDomDocument& host, QDomElement& root, const bool defaults = false);
 public:
  // getters
  QString name(void) { return m_Name; }
  bool current(void) { return m_Current; }
  // setters
  void setName(const QString& value) { m_Name = value; }
  void setValue(const bool value) { m_Current = value; }
 protected:
  QString m_Name; // required
  bool m_Current; // default = false
};

class CVariant: public CEntity
{
 public:
  CVariant(const CVariant& variant);
  CVariant(void);
  virtual ~CVariant(void);
 public:
  virtual void operator =(const CVariant& variant);
  virtual void clear(void);
  virtual void assign(const CVariant& variant);
  virtual void show(std::ostream& out, const int level = 0);
  virtual bool readFromXML(const QDomElement& root);
  virtual bool writeToXML(QDomDocument& host, QDomElement& root, const bool defaults = false);
 public:
  // getters
  QString name(void) { return m_Name; }
  bool populate(void) { return m_Populate; }
  QString value(void) { return m_Value; }
  QString technology(void) { return m_Technology; }
  // setters
  void setName(const QString& value) { m_Name = value; }
  void setPopulate(const bool value) { m_Populate = value; }
  void setValue(const QString& value) { m_Value = value; }
  void setTechnology(const QString& value) { m_Technology = value; }
 protected:
  QString m_Name; // required
  bool m_Populate; // default = true
  QString m_Value; // implied
  QString m_Technology; // implied
};

class CGate: public CEntity
{
 public:
  enum AddLevel {alMust, alCan, alNext, alRequest, alAlways};
 public:
  CGate(const QString& name, const QString& symbol, const TCoord x, const TCoord y,
        const CGate::AddLevel addLevel, const int swapLevel);
  CGate(const CGate& gate);
  CGate(void);
  virtual ~CGate(void);
 public:
  static QString toString(const CGate::AddLevel value);
 public:
  virtual void operator =(const CGate& gate);
  virtual void clear(void);
  virtual void assign(const CGate& gate);
  virtual void scale(const double factor);
  virtual void show(std::ostream& out, const int level = 0);
  virtual bool readFromXML(const QDomElement& root);
  virtual bool writeToXML(QDomDocument& host, QDomElement& root, const bool defaults = false);
 public:
  // getters
  QString name(void) { return m_Name; }
  QString symbol(void) { return m_Symbol; }
  TCoord x(void) { return m_X; }
  TCoord y(void) { return m_Y; }
  CGate::AddLevel addLevel(void) { return m_AddLevel; }
  int swapLevel(void) { return m_SwapLevel; }
  // setters
  void setName(const QString& value) { m_Name = value; }
  void setSymbol(const QString& value) { m_Symbol = value; }
  void setX(const TCoord value) { m_X = value; }
  void setY(const TCoord value) { m_Y = value; }
  void setAddLevel(const CGate::AddLevel value) { m_AddLevel = value; }
  void setSwapLevel(const int value) { m_SwapLevel = value; }
 protected:
  QString m_Name; // required
  QString m_Symbol; // required
  TCoord m_X; // required
  TCoord m_Y; // required
  CGate::AddLevel m_AddLevel; // default = next
  int m_SwapLevel; //default = 0
};

class CWire: public CEntity
{
 public:
  enum Style {wsContinuous, wsLongDash, wsShortDash, wsDashDot};
  enum Cap {wcFlat, wcRound};
 public:
  CWire(const TCoord x1, const TCoord y1, const TCoord x2, const TCoord y2,
        const TDimension width, const double curve, const TLayer layer,
        const CWire::Style style, const CWire::Cap cap, const QString& extent);
  CWire(const CWire& wire);
  CWire(void);
  virtual ~CWire(void);
 public:
  static QString toString(const CWire::Style value);
  static QString toString(const CWire::Cap value);
 public:
  virtual void operator =(const CWire& wire);
  virtual void clear(void);
  virtual void assign(const CWire& wire);
  virtual void scale(const double factor);
  virtual void show(std::ostream& out, const int level = 0);
  virtual bool readFromXML(const QDomElement& root);
  virtual bool writeToXML(QDomDocument& host, QDomElement& root, const bool defaults = false);
 public:
  // getters
  TCoord x1(void) { return m_X1; }
  TCoord y1(void) { return m_Y1; }
  TCoord x2(void) { return m_X2; }
  TCoord y2(void) { return m_Y2; }
  TDimension width(void) { return m_Width; }
  TLayer layer(void) { return m_Layer; }
  QString extent(void) { return m_Extent; }
  CWire::Style style(void) { return m_Style; }
  double curve(void) { return m_Curve; }
  CWire::Cap cap(void) { return m_Cap; }
  // setters
  void setX1(const TCoord value) { m_X1 = value; }
  void setY1(const TCoord value) { m_Y1 = value; }
  void setX2(const TCoord value) { m_X2 = value; }
  void setY2(const TCoord value) { m_Y2 = value; }
  void setWidth(const TDimension value) { m_Width = value; }
  void setLayer(const TLayer value) { m_Layer = value; }
  void setExtent(const QString& value) { m_Extent = value; }
  void setStyle(const CWire::Style value) { m_Style = value; }
  void setCurve(const double value) { m_Curve = value; }
  void setCap(const CWire::Cap value) { m_Cap = value; }
  //
  double chord(void) const;
  double length(void) const;
  double radius(void) const;
 protected:
  TCoord m_X1; // required
  TCoord m_Y1; // required
  TCoord m_X2; // required
  TCoord m_Y2; // required
  TDimension m_Width; // required
  TLayer m_Layer; // required
  QString m_Extent; // implied, the layers an airwire extends through, given as "topmost-bottompost"
  CWire::Style m_Style; // default = continuous
  double m_Curve; // default = 0
  CWire::Cap m_Cap; // default = round, only applicable if m_Curve > 0
};

class CDimension: public CEntity
{
 public:
  enum Type {dtParallel, dtHorizontal, dtVertical, dtRadius, dtDiameter, dtLeader};
 public:
  CDimension(const CDimension& dimension);
  CDimension(void);
  virtual ~CDimension(void);
 public:
  static QString toString(const CDimension::Type value);
 public:
  virtual void operator =(const CDimension& dimension);
  virtual void clear(void);
  virtual void assign(const CDimension& dimension);
  virtual void scale(const double factor);
  virtual void show(std::ostream& out, const int level = 0);
  virtual bool readFromXML(const QDomElement& root);
  virtual bool writeToXML(QDomDocument& host, QDomElement& root, const bool defaults = false);
 public:
  static const int DEFAULT_EXT_WIDTH = 0;
  static const int DEFAULT_EXT_LENGTH = 0;
  static const int DEFAULT_EXT_OFFSET = 0;
  static const int DEFAULT_TEXT_RATIO = 8;
  static const CGrid::Unit DEFAULT_GRID_UNIT = CGrid::guMM;
  static const int DEFAULT_PRECISION = 2;
 public:
  // getters
  TCoord x1(void) { return m_X1; }
  TCoord y1(void) { return m_Y1; }
  TCoord x2(void) { return m_X2; }
  TCoord y2(void) { return m_Y2; }
  TCoord x3(void) { return m_X3; }
  TCoord y3(void) { return m_Y3; }
  TLayer layer(void) { return m_Layer; }
  CDimension::Type dimType(void) { return m_DType; }
  TDimension width(void) { return m_Width; }
  TDimension extWidth(void) { return m_ExtWidth; }
  TDimension extLength(void) { return m_ExtLength; }
  TDimension extOffset(void) { return m_ExtOffset; }
  TDimension textSize(void) { return m_TextSize; }
  int textRatio(void) { return m_TextRatio; }
  CGrid::Unit gridUnit(void) { return m_GridUnit; }
  int precision(void) { return m_Precision; }
  bool visible(void) { return m_Visible; }
  // setters
  void setX1(const TCoord value) { m_X1 = value; }
  void setY1(const TCoord value) { m_Y1 = value; }
  void setX2(const TCoord value) { m_X2 = value; }
  void setY2(const TCoord value) { m_Y2 = value; }
  void setX3(const TCoord value) { m_X3 = value; }
  void setY3(const TCoord value) { m_Y3 = value; }
  void setLayer(const TLayer value) { m_Layer = value; }
  void setDimType(const CDimension::Type value) { m_DType = value; }
  void setWidth(const TDimension value) { m_Width = value; }
  void setExtWidth(const TDimension value) { m_ExtWidth = value; }
  void setExtLength(const TDimension value) { m_ExtLength = value; }
  void setExtOffset(const TDimension value) { m_ExtOffset = value; }
  void setTextSize(const TDimension value) { m_TextSize = value; }
  void setTextRatio(const int value) { m_TextRatio = value; }
  void setGridUnit(const CGrid::Unit value) { m_GridUnit = value; }
  void setPrecision(const int value) { m_Precision = value; }
  void setVisible(const bool value) { m_Visible = value; }
 protected:
  TCoord m_X1; // required
  TCoord m_Y1; // required
  TCoord m_X2; // required
  TCoord m_Y2; // required
  TCoord m_X3; // required
  TCoord m_Y3; // required
  TLayer m_Layer; // required
  CDimension::Type m_DType; // default = parallel
  TDimension m_Width; // required
  TDimension m_ExtWidth; // default = 0
  TDimension m_ExtLength; // default = 0
  TDimension m_ExtOffset; // default = 0
  TDimension m_TextSize; // required
  int m_TextRatio; // default = 8
  CGrid::Unit m_GridUnit; // default == mm
  int m_Precision; // default = 2
  bool m_Visible; // default = false
};

class CText: public CEntity
{
 public:
  enum Font {tfVector, tfProportional, tfFixed};
  enum Align {taBottomLeft, taBottomCenter, taBottomRight,
              taCenterLeft, taCenter, taCenterRight,
              taTopLeft, taTopCenter, taTopRight};
 public:
  CText(const QString& text, const TCoord x, const TCoord y,
        const TDimension size, const TLayer layer,
        const CText::Font font = CText::DEFAULT_FONT,
        const int ratio = CText::DEFAULT_RATIO,
        const double rotation = CText::DEFAULT_ROTATION,
        const bool reflection = false,
        const bool spin = false,
        const CText::Align align = CText::DEFAULT_ALIGN,
        const int distance = CText::DEFAULT_DISTANCE);
  CText(const CText& text);
  CText(void);
  virtual ~CText(void);
 public:
  static QString toString(const CText::Font value);
  static QString toString(const CText::Align value);
  static CText::Align fromString(const QString& value);
 public:
  virtual void operator =(const CText& text);
  virtual void clear(void);
  virtual void assign(const CText& text);
  virtual void scale(const double factor);
  virtual void show(std::ostream& out, const int level = 0);
  virtual bool readFromXML(const QDomElement& root);
  virtual bool writeToXML(QDomDocument& host, QDomElement& root, const bool defaults = false);
 public:
  static const CText::Font DEFAULT_FONT = CText::tfProportional;
  static const int DEFAULT_RATIO = 8;
  static const double DEFAULT_ROTATION = 0.0;
  static const CText::Align DEFAULT_ALIGN = CText::taBottomLeft;
  static const int DEFAULT_DISTANCE = 50;
 public:
  // getters
  QString text(void) const { return m_Text; }
  TCoord x(void) const { return m_X; }
  TCoord y(void) const { return m_Y; }
  TDimension size(void) const { return m_Size; }
  TLayer layer(void) const { return m_Layer; }
  CText::Font font(void) const { return m_Font; }
  int ratio(void) const { return m_Ratio; }
  double rotation(void) const { return m_Rotation; }
  bool reflection(void) const { return m_Reflection; }
  bool spin(void) const { return m_Spin; }
  CText::Align align(void) const { return m_Align; }
  int distance(void) const { return m_Distance; }
  // setters
  void setText(const QString& value) { m_Text = value; }
  void setX(const TCoord value) { m_X = value; }
  void setY(const TCoord value) { m_Y = value; }
  void setSize(const TDimension value) { m_Size = value; }
  void setLayer(const TLayer value) { m_Layer = value; }
  void setFont(const CText::Font value) { m_Font = value; }
  void setRatio(const int value) { m_Ratio = value; }
  void setRotation(const double value) { m_Rotation = std::min(std::max(value,0.0),359.999); }
  void setReflection(const bool value) { m_Reflection = value; }
  void setSpin(const bool value) { m_Spin = value; }
  void setAlign(const CText::Align value) { m_Align = value; }
  void setDistance(const int value) { m_Distance = value; }
 protected:
  QString m_Text;
  TCoord m_X; // required
  TCoord m_Y; // required
  TDimension m_Size; // required
  TLayer m_Layer; // required
  CText::Font m_Font; // default = proportional
  int m_Ratio; // default = 8
  double m_Rotation; // default = 0, valid 0 .. 359.9(9)
  bool m_Reflection; // default = false;
  bool m_Spin; // default = false;
  CText::Align m_Align; // default = bottom-left
  int m_Distance; // default = 50
};

class CCircle: public CEntity
{
 public:
  CCircle(const CCircle& circle);
  CCircle(void);
  virtual ~CCircle(void);
 public:
  virtual void operator =(const CCircle& circle);
  virtual void clear(void);
  virtual void assign(const CCircle& circle);
  virtual void scale(const double factor);
  virtual void show(std::ostream& out, const int level = 0);
  virtual bool readFromXML(const QDomElement& root);
  virtual bool writeToXML(QDomDocument& host, QDomElement& root, const bool defaults = false);
 public:
  // getters
  TCoord x(void) { return m_X; }
  TCoord y(void) { return m_Y; }
  TCoord radius(void) { return m_Radius; }
  TDimension width(void) { return m_Width; }
  TLayer layer(void) { return m_Layer; }
  // setters
  void setX(const TCoord value) { m_X = value; }
  void setY(const TCoord value) { m_Y = value; }
  void setRadius(const TCoord value) { m_Radius = value; }
  void setWidth(const TDimension value) { m_Width = value; }
  void setLayer(const TLayer value) { m_Layer = value; }
 protected:
  TCoord m_X; // required
  TCoord m_Y; // required
  TCoord m_Radius; // required
  TDimension m_Width; // required
  TLayer m_Layer; // required
};

class CRectangle: public CEntity
{
 public:
  CRectangle(const CRectangle& rectangle);
  CRectangle(void);
  virtual ~CRectangle(void);
 public:
  virtual void operator =(const CRectangle& rectangle);
  virtual void clear(void);
  virtual void assign(const CRectangle& rectangle);
  virtual void scale(const double factor);
  virtual void show(std::ostream& out, const int level = 0);
  virtual bool readFromXML(const QDomElement& root);
  virtual bool writeToXML(QDomDocument& host, QDomElement& root, const bool defaults = false);
 public:
  static const double DEFAULT_ROTATION = 0.0;
 public:
  // getters
  TCoord x1(void) { return m_X1; }
  TCoord y1(void) { return m_Y1; }
  TCoord x2(void) { return m_X2; }
  TCoord y2(void) { return m_Y2; }
  TLayer layer(void) { return m_Layer; }
  double rotation(void) { return m_Rotation; }
  // setters
  void setX1(const TCoord value) { m_X1 = value; }
  void setY1(const TCoord value) { m_Y1 = value; }
  void setX2(const TCoord value) { m_X2 = value; }
  void setY2(const TCoord value) { m_Y2 = value; }
  void setLayer(const TLayer value) { m_Layer = value; }
  void setRotation(const double value) { m_Rotation = std::min(std::max(value,0.0),359.999); }
 protected:
  TCoord m_X1; // required
  TCoord m_Y1; // required
  TCoord m_X2; // required
  TCoord m_Y2; // required
  TLayer m_Layer; // required
  double m_Rotation; // default = 0, valid 0 .. 359.9(9)
};

class CFrame: public CEntity
{
 public:
  CFrame(const CFrame& frame);
  CFrame(void);
  virtual ~CFrame(void);
 public:
  virtual void operator =(const CFrame& frame);
  virtual void clear(void);
  virtual void assign(const CFrame& frame);
  virtual void scale(const double factor);
  virtual void show(std::ostream& out, const int level = 0);
  virtual bool readFromXML(const QDomElement& root);
  virtual bool writeToXML(QDomDocument& host, QDomElement& root, const bool defaults = false);
 public:
  // getters
  TCoord x1(void) { return m_X1; }
  TCoord y1(void) { return m_Y1; }
  TCoord x2(void) { return m_X2; }
  TCoord y2(void) { return m_Y2; }
  int columns(void) { return m_Columns; }
  int rows(void) { return m_Rows; }
  TLayer layer(void) { return m_Layer; }
  bool borderLeft(void) { return m_BorderLeft; }
  bool borderTop(void) { return m_BorderTop; }
  bool borderRight(void) { return m_BorderRight; }
  bool borderBottom(void) { return m_BorderBottom; }
  // setters
  void setX1(const TCoord value) { m_X1 = value; }
  void setY1(const TCoord value) { m_Y1 = value; }
  void setX2(const TCoord value) { m_X2 = value; }
  void setY2(const TCoord value) { m_Y2 = value; }
  void setColumns(const int value) { m_Columns = value; }
  void setRows(const int value) { m_Rows = value; }
  void setLayer(const TLayer value) { m_Layer = value; }
  void setBorderLeft(const bool value) { m_BorderLeft = value; }
  void setBorderTop(const bool value) { m_BorderTop = value; }
  void setBorderRight(const bool value) { m_BorderRight = value; }
  void setBorderBottom(const bool value) { m_BorderBottom = value; }
 protected:
  TCoord m_X1; // required
  TCoord m_Y1; // required
  TCoord m_X2; // required
  TCoord m_Y2; // required
  int m_Columns; // required
  int m_Rows; // required
  TLayer m_Layer; // required
  bool m_BorderLeft; // default = true
  bool m_BorderTop; // default = true
  bool m_BorderRight; // default = true
  bool m_BorderBottom; // default = true
};

class CHole: public CEntity
{
 public:
  CHole(const CHole& hole);
  CHole(void);
  virtual ~CHole(void);
 public:
  virtual void operator =(const CHole& hole);
  virtual void clear(void);
  virtual void assign(const CHole& hole);
  virtual void scale(const double factor);
  virtual void show(std::ostream& out, const int level = 0);
  virtual bool readFromXML(const QDomElement& root);
  virtual bool writeToXML(QDomDocument& host, QDomElement& root, const bool defaults = false);
 public:
  // getters
  TCoord x(void) { return m_X; }
  TCoord y(void) { return m_Y; }
  TDimension drill(void) { return m_Drill; }
  // setters
  void setX(const TCoord value) { m_X = value; }
  void setY(const TCoord value) { m_Y = value; }
  void setDrill(const TDimension value) { m_Drill = value; }
 protected:
  TCoord m_X; // required
  TCoord m_Y; // required
  TDimension m_Drill; // required
};

class CPad: public CEntity
{
 public:
  enum Shape {psSquare, psRound, psOctagon, psLong, psOffset};
 public:
  CPad(const CPad& pad);
  CPad(void);
  virtual ~CPad(void);
 public:
  static QString toString(const CPad::Shape value);
 public:
  virtual void operator =(const CPad& pad);
  virtual void clear(void);
  virtual void assign(const CPad& pad);
  virtual void scale(const double factor);
  virtual void show(std::ostream& out, const int level = 0);
  virtual bool readFromXML(const QDomElement& root);
  virtual bool writeToXML(QDomDocument& host, QDomElement& root, const bool defaults = false);
 public:
  static const TDimension DEFAULT_DIAMETER = 0.0;
  static const CPad::Shape DEFAULT_SHAPE = CPad::psRound;
  static const double DEFAULT_ROTATION = 0.0;
 public:
  // getters
  QString name(void) { return m_Name; }
  TCoord x(void) { return m_X; }
  TCoord y(void) { return m_Y; }
  TDimension drill(void) { return m_Drill; }
  TDimension diameter(void) { return m_Diameter; }
  CPad::Shape shape(void) { return m_Shape; }
  double rotation(void) { return m_Rotation; }
  bool stop(void) { return m_Stop; }
  bool thermals(void) { return m_Thermals; }
  bool first(void) { return m_First; }
  // setters
  void setName(const QString& value) { m_Name = value; }
  void setX(const TCoord value) { m_X = value; }
  void setY(const TCoord value) { m_Y = value; }
  void setDrill(const TDimension value) { m_Drill = value; }
  void setDiameter(const TDimension value) { m_Diameter = value; }
  void setShape(const CPad::Shape value) { m_Shape = value; }
  void setRotation(const double value) { m_Rotation = std::min(std::max(value,0.0),359.999); }
  void setStop(const bool value) { m_Stop = value; }
  void setThermals(const bool value) { m_Thermals = value; }
  void setFirst(const bool value) { m_First = value; }
 protected:
  QString m_Name; // required
  TCoord m_X; // required
  TCoord m_Y; // required
  TDimension m_Drill; // required
  TDimension m_Diameter; // default = 0
  CPad::Shape m_Shape; // default = round
  double m_Rotation; // default = 0
  bool m_Stop; // default = true
  bool m_Thermals; // default = true
  bool m_First; // default = false
};

class CSMD: public CEntity
{
 public:
  CSMD(const QString& name, const TCoord x, const TCoord y,
   const TDimension dx, const TDimension dy, const TLayer layer,
   const int roundness, const double rotation, const bool stop,
   const bool thermals, const bool cream);
  CSMD(const CSMD& smd);
  CSMD(void);
  virtual ~CSMD(void);
 public:
  virtual void operator =(const CSMD& smd);
  virtual void clear(void);
  virtual void assign(const CSMD& smd);
  virtual void scale(const double factor);
  virtual void show(std::ostream& out, const int level = 0);
  virtual bool readFromXML(const QDomElement& root);
  virtual bool writeToXML(QDomDocument& host, QDomElement& root, const bool defaults = false);
 public:
  static const int DEFAULT_ROUNDNESS = 0;
  static const double DEFAULT_ROTATION = 0.0;
 public:
  // getters
  QString name(void) { return m_Name; }
  TCoord x(void) { return m_X; }
  TCoord y(void) { return m_Y; }
  TDimension dx(void) { return m_DX; }
  TDimension dy(void) { return m_DY; }
  TLayer layer(void) { return m_Layer; }
  int roundness(void) { return m_Roundness; }
  double rotation(void) { return m_Rotation; }
  bool stop(void) { return m_Stop; }
  bool thermals(void) { return m_Thermals; }
  bool cream(void) { return m_Cream; }
  // setters
  void setName(const QString& value) { m_Name = value; }
  void setX(const TCoord value) { m_X = value; }
  void setY(const TCoord value) { m_Y = value; }
  void setDX(const TDimension value) { m_DX = value; }
  void setDY(const TDimension value) { m_DY = value; }
  void setLayer(const TLayer value) { m_Layer = value; }
  void setRoundness(const int value) { m_Roundness = value; }
  void setRotation(const double value) { m_Rotation = std::min(std::max(value,0.0),359.999); }
  void setStop(const bool value) { m_Stop = value; }
  void setThermals(const bool value) { m_Thermals = value; }
  void setCream(const bool value) { m_Cream = value; }
 protected:
  QString m_Name; // required
  TCoord m_X; // required
  TCoord m_Y; // required
  TDimension m_DX; // required
  TDimension m_DY; // required
  TLayer m_Layer; // required
  int m_Roundness; // default = 0
  double m_Rotation; // default = 0
  bool m_Stop; // default = true
  bool m_Thermals; // default = true
  bool m_Cream; // default = false
};

class CVia: public CEntity
{
 public:
  enum Shape {vsSquare, vsRound, vsOctagon};
 public:
  CVia(const CVia& via);
  CVia(void);
  virtual ~CVia(void);
 public:
  static QString toString(const CVia::Shape value);
 public:
  virtual void operator =(const CVia& via);
  virtual void clear(void);
  virtual void assign(const CVia& via);
  virtual void scale(const double factor);
  virtual void show(std::ostream& out, const int level = 0);
  virtual bool readFromXML(const QDomElement& root);
  virtual bool writeToXML(QDomDocument& host, QDomElement& root, const bool defaults = false);
 public:
  static const TDimension DEFAULT_DIAMETER = 0.0;
  static const CVia::Shape DEFAULT_SHAPE = CVia::vsRound;
 public:
  // getters
  TCoord x(void) { return m_X; }
  TCoord y(void) { return m_Y; }
  QString extent(void) { return m_Extent; }
  TDimension drill(void) { return m_Drill; }
  TDimension diameter(void) { return m_Diameter; }
  CVia::Shape shape(void) { return m_Shape; }
  bool alwaysStop(void) { return m_AlwaysStop; }
  // setters
  void setX(const TCoord value) { m_X = value; }
  void setY(const TCoord value) { m_Y = value; }
  void setExtent(const QString value) { m_Extent = value; }
  void setDrill(const TDimension value) { m_Drill = value; }
  void setDiameter(const TDimension value) { m_Diameter = value; }
  void setShape(const CVia::Shape value) { m_Shape = value; }
  void setAlwaysStop(const bool value) { m_AlwaysStop = value; }
 protected:
  TCoord m_X; // required
  TCoord m_Y; // required
  QString m_Extent; // required, the layers an via extends through, given as "topmost-bottompost"
  TDimension m_Drill; // required
  TDimension m_Diameter; // default = 0
  CVia::Shape m_Shape; // default = round
  bool m_AlwaysStop; // default = false
};

class CVertex: public CEntity
{
 public:
  CVertex(const TCoord x, const TCoord y, const double curve);
  CVertex(const CVertex& vertex);
  CVertex(void);
  virtual ~CVertex(void);
 public:
  virtual void operator =(const CVertex& vertex);
  virtual void clear(void);
  virtual void assign(const CVertex& vertex);
  virtual void scale(const double factor);
  virtual void show(std::ostream& out, const int level = 0);
  virtual bool readFromXML(const QDomElement& root);
  virtual bool writeToXML(QDomDocument& host, QDomElement& root, const bool defaults = false);
 public:
  static const double DEFAULT_CURVE = 0.0;
 public:
  // getters
  TCoord x(void) { return m_X; }
  TCoord y(void) { return m_Y; }
  double curve(void) { return m_Curve; }
  // setters
  void setX(const TCoord value) { m_X = value; }
  void setY(const TCoord value) { m_Y = value; }
  void setCurve(const double value) { m_Curve = std::min(std::max(value,-359.999),359.999); }
 protected:
  TCoord m_X; // required
  TCoord m_Y; // required
  double m_Curve; // default = 0, valid -359.9(9) .. +359.9(9)
};

class CPin: public CEntity
{
 public:
  enum Visible {pvOff, pvPad, pvPin, pvBoth};
  enum Length {plPoint, plShort, plMiddle, plLong};
  enum Direction {pdNC, pdIN, pdOUT, pdIO, pdOC, pdPWR, pdPAS, pdHIZ, pdSUP};
  enum Function {pfNone, pfDot, pfClk, pfDotClk};
 public:
  CPin(const CPin& pin);
  CPin(void);
  virtual ~CPin(void);
 public:
  static QString toString(const CPin::Visible value);
  static QString toString(const CPin::Length value);
  static QString toString(const CPin::Direction value);
  static QString toString(const CPin::Function value);
 public:
  virtual void operator =(const CPin& pin);
  virtual void clear(void);
  virtual void assign(const CPin& pin);
  virtual void scale(const double factor);
  virtual void show(std::ostream& out, const int level = 0);
  virtual bool readFromXML(const QDomElement& root);
  virtual bool writeToXML(QDomDocument& host, QDomElement& root, const bool defaults = false);
 public:
  static const CPin::Visible DEFAULT_VISIBLE = CPin::pvBoth;
  static const CPin::Length DEFAULT_LENGTH = CPin::plLong;
  static const CPin::Direction DEFAULT_DIRECTION = CPin::pdIO;
  static const CPin::Function DEFAULT_FUNCTION = CPin::pfNone;
  static const int DEFAULT_SWAPLEVEL = 0;
  static const double DEFAULT_ROTATION = 0.0;
 public:
  // getters
  QString name(void) { return m_Name; }
  TCoord x(void) { return m_X; }
  TCoord y(void) { return m_Y; }
  CPin::Visible visible(void) { return m_Visible; }
  CPin::Length length(void) { return m_Length; }
  CPin::Direction direction(void) { return m_Direction; }
  CPin::Function function(void) { return m_Function; }
  int swapLevel(void) { return m_SwapLevel; }
  double rotation(void) { return m_Rotation; }
  // setters
  void setName(const QString& value) { m_Name = value; }
  void setX(const TCoord value) { m_X = value; }
  void setY(const TCoord value) { m_Y = value; }
  void setVisible(const CPin::Visible value) { m_Visible = value; }
  void setLength(const CPin::Length value) { m_Length = value; }
  void setDirection(const CPin::Direction value) { m_Direction = value; }
  void setFunction(const CPin::Function value) { m_Function = value; }
  void setSwapLevel(const int value) { m_SwapLevel = value; }
  void setRotation(const double value) { m_Rotation = std::min(std::max(value,0.0),359.999); }
 protected:
  QString m_Name; // required
  TCoord m_X; // required
  TCoord m_Y; // required
  CPin::Visible m_Visible; // default = both
  CPin::Length m_Length; // default = long
  CPin::Direction m_Direction; // default = io
  CPin::Function m_Function; // default = none
  int m_SwapLevel; // default = 0
  double m_Rotation; // default = 0
};

class CLabel: public CEntity
{
 public:
  CLabel(const CLabel& label);
  CLabel(void);
  virtual ~CLabel(void);
 public:
  virtual void operator =(const CLabel& label);
  virtual void clear(void);
  virtual void assign(const CLabel& label);
  virtual void scale(const double factor);
  virtual void show(std::ostream& out, const int level = 0);
  virtual bool readFromXML(const QDomElement& root);
  virtual bool writeToXML(QDomDocument& host, QDomElement& root, const bool defaults = false);
 public:
  static const CText::Font DEFAULT_FONT = CText::tfProportional;
  static const int DEFAULT_RATIO = 8;
  static const double DEFAULT_ROTATION = 0.0;
 public:
  // getters
  TCoord x(void) { return m_X; }
  TCoord y(void) { return m_Y; }
  TDimension size(void) { return m_Size; }
  TLayer layer(void) { return m_Layer; }
  CText::Font fone(void) { return m_Font; }
  int ratio(void) { return m_Ratio; }
  double rotation(void) { return m_Rotation; }
  bool xref(void) { return m_XRef; }
  // setters
  void setX(const TCoord value) { m_X = value; }
  void setY(const TCoord value) { m_Y = value; }
  void setSize(const TDimension value) { m_Size = value; }
  void setLayer(const TLayer value) { m_Layer = value; }
  void setFont(const CText::Font value) { m_Font = value; }
  void setRatio(const int value) { m_Ratio = value; }
  void setRotation(const double value);
  void setXRef(const bool value) { m_XRef = value; }
 protected:
  TCoord m_X; // required
  TCoord m_Y; // required
  TDimension m_Size; // required
  TLayer m_Layer; // required
  CText::Font m_Font; // default = proportional
  int m_Ratio; // default = 8
  double m_Rotation; // default = 0, valid 0,90,180,270
  bool m_Reflection; // default = false
  bool m_XRef; // default = false, only in <net> context
};

class CJunction: public CEntity
{
 public:
  CJunction(const CJunction& junction);
  CJunction(void);
  virtual ~CJunction(void);
 public:
  virtual void operator =(const CJunction& junction);
  virtual void clear(void);
  virtual void assign(const CJunction& junction);
  virtual void scale(const double factor);
  virtual void show(std::ostream& out, const int level = 0);
  virtual bool readFromXML(const QDomElement& root);
  virtual bool writeToXML(QDomDocument& host, QDomElement& root, const bool defaults = false);
 public:
  // getters
  TCoord x(void) { return m_X; }
  TCoord y(void) { return m_Y; }
  // setters
  void setX(const TCoord value) { m_X = value; }
  void setY(const TCoord value) { m_Y = value; }
 protected:
  TCoord m_X; // required
  TCoord m_Y; // required
};

class CConnect: public CEntity
{
 public:
  enum Route {crAll, crAny};
 public:
  CConnect(const CConnect& connect);
  CConnect(void);
  virtual ~CConnect(void);
 public:
  static QString toString(const CConnect::Route value);
 public:
  virtual void operator =(const CConnect& connect);
  virtual void clear(void);
  virtual void assign(const CConnect& connect);
  virtual void show(std::ostream& out, const int level = 0);
  virtual bool readFromXML(const QDomElement& root);
  virtual bool writeToXML(QDomDocument& host, QDomElement& root, const bool defaults = false);
 public:
  static const CConnect::Route DEFAULT_ROUTE = crAll;
 public:
  // getters
  QString gate(void) { return m_Gate; }
  QString pin(void) { return m_Pin; }
  QString pad(void) { return m_Pad; }
  CConnect::Route route(void) { return m_Route; }
  // setters
  void setGate(const QString& value) { m_Gate = value; }
  void setPin(const QString& value) { m_Pin = value; }
  void setPad(const QString& value) { m_Pad = value; }
  void setRoute(const CConnect::Route& value) { m_Route = value; }
 protected:
  QString m_Gate; // required
  QString m_Pin; // required
  QString m_Pad; // required
  CConnect::Route m_Route; // default = all
};

class CAttribute: public CEntity
{
 public:
  enum Display {adOff, adValue, adName, adBoth};
 public:
  CAttribute(const CAttribute& attribute);
  CAttribute(void);
  virtual ~CAttribute(void);
 public:
  static QString toString(const CAttribute::Display value);
 public:
  virtual void operator =(const CAttribute& attribute);
  virtual void clear(void);
  virtual void assign(const CAttribute& attribute);
  virtual void scale(const double factor);
  virtual void show(std::ostream& out, const int level = 0);
  virtual bool readFromXML(const QDomElement& root);
  virtual bool writeToXML(QDomDocument& host, QDomElement& root, const bool defaults = false);
 public:
  static const double DEFAULT_ROTATION = 0.0;
  static const CAttribute::Display DEFAULT_DISPLAY = CAttribute::adValue;
 public:
  // getters
  QString name(void) { return m_Name; }
  QString value(void) { return m_Value; }
  TCoord x(void) { return m_X; }
  TCoord y(void) { return m_Y; }
  TDimension size(void) { return m_Size; }
  TLayer layer(void) { return m_Layer; }
  CText::Font font(void) { return m_Font; }
  int ratio(void) { return m_Ratio; }
  double rotation(void) { return m_Rotation; }
  CAttribute::Display display(void) { return m_Display; }
  bool constant(void) { return m_Constant; }
  // setters
  void setName(const QString& value) { m_Name = value; }
  void setValue(const QString& value) { m_Value = value; }
  void setX(const TCoord value) { m_X = value; }
  void setY(const TCoord value) { m_Y = value; }
  void setSize(const TDimension value) { m_Size = value; }
  void setLayer(const TLayer value) { m_Layer = value; }
  void setFont(const CText::Font value) { m_Font = value; }
  void setRatio(const int value) {  m_Ratio = value; }
  void setRotation(const double value) { m_Rotation = std::min(std::max(value,0.0),359.999); }
  void setDisplay(const CAttribute::Display value) { m_Display = value; }
  void setConstant(const bool value) { m_Constant = value; }
 protected:
  QString m_Name; // required
  QString m_Value; // implied
  TCoord m_X; // implied
  TCoord m_Y; // implied
  TDimension m_Size; // implied
  TLayer m_Layer; // implied
  CText::Font m_Font; // implied
  int m_Ratio; // implied
  double m_Rotation; // default = 0, valid 0 .. 359.9(9)
  bool m_Reflection;
  CText::Align m_Align;
  CAttribute::Display m_Display; // default = value
  bool m_Constant; // default = false
};

class CPinRef: public CEntity
{
 public:
  CPinRef(const CPinRef& pinRef);
  CPinRef(void);
  virtual ~CPinRef(void);
 public:
  virtual void operator =(const CPinRef& pinRef);
  virtual void clear(void);
  virtual void assign(const CPinRef& pinRef);
  virtual void show(std::ostream& out, const int level = 0);
  virtual bool readFromXML(const QDomElement& root);
  virtual bool writeToXML(QDomDocument& host, QDomElement& root, const bool defaults = false);
 public:
  // getters
  QString part(void) { return m_Part; }
  QString gate(void) { return m_Gate; }
  QString pin(void) { return m_Pin; }
  // setters
  void part(const QString& value) { m_Part = value; }
  void gate(const QString& value) { m_Gate = value; }
  void pin(const QString& value) { m_Pin = value; }
 protected:
  QString m_Part; // required
  QString m_Gate; // required
  QString m_Pin; // required
};

class CContactRef: public CEntity
{
 public:
  CContactRef(const CContactRef& contactRef);
  CContactRef(void);
  virtual ~CContactRef(void);
 public:
  virtual void operator =(const CContactRef& contactRef);
  virtual void clear(void);
  virtual void assign(const CContactRef& contactRef);
  virtual void show(std::ostream& out, const int level = 0);
  virtual bool readFromXML(const QDomElement& root);
  virtual bool writeToXML(QDomDocument& host, QDomElement& root, const bool defaults = false);
 public:
  static const CConnect::Route DEFAULT_ROUTE = CConnect::crAll;
 public:
  // getters
  QString element(void) { return m_Element; }
  QString pad(void) { return m_Pad; }
  CConnect::Route route(void) { return m_Route; }
  QString routeTag(void) { return m_RouteTag; }
  // setters
  void setEelement(const QString& value) { m_Element = value; }
  void setPad(const QString& value) { m_Pad = value; }
  void setRoute(const CConnect::Route value) { m_Route = value; }
  void routeTag(const QString& value) { m_RouteTag = value; }
 protected:
  QString m_Element; // required
  QString m_Pad; // required
  CConnect::Route m_Route; // default = all
  QString m_RouteTag; // default = ""
};

class CTechnology: public CEntity
{
 public:
  CTechnology(const CTechnology& technology);
  CTechnology(void);
  virtual ~CTechnology(void);
 public:
  virtual void operator =(const CTechnology& technology);
  virtual void clear(void);
  virtual void assign(const CTechnology& technology);
  virtual void scale(const double factor);
  virtual void show(std::ostream& out, const int level = 0);
  virtual bool readFromXML(const QDomElement& root);
  virtual bool writeToXML(QDomDocument& host, QDomElement& root, const bool defaults = false);
 public:
  // getters
  QList<CAttribute*>& attributes(void) { return m_Attributes; }
  QString name(void) { return m_Name; }
  // setters
  void setName(const QString& value) { m_Name = value; }
 protected:
  QList<CAttribute*> m_Attributes;
  QString m_Name; // required
};

class CInstance: public CEntity
{
 public:
  CInstance(const CInstance& instance);
  CInstance(void);
  virtual ~CInstance(void);
 public:
  virtual void operator =(const CInstance& instance);
  virtual void clear(void);
  virtual void assign(const CInstance& instance);
  virtual void scale(const double factor);
  virtual void show(std::ostream& out, const int level = 0);
  virtual bool readFromXML(const QDomElement& root);
  virtual bool writeToXML(QDomDocument& host, QDomElement& root, const bool defaults = false);
 public:
  static const double DEFAULT_ROTATION = 0.0;
 public:
  // getters
  QList<CAttribute*>& attributes(void) { return m_Attributes; }
  QString part(void) { return m_Part; }
  QString gate(void) { return m_Gate; }
  TCoord x(void) { return m_X; }
  TCoord y(void) { return m_Y; }
  bool smashed(void) { return m_Smashed; }
  double rotation(void) { return m_Rotation; } // default = 0, valid 0,90,180,270
  bool reflection(void) { return m_Reflection; }
  bool spin(void) { return m_Spin; }
  // setters
  void setPart(const QString& value) { m_Part = value; }
  void setGate(const QString& value) { m_Gate = value; }
  void setX(const TCoord value) { m_X = value; }
  void setY(const TCoord value) { m_Y = value; }
  void setSmashed(const bool value) { m_Smashed = value; }
  void setRotation(const double value);
  void setReflection(const bool value) { m_Reflection = value; }
  void setSpin(const bool value) { m_Spin = value; }
 protected:
  QList<CAttribute*> m_Attributes;
  QString m_Part; // required
  QString m_Gate; // required
  TCoord m_X; // required
  TCoord m_Y; // required
  bool m_Smashed; // default = false;
  double m_Rotation; // default = 0, valid 0,90,180,270
  bool m_Reflection; //default = false;
  bool m_Spin; //default = false;
};

class CPart: public CEntity
{
 public:
  CPart(const CPart& part);
  CPart(void);
  virtual ~CPart(void);
 public:
  virtual void operator =(const CPart& part);
  virtual void clear(void);
  virtual void assign(const CPart& part);
  virtual void scale(const double factor);
  virtual void show(std::ostream& out, const int level = 0);
  virtual bool readFromXML(const QDomElement& root);
  virtual bool writeToXML(QDomDocument& host, QDomElement& root, const bool defaults = false);
 public:
  // getters
  QList<CAttribute*>& attributes(void) { return m_Attributes; }
  QList<CVariant*>& variants(void) { return m_Variants; }
  QString name(void) { return m_Name; }
  QString library(void) { return m_Library; }
  QString deviceSet(void) { return m_DeviceSet; }
  QString device(void) { return m_Device; }
  QString technology(void) { return m_Technology; }
  QString value(void) { return m_Value; }
  // setters
  void setName(const QString& value) { m_Name = value; }
  void setLibrary(const QString& value) { m_Library = value; }
  void setDeviceSet(const QString& value) { m_DeviceSet = value; }
  void setDevice(const QString& value) { m_Device = value; }
  void setTechnology(const QString& value) { m_Technology = value; }
  void setValue(const QString& value) { m_Value = value; }
 protected:
  QList<CAttribute*> m_Attributes;
  QList<CVariant*> m_Variants;
  QString m_Name; // required
  QString m_Library; // required
  QString m_DeviceSet; // required
  QString m_Device; // required
  QString m_Technology; // default = ""
  QString m_Value; // implied
};

class CPolygon: public CEntity
{
 public:
  enum Pour {ppSolid, ppHatch, ppCutout};
 public:
  CPolygon(const QList<CVertex*>& vertices, const TDimension width,
           const TLayer layer, const TDimension spacing,
           const CPolygon::Pour pour, const TDimension isolate,
           const bool orphans, const bool thermals, const int rank);
  CPolygon(const CPolygon& polygon);
  CPolygon(void);
  virtual ~CPolygon(void);
 public:
  QString toString(const CPolygon::Pour value);
 public:
  virtual void operator =(const CPolygon& polygon);
  virtual void clear(void);
  virtual void assign(const CPolygon& polygon);
  virtual void scale(const double factor);
  virtual void show(std::ostream& out, const int level = 0);
  virtual bool readFromXML(const QDomElement& root);
  virtual bool writeToXML(QDomDocument& host, QDomElement& root, const bool defaults = false);
 public:
  static const CPolygon::Pour DEFAULT_POUR = CPolygon::ppSolid;
  static const int DEFAULT_RANK = 0;
 public:
  // getters
  QList<CVertex*>& vertices(void) { return m_Vertices; }
  TDimension width(void) { return m_Width; }
  TLayer layer(void) { return m_Layer; }
  TDimension spacing(void) { return m_Spacing; }
  CPolygon::Pour pour(void) { return m_Pour; }
  TDimension isolate(void) { return m_Isolate; }
  bool orphans(void) { return m_Orphans; }
  bool thermals(void) { return m_Thermals; }
  int rank(void) { return m_Rank; }
  // setters
  void setWidth(const TDimension value) { m_Width = value; }
  void setLayer(const TLayer value) { m_Layer = value; }
  void setSpacing(const TDimension value) { m_Spacing = value; }
  void setPour(const CPolygon::Pour value) { m_Pour = value; }
  void setIsolate(const TDimension value) { m_Isolate = value; }
  void setOrphans(const bool value) { m_Orphans = value; }
  void setThermals(const bool value) { m_Thermals = value; }
  void setRank(const int value) { m_Rank = value; }
 protected:
  QList<CVertex*> m_Vertices;
  TDimension m_Width; // required
  TLayer m_Layer; // required
  TDimension m_Spacing; // implied
  CPolygon::Pour m_Pour; // default = solid
  TDimension m_Isolate; // implied, only in <signal> and <package> contexts
  bool m_Orphans; // default = false, only in <signal> context
  bool m_Thermals; // default = true, only in <signal> context
  int m_Rank; // default = 0, 1..6 in <signal> context, 0..7 in <package> context
};

class CElement: public CEntity
{
 public:
  CElement(const CElement& element);
  CElement(void);
  virtual ~CElement(void);
 public:
  virtual void operator =(const CElement& element);
  virtual void clear(void);
  virtual void assign(const CElement& element);
  virtual void scale(const double factor);
  virtual void show(std::ostream& out, const int level = 0);
  virtual bool readFromXML(const QDomElement& root);
  virtual bool writeToXML(QDomDocument& host, QDomElement& root, const bool defaults = false);
 public:
  static const double DEFAULT_ROTATION = 0.0;
 public:
  // getters
  QList<CAttribute*>& attributes(void) { return m_Attributes; }
  QList<CVariant*>& variants(void) { return m_Variants; }
  QString name(void) { return m_Name; }
  QString library(void) { return m_Library; }
  QString package(void) { return m_Package; }
  QString value(void) { return m_Value; }
  TCoord x(void) { return m_X; }
  TCoord y(void) { return m_Y; }
  bool locked(void) { return m_Locked; }
  bool smashed(void) { return m_Smashed; }
  double rotation(void) { return m_Rotation; }
  bool reflection(void) { return m_Reflection; }
  // setters
  void setName(const QString& value) { m_Name = value; }
  void setLibrary(const QString& value) { m_Library = value; }
  void setPackage(const QString& value) { m_Package = value; }
  void setValue(const QString& value) { m_Value = value; }
  void setX(const TCoord value) { m_X = value; }
  void setY(const TCoord value) { m_Y = value; }
  void setLocked(const bool value) { m_Locked = value; }
  void setSmashed(const bool value) { m_Smashed = value; }
  void setRotation(const double value) { m_Rotation = std::min(std::max(value,0.0),359.999); }
  void setReflection(const bool value) { m_Reflection; }
 protected:
  QList<CAttribute*> m_Attributes;
  QList<CVariant*> m_Variants;
  QString m_Name; // required
  QString m_Library; // required
  QString m_Package; // required
  QString m_Value; // required
  TCoord m_X; // required
  TCoord m_Y; // required
  bool m_Locked; // default = false
  bool m_Smashed; // default = false
  double m_Rotation; // default = 0, valid 0 .. 359.9(9)
  bool m_Reflection; // default = false;
};

// high level objects

class CSignal: public CEntity
{
 public:
  CSignal(const CSignal& signal);
  CSignal(void);
  virtual ~CSignal(void);
 public:
  virtual void operator =(const CSignal& signal);
  virtual void clear(void);
  virtual void assign(const CSignal& signal);
  virtual void scale(const double factor);
  virtual void show(std::ostream& out, const int level = 0);
  virtual bool readFromXML(const QDomElement& root);
  virtual bool writeToXML(QDomDocument& host, QDomElement& root, const bool defaults = false);
 public:
  static const int DEFAULT_CLASS = 0;
 public:
  QList<CContactRef*>& contactRefs(void) { return m_ContactRefs; }
  QList<CPolygon*>& polygons(void) { return m_Polygons; }
  QList<CWire*>& wires(void) { return m_Wires; }
  QList<CVia*>& vias(void) { return m_Vias; }
  QString name(void) { return m_Name; }
  int signalClass(void) { return m_Class; }
  bool airWiresHidden(void) { return m_AirWiresHidden; }
  void setName(const QString& value) { m_Name = value; }
  void setSignalClass(const int value) { m_Class = value; }
  void setAirWiresHidden(const bool value) { m_AirWiresHidden = value; }
 protected:
  QList<CContactRef*> m_ContactRefs;
  QList<CPolygon*> m_Polygons;
  QList<CWire*> m_Wires;
  QList<CVia*> m_Vias;
  QString m_Name; // required
  int m_Class; // default = 0
  bool m_AirWiresHidden; // default = false
};

class CSegment: public CEntity
{
 public:
  CSegment(const CSegment& segment);
  CSegment(void);
  virtual ~CSegment(void);
 public:
  virtual void operator =(const CSegment& segment);
  virtual void clear(void);
  virtual void assign(const CSegment& segment);
  virtual void scale(const double factor);
  virtual void show(std::ostream& out, const int level = 0);
  virtual bool readFromXML(const QDomElement& root);
  virtual bool writeToXML(QDomDocument& host, QDomElement& root, const bool defaults = false);
 public:
  QList<CPinRef*>& pinRefs(void) { return m_PinRefs; }
  QList<CWire*>& wires(void) { return m_Wires; }
  QList<CJunction*>& junctions(void) { return m_Junctions; }
  QList<CLabel*>& labels(void) { return m_Labels; }
 protected:
  QList<CPinRef*> m_PinRefs; // only in <net> context
  QList<CWire*> m_Wires;
  QList<CJunction*> m_Junctions; // only in <net> context
  QList<CLabel*> m_Labels;
};

class CNet: public CEntity
{
 public:
  CNet(const CNet& net);
  CNet(void);
  virtual ~CNet(void);
 public:
  virtual void operator =(const CNet& net);
  virtual void clear(void);
  virtual void assign(const CNet& net);
  virtual void scale(const double factor);
  virtual void show(std::ostream& out, const int level = 0);
  virtual bool readFromXML(const QDomElement& root);
  virtual bool writeToXML(QDomDocument& host, QDomElement& root, const bool defaults = false);
 public:
  static const int DEFAULT_CLASS = 0;
 public:
  QList<CSegment*>& segments(void) { return m_Segments; }
  QString name(void) { return m_Name; }
  int netClass(void) { return m_Class; }
  void setName(const QString& value) { m_Name = value; }
  void setNetClass(const int& value) { m_Class = value; }
 protected:
  QList<CSegment*> m_Segments;
  QString m_Name; // required
  int m_Class; // default = 0
};

class CBus: public CEntity
{
 public:
  CBus(const CBus& bus);
  CBus(void);
  virtual ~CBus(void);
 public:
  virtual void operator =(const CBus& bus);
  virtual void clear(void);
  virtual void assign(const CBus& bus);
  virtual void scale(const double factor);
  virtual void show(std::ostream& out, const int level = 0);
  virtual bool readFromXML(const QDomElement& root);
  virtual bool writeToXML(QDomDocument& host, QDomElement& root, const bool defaults = false);
 public:
  QList<CSegment*>& segments(void) { return m_Segments; }
  QString name(void) { return m_Name; }
  void setName(const QString& value) { m_Name = value; }
 protected:
  QList<CSegment*> m_Segments;
  QString m_Name; // required
};

class CDevice: public CEntity
{
 public:
  CDevice(const QString& name, const QString& package);
  CDevice(const CDevice& device);
  CDevice(void);
  virtual ~CDevice(void);
 public:
  virtual void operator =(const CDevice& device);
  virtual void clear(void);
  virtual void assign(const CDevice& device);
  virtual void scale(const double factor);
  virtual void show(std::ostream& out, const int level = 0);
  virtual bool readFromXML(const QDomElement& root);
  virtual bool writeToXML(QDomDocument& host, QDomElement& root, const bool defaults = false);
 public:
  QList<CConnect*>& connects(void) { return m_Connects; }
  QList<CTechnology*>& technologies(void) { return m_Technologies; }
  QString name(void) { return m_Name; }
  QString package(void) { return m_Package; }
  void setName(const QString& value) { m_Name = value; }
  void setPackage(const QString& value) { m_Package = value; }
 protected:
  QList<CConnect*> m_Connects;
  QList<CTechnology*> m_Technologies;
  QString m_Name; // default = ""
  QString m_Package; // implied
};

class CDeviceSet: public CEntity
{
 public:
  CDeviceSet(const QString& name, const QString& prefix, const bool userValue);
  CDeviceSet(const CDeviceSet& deviceSet);
  CDeviceSet(void);
  virtual ~CDeviceSet(void);
 public:
  virtual void operator =(const CDeviceSet& deviceSet);
  virtual void clear(void);
  virtual void assign(const CDeviceSet& deviceSet);
  virtual void scale(const double factor);
  virtual void show(std::ostream& out, const int level = 0);
  virtual bool readFromXML(const QDomElement& root);
  virtual bool writeToXML(QDomDocument& host, QDomElement& root, const bool defaults = false);
 public:
  CDescription& description(void) { return m_Description; }
  QList<CGate*>& gates(void) { return m_Gates; }
  QList<CDevice*>& devices(void) { return m_Devices; }
  QString name(void) const { return m_Name; }
  QString prefix(void) const { return m_Prefix; }
  bool userValue(void) const { return m_UserValue; }
  void setName(const QString& value) { m_Name = value; }
  void setPrefix(const QString& value) { m_Prefix = value; }
  void setUserValue(const bool value) { m_UserValue = value; }
 public:
  CGate *findGateByName(const QString& name);
  CDevice *findDeviceByName(const QString& name);
 protected:
  CDescription m_Description;
  QList<CGate*> m_Gates;
  QList<CDevice*> m_Devices;
  QString m_Name; // required
  QString m_Prefix; // default = ""
  bool m_UserValue; // default = false
};

class CSymbol: public CEntity
{
 public:
  CSymbol(const QString& name);
  CSymbol(const CSymbol& symbol);
  CSymbol(void);
  virtual ~CSymbol(void);
 public:
  virtual void operator =(const CSymbol& symbol);
  virtual void clear(void);
  virtual void assign(const CSymbol& symbol);
  virtual void scale(const double factor);
  virtual void show(std::ostream& out, const int level = 0);
  virtual bool readFromXML(const QDomElement& root);
  virtual bool writeToXML(QDomDocument& host, QDomElement& root, const bool defaults = false);
 public:
  // getters
  CDescription& description(void) { return m_Description; }
  QList<CPolygon*>& polygons(void) { return m_Polygons; }
  QList<CWire*>& wires(void) { return m_Wires; }
  QList<CText*>& texts(void) { return m_Texts; }
  QList<CDimension*>& dimensions(void) { return m_Dimensions; }
  QList<CPin*>& pins(void) { return m_Pins; }
  QList<CCircle*>& circles(void) { return m_Circles; }
  QList<CRectangle*>& rectangles(void) { return m_Rectangles; }
  QList<CFrame*>& frames(void) { return m_Frames; }
  QString name(void) { return m_Name; }
  // setters
  void setName(const QString& value) { m_Name = value; }
 protected:
  CDescription m_Description;
  QList<CPolygon*> m_Polygons;
  QList<CWire*> m_Wires;
  QList<CText*> m_Texts;
  QList<CDimension*> m_Dimensions;
  QList<CPin*> m_Pins;
  QList<CCircle*> m_Circles;
  QList<CRectangle*> m_Rectangles;
  QList<CFrame*> m_Frames;
  QString m_Name; // required
};

class CPackage: public CEntity
{
 public:
  CPackage(const CPackage& package);
  CPackage(void);
  virtual ~CPackage(void);
 public:
  virtual void operator =(const CPackage& package);
  virtual void clear(void);
  virtual void assign(const CPackage& package);
  virtual void scale(const double factor);
  virtual void show(std::ostream& out, const int level = 0);
  virtual bool readFromXML(const QDomElement& root);
  virtual bool writeToXML(QDomDocument& host, QDomElement& root, const bool defaults = false);
 public:
  // getters
  CDescription& description(void) { return m_Description; }
  QList<CPolygon*>& polygons(void) { return m_Polygons; }
  QList<CWire*>& wires(void) { return m_Wires; }
  QList<CText*>& texts(void) { return m_Texts; }
  QList<CDimension*>& dimensions(void) { return m_Dimensions; }
  QList<CCircle*>& circles(void) { return m_Circles; }
  QList<CRectangle*>& rectangles(void) { return m_Rectangles; }
  QList<CFrame*>& frames(void) { return m_Frames; }
  QList<CHole*>& holes(void) { return m_Holes; }
  QList<CPad*>& pads(void) { return m_Pads; }
  QList<CSMD*>& smds(void) { return m_SMDs; }
  QString name(void) { return m_Name; }
  // setters
  void setName(const QString& value) { m_Name = value; }
 protected:
  CDescription m_Description;
  QList<CPolygon*> m_Polygons;
  QList<CWire*> m_Wires;
  QList<CText*> m_Texts;
  QList<CDimension*> m_Dimensions;
  QList<CCircle*> m_Circles;
  QList<CRectangle*> m_Rectangles;
  QList<CFrame*> m_Frames;
  QList<CHole*> m_Holes;
  QList<CPad*> m_Pads;
  QList<CSMD*> m_SMDs;
  QString m_Name; // required
};

class CPlain: public CEntity
{
 public:
  CPlain(const CPlain& plain);
  CPlain(void);
  ~CPlain(void);
 public:
  virtual void operator =(const CPlain& plain);
  virtual void clear(void);
  virtual void assign(const CPlain& plain);
  virtual void scale(const double factor);
  virtual void show(std::ostream& out, const int level = 0);
  virtual bool readFromXML(const QDomElement& root);
  virtual bool writeToXML(QDomDocument& host, QDomElement& root, const bool defaults = false);
 public:
  QList<CPolygon*>& polygons(void) { return m_Polygons; }
  QList<CWire*>& wires(void) { return m_Wires; }
  QList<CText*>& texts(void) { return m_Texts; }
  QList<CCircle*>& circles(void) { return m_Circles; }
  QList<CRectangle*>& rectangles(void) { return m_Rectangles; }
  QList<CFrame*>& frames(void) { return m_Frames; }
  QList<CHole*>& holes(void) { return m_Holes; }
  QList<CDimension*>& dimensions(void) { return m_Dimensions; }
 protected:
  QList<CPolygon*> m_Polygons;
  QList<CWire*> m_Wires;
  QList<CText*> m_Texts;
  QList<CCircle*> m_Circles;
  QList<CRectangle*> m_Rectangles;
  QList<CFrame*> m_Frames;
  QList<CHole*> m_Holes;
  QList<CDimension*> m_Dimensions;
};

class CSheet: public CEntity
{
 public:
  CSheet(const CSheet& sheet);
  CSheet(void);
  virtual ~CSheet(void);
 public:
  virtual void operator =(const CSheet& sheet);
  virtual void clear(void);
  virtual void assign(const CSheet& sheet);
  virtual void scale(const double factor);
  virtual void show(std::ostream& out, const int level = 0);
  virtual bool readFromXML(const QDomElement& root);
  virtual bool writeToXML(QDomDocument& host, QDomElement& root, const bool defaults = false);
 public:
  CDescription& description(void) { return m_Description; }
  CPlain& plain(void) { return m_Plain; }
  QList<CInstance*>& instances(void) { return m_Instances; }
  QList<CBus*>& busses(void) { return m_Busses; }
  QList<CNet*>& nets(void) { return m_Nets; }
 protected:
  CDescription m_Description;
  CPlain m_Plain;
  QList<CInstance*> m_Instances;
  QList<CBus*> m_Busses;
  QList<CNet*> m_Nets;
};

// drawing definitions

class CLibrary: public CEntity
{
 public:
  CLibrary(const CLibrary& library);
  CLibrary(void);
  virtual ~CLibrary(void);
 public:
  virtual void operator =(const CLibrary& library);
  virtual void clear(void);
  virtual void assign(const CLibrary& library);
  virtual void scalePackages(const double factor);
  virtual void scaleSymbols(const double factor);
  virtual void scale(const double factor);
  virtual void show(std::ostream& out, const int level = 0);
  virtual bool readFromXML(const QDomElement& root);
  virtual bool writeToXML(QDomDocument& host, QDomElement& root, const bool defaults = false);
 public:
  CDescription& description(void) { return m_Description; }
  QList<CPackage*>& packages(void) { return m_Packages; }
  QList<CSymbol*>& symbols(void) { return m_Symbols; }
  QList<CDeviceSet*>& deviceSets(void) { return m_DeviceSets; }
  QString& name(void) { return m_Name; }
 public:
  CPackage *findPackageByName(const QString& name);
  CSymbol *findSymbolByName(const QString& name);
  CDeviceSet *findDeviceSetByName(const QString& name);
 protected:
  CDescription m_Description;
  QList<CPackage*> m_Packages;
  QList<CSymbol*> m_Symbols;
  QList<CDeviceSet*> m_DeviceSets;
  QString m_Name; // required
};

class CBoard: public CEntity
{
 public:
  CBoard(const CBoard& board);
  CBoard(void);
  virtual ~CBoard(void);
 public:
  virtual void operator =(const CBoard& board);
  virtual void clear(void);
  virtual void assign(const CBoard& board);
  virtual void scale(const double factor);
  virtual void show(std::ostream& out, const int level = 0);
  virtual bool readFromXML(const QDomElement& root);
  virtual bool writeToXML(QDomDocument& host, QDomElement& root, const bool defaults = false);
 public:
  CDescription& description(void) { return m_Description; }
  CPlain& plain(void) { return m_Plain; }
  QList<CLibrary*>& libraries(void) { return m_Libraries; }
  QList<CAttribute*>& attributes(void) { return m_Attributes; }
  QList<CVariantDef*>& variantDefs(void) { return m_VariantDefs; }
  QList<CClass*>& classes(void) { return m_Classes; }
  CDesignRule& designRules(void) { return m_DesignRules; }
  QList<CPass*>& autorouter(void) { return m_Passes; }
  QList<CElement*>& elements(void) { return m_Elements; }
  QList<CSignal*>& signal(void) { return m_Signals; }
  QList<CApproved*>& errors(void) { return m_Errors; }
 protected:
  CDescription m_Description;
  CPlain m_Plain;
  QList<CLibrary*> m_Libraries;
  QList<CAttribute*> m_Attributes;
  QList<CVariantDef*> m_VariantDefs;
  QList<CClass*> m_Classes;
  //QList<CDesignRule*> m_DesignRules;
  CDesignRule m_DesignRules;
  // autorouter
  QList<CPass*> m_Passes;
  //
  QList<CElement*> m_Elements;
  QList<CSignal*> m_Signals;
  QList<CApproved*> m_Errors;
};

class CSchematic: public CEntity
{
 public:
  CSchematic(const CSchematic& schematic);
  CSchematic(void);
  virtual ~CSchematic(void);
 public:
  virtual void operator =(const CSchematic& schematic);
  virtual void clear(void);
  virtual void assign(const CSchematic& schematic);
  virtual void scale(const double factor);
  virtual void scalePackages(const double factor);
  virtual void show(std::ostream& out, const int level = 0);
  virtual bool readFromXML(const QDomElement& root);
  virtual bool writeToXML(QDomDocument& host, QDomElement& root, const bool defaults = false);
 public:
  CDescription& description(void) { return m_Description; }
  QList<CLibrary*>& libraries(void) { return m_Libraries; }
  QList<CAttribute*>& attributes(void) { return m_Attributes; }
  QList<CVariantDef*>& variantDefs(void) { return m_VariantDefs; }
  QList<CClass*>& classes(void) { return m_Classes; }
  QList<CPart*>& parts(void) { return m_Parts; }
  QList<CSheet*>& sheets(void) { return m_Sheets; }
  QList<CApproved*>& errors(void) { return m_Errors; }
  QString& xRefLabel(void) { return m_XRefLabel; }
  QString& xRefPart(void) { return m_XRefPart; }
 public:
  CLibrary *findLibraryByName(const QString& name);
  CPart *findPartByName(const QString& name);
 protected:
  CDescription m_Description;
  QList<CLibrary*> m_Libraries;
  QList<CAttribute*> m_Attributes;
  QList<CVariantDef*> m_VariantDefs;
  QList<CClass*> m_Classes;
  QList<CPart*> m_Parts;
  QList<CSheet*> m_Sheets;
  QList<CApproved*> m_Errors;
  QString m_XRefLabel; // implied
  QString m_XRefPart; // implied
};

class CDrawing: public CEntity
{
 public:
  enum Mode {dmLibrary, dmSchematic, dmBoard, dmMixed};
 public:
  CDrawing(const CDrawing& drawing);
  CDrawing(void);
  virtual ~CDrawing(void);
 public:
  virtual void operator =(const CDrawing& drawing);
  virtual void clear(void);
  virtual void assign(const CDrawing& drawing);
  virtual void initDefaultLayers();
  CLayer *findLayerByID(const TLayer layer);
  virtual void scale(const double factor);
  virtual void show(std::ostream& out, const int level = 0);
  virtual bool readFromXML(const QDomElement &root);
  virtual bool writeToXML(QDomDocument& host, QDomElement& root, const bool defaults = false);
 public:
  CSettings& settings(void) { return m_Settings; }
  CGrid& grid(void) { return m_Grid; }
  QList<CLayer*>& layers(void) { return m_Layers; }
  CLibrary& library(void) { return m_Library; }
  CSchematic& schematic(void) { return  m_Schematic; }
  CBoard& board(void) { return  m_Board; }
  CDrawing::Mode& mode(void) { return  m_Mode; }
 protected:
  CSettings m_Settings;
  CGrid m_Grid;
  QList<CLayer*> m_Layers;
  CLibrary m_Library;
  CSchematic m_Schematic;
  CBoard m_Board;
  //
  CDrawing::Mode m_Mode;
};

class CNote: public CEntity
{
 public:
  enum Severity {nsInfo, nsWarning, nsError};
 public:
  CNote(const CNote& note);
  CNote(void);
  virtual ~CNote(void);
 public:
  static QString toString(const CNote::Severity value);
 public:
  virtual void operator =(const CNote& note);
  virtual void clear(void);
  virtual void assign(const CNote& note);
  virtual void show(std::ostream& out, const int level = 0);
  virtual bool readFromXML(const QDomElement &root);
  virtual bool writeToXML(QDomDocument& host, QDomElement& root, const bool defaults = false);
 public:
  // getters
  QString note(void) { return m_Note; }
  CNote::Severity severity(void) { return m_Severity; }
  // setters
  void setNote(const QString& value) { m_Note = value; }
  void setSeverity(const CNote::Severity value) { m_Severity = value; }
 protected:
  QString m_Note;
  double m_Version; // required
  CNote::Severity m_Severity; // required
};

class CCompatibility: public CEntity
{
 public:
  CCompatibility(const CCompatibility& compatibility);
  CCompatibility(void);
  virtual ~CCompatibility(void);
 public:
  virtual void operator =(const CCompatibility& compatibility);
  virtual void clear(void);
  virtual void assign(const CCompatibility& compatibility);
  virtual void show(std::ostream& out, const int level = 0);
  virtual bool readFromXML(const QDomElement &root);
  virtual bool writeToXML(QDomDocument& host, QDomElement& root, const bool defaults = false);
 public:
  QList<CNote*>& notes(void) { return m_Notes; }
 protected:
  QList<CNote*> m_Notes;
};

class CEagleDocument: public CEntity
{
 public:
  CEagleDocument(const CEagleDocument& document);
  CEagleDocument(void);
  virtual ~CEagleDocument(void);
 public:
  virtual void operator =(const CEagleDocument& document);
  virtual void clear(void);
  virtual void assign(const CEagleDocument& document);
  virtual void show(std::ostream& out, const int level = 0);
  bool loadFromFile(const QString& fileName);
  bool saveToFile(const QString& fileName);
 //protected:
  virtual bool readFromXML(const QDomElement& root);
  virtual bool writeToXML(QDomDocument& host, QDomElement& root, const bool defaults = false);
 public:
  // getters
  CCompatibility& preNotes(void) { return m_PreNotes; }
  CDrawing& drawing(void) { return m_Drawing; }
  CCompatibility& postNotes(void) { return m_PostNotes; }
  double version(void) const { return m_Version; }
  //
  bool verifyDocType(void) const { return m_VerifyDocType; }
  int indentation(void) const { return m_Indentation; }
  bool writeDefaults(void) const { return m_WriteDefaults; }
  bool validDocType(void) const { return m_ValidDocType; }
  bool validXMLdata(void) const { return m_ValidXMLdata; }
  // setters
  void setVerifyDocType(const bool value) { m_VerifyDocType = value; }
  void setIndentation(const int value) { m_Indentation = value; }
  void setWriteDefaults(const bool value) { m_WriteDefaults = value; }
 protected:
  CCompatibility m_PreNotes;
  CDrawing m_Drawing;
  CCompatibility m_PostNotes;
  double m_Version; // required, V.RR
  //
  bool m_VerifyDocType;
  int m_Indentation;
  bool m_WriteDefaults;
  bool m_ValidDocType;
  bool m_ValidXMLdata;
};

}

#endif // QEAGLELIB_H
//------------------------------------------------------------------------------


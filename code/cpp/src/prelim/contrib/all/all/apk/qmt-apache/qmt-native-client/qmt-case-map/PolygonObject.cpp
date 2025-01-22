#include "PolygonObject.h"

#include "guts/Conversions.h"
#include "CircleObject.h"

#include <QtDebug>
#include <QKeyEvent>

#include "MapTileSource.h"
#include "MapGraphicsView.h"

#include "qmt-gis/qmt-geospatial-marker.h"

#include <string>


#include <iostream>
#include <string>
#include <cstdlib>
#include <tsl/ordered_map.h>
#include <tsl/ordered_set.h>

//int main() {
//    tsl::ordered_map<char, int> map = {{'d', 1}, {'a', 2}, {'g', 3}};
//    map.insert({'b', 4});
//    map['h'] = 5;
//    map['e'] = 6;

//    map.erase('a');


//    // {d, 1} {g, 3} {b, 4} {h, 5} {e, 6}
//    for(const auto& key_value : map) {
//        std::cout << "{" << key_value.first << ", " << key_value.second << "}" << std::endl;
//    }


//    map.unordered_erase('b');

//    // Break order: {d, 1} {g, 3} {e, 6} {h, 5}
//    for(const auto& key_value : map) {
//        std::cout << "{" << key_value.first << ", " << key_value.second << "}" << std::endl;
//    }


//    for(auto it = map.begin(); it != map.end(); ++it) {
//        //it->second += 2; // Not valid.
//        it.value() += 2;
//    }


//    if(map.find('d') != map.end()) {
//        std::cout << "Found 'd'." << std::endl;
//    }

//    const std::size_t precalculated_hash = std::hash<char>()('d');
//    // If we already know the hash beforehand, we can pass it as argument to speed-up the lookup.
//    if(map.find('d', precalculated_hash) != map.end()) {
//        std::cout << "Found 'd' with hash " << precalculated_hash << "." << std::endl;
//    }


//    tsl::ordered_set<char, std::hash<char>, std::equal_to<char>,
//                     std::allocator<char>, std::vector<char>> set;
//    set.reserve(6);

//    set = {'3', '4', '9', '2'};
//    set.erase('2');
//    set.insert('1');
//    set.insert('\0');

//    set.pop_back();
//    set.insert({'0', '\0'});

//    // Get raw buffer for C API: 34910
//    std::cout << atoi(set.data()) << std::endl;
//}


PolygonObject::PolygonObject(MapGraphicsView* containing_view, QPolygonF geoPoly, QColor fillColor, QObject *parent) :
    MapGraphicsObject(containing_view, parent), _geoPoly(geoPoly), _fillColor(fillColor),
    ref_marker_(nullptr), ref_geo_hot_spot_(nullptr)
{
    this->setFlag(MapGraphicsObject::ObjectIsMovable);
    this->setFlag(MapGraphicsObject::ObjectIsSelectable,false);
    this->setFlag(MapGraphicsObject::ObjectIsFocusable);
    this->setGeoPoly(geoPoly);
}

PolygonObject::~PolygonObject()
{
    qDebug() << this << "destroying";
    foreach(MapGraphicsObject * circle, _editCircles)
        this->destroyEditCircle(circle);
    _editCircles.clear();

    foreach(MapGraphicsObject * circle, _addVertexCircles)
        this->destroyAddVertexCircle(circle);
    _addVertexCircles.clear();
}

void PolygonObject::init_ref_marker(const QPolygonF& qpf, QColor passive_color, QColor active_color)
{
 ref_marker_ = new QMT_Geospatial_Marker(qpf, passive_color, active_color);
}


//pure-virtual from MapGraphicsObject
QRectF PolygonObject::boundingRect() const
{
    QRectF latLonRect = _geoPoly.boundingRect();
    QPointF latLonCenter = latLonRect.center();
    Position latLonCenterPos(latLonCenter,0.0);
    Position topLeftPos(latLonRect.topLeft(),0.0);
    Position bottomRightPos(latLonRect.bottomRight(),0.0);

    QPointF topLeftENU = Conversions::lla2enu(topLeftPos,latLonCenterPos).toPointF();
    QPointF bottomRightENU = Conversions::lla2enu(bottomRightPos,latLonCenterPos).toPointF();

    return QRectF(topLeftENU,bottomRightENU);
}


bool PolygonObject::pixel_contains(const QPointF& geoPos) const
{
 if(!ref_geo_hot_spot_)
   return false;

//  qDebug() << " ============== ";
//  qDebug() << "lat lon = " << latitude() << ", " << longitude();
//  qDebug() << "geo pos = " << qSetRealNumberPrecision(10) << geoPos.x() << " , " << geoPos.y();

// QPointF geoPos1 =  QPointF(-74.1096,40.8782);

// //?-74.10964966","40.87821814

// qDebug() << "ref_geo_hot_spot_ = " << *ref_geo_hot_spot_;
// qDebug() << "current_enu_polygon_ = " << current_enu_polygon_;
// qDebug() << "current_enu_polygon_ b = " << current_enu_polygon_.boundingRect();
// qDebug() << "ref enu hot spot = " << ref_enu_hot_spot_;

 //
 Position latLonPos(geoPos, 0.0);

// qDebug() << "latLonPos = " << latLonPos;


 // Position latLonPos(geoPos1, 0.0);
 QPointF enu = Conversions::lla2enu(latLonPos, *ref_geo_hot_spot_).toPointF();

//? qDebug() << "enu = " << enu;

 held_enu_hot_spot_ = enu;

 QPointF ll_enu;
 {
  Position latLonPos({longitude(), latitude()}, 0.0);
  ll_enu = Conversions::lla2enu(latLonPos,*ref_geo_hot_spot_).toPointF();
 }



//? ref_enu_hot_spot_ = Conversions::lla2enu(*ref_geo_hot_spot_,*ref_geo_hot_spot_).toPointF();
 QPointF ref_enu_hot_spot = {current_enu_polygon_.boundingRect().center().x(), current_enu_polygon_.boundingRect().top()};

// qDebug() << "! ref_enu_hot_spot = " << ref_enu_hot_spot;
// qDebug() << "! ll_enu = " << ll_enu;




//? return current_enu_polygon_.containsPoint(enu, Qt::OddEvenFill);
//?
 return current_enu_polygon_.boundingRect().contains(enu);
}


#ifdef HIDE
 qDebug() << "\n === " ;
 {
  QPointF geoPos1 {longitude(), latitude()};

  qDebug() << "geoPos ll = " << geoPos1;

  Position latLonCenterPos(_geoPoly.boundingRect().center(),0);
  Position latLonPos(geoPos1, 0.0);
  QPointF enu = Conversions::lla2enu(latLonPos,latLonCenterPos).toPointF();

  qDebug() << "enu ll = " << enu;
 }

 QPointF geoPos0 = QPointF(longitude(), latitude()); //-74.1095,40.8781);


 qDebug() << "\ngeo = " << geoPos;
 qDebug() << "cur = " << current_enu_polygon_;

 qDebug() << "curb = " << current_enu_polygon_.boundingRect();


 Position latLonCenterPos(_geoPoly.boundingRect().center(),0);

 Position latLonPos0(geoPos0, 0.0);
 QPointF enu0 = Conversions::lla2enu(latLonPos0,latLonCenterPos).toPointF();

 qDebug() << "enu0 = " << enu0;

 if(current_enu_polygon_.boundingRect().contains(enu0))
 {
  qDebug() << "in poly";
 }
 else
 {
  qDebug() << "not in poly";
 }

 Position latLonPos(geoPos, 0.0);
 QPointF enu = Conversions::lla2enu(latLonPos,latLonCenterPos).toPointF();

 qDebug() << "enu = " << enu;

// return current_enu_polygon_.containsPoint(enu, Qt::OddEvenFill);
 return current_enu_polygon_.boundingRect().contains(enu);
#endif


#ifdef HIDE
 qDebug() << "pixel contains called ...";

 qDebug() << "geoPos = " << geoPos;

 QPointF center = _geoPoly.boundingRect().center();

//?
//
 bool result = _geoPoly.containsPoint(geoPos, Qt::OddEvenFill);

 qDebug() << "geoPoly b = " << _geoPoly.boundingRect();

 bool result0 = _geoPoly.boundingRect().contains(geoPos);
 //?
 bool result1 = _geoPoly.containsPoint(center, Qt::OddEvenFill);


 if(result)
   qDebug() << geoPos << " : " << _geoPoly;
 else
   qDebug() << geoPos << " ; " << _geoPoly;

 if(result1)
   qDebug() << center << " :: " << _geoPoly;
 else
   qDebug() << center << " ;; " << _geoPoly;

 if(result0)
   qDebug() << geoPos << " (0) " << _geoPoly.boundingRect();
 else
   qDebug() << geoPos << " [0] " << _geoPoly.boundingRect();


//  return _geoPoly.containsPoint(geoPos,
//                               Qt::OddEvenFill);

 return result;
#endif


//virtual from MapGraphicsObject
bool PolygonObject::contains(const QPointF &geoPos) const
{
 return _geoPoly.containsPoint(geoPos,
   Qt::OddEvenFill);

#ifdef HIDE
 qDebug() << "contains called ...";

 qDebug() << "geoPos = " << geoPos;

 QPointF center = _geoPoly.boundingRect().center();

//?
//
 bool result = _geoPoly.containsPoint(geoPos, Qt::OddEvenFill);

 qDebug() << "geoPoly b = " << _geoPoly.boundingRect();

 bool result0 = _geoPoly.boundingRect().contains(geoPos);
 //?
 bool result1 = _geoPoly.containsPoint(center, Qt::OddEvenFill);


 if(result)
   qDebug() << geoPos << " : " << _geoPoly;
 else
   qDebug() << geoPos << " ; " << _geoPoly;

 if(result1)
   qDebug() << center << " :: " << _geoPoly;
 else
   qDebug() << center << " ;; " << _geoPoly;

 if(result0)
   qDebug() << geoPos << " (0) " << _geoPoly.boundingRect();
 else
   qDebug() << geoPos << " [0] " << _geoPoly.boundingRect();


//  return _geoPoly.containsPoint(geoPos,
//                               Qt::OddEvenFill);

 return result;
#endif
}

const QString _outline_ = "outline";
const QString _nugget_ = "nugget";

void PolygonObject::draw_scene_polygon_at_ll(const QPolygonF& poly, QPointF latlon,
  QColor color, tsl::ordered_map<const QString*, QVector<QPair<QBrush, QPen>>>& styles, r8 scale)
{
 QSharedPointer<MapTileSource> tileSource = containing_view_->tileSource();  //_infoSource->tileSource();
 if (tileSource.isNull())
 {
  qWarning() << this << "can't do bounding box conversion, null tile source.";
  return;
 }

 QPolygonF qpf = poly;
 QPointF base = latlon.transposed();

 styles.insert({&_outline_, {}});
 styles.insert({&_nugget_, {}});
 QVector<QPair<QBrush, QPen>>& outline_styles = styles.at(&_outline_); // outline
 QVector<QPair<QBrush, QPen>>& nugget_styles = styles.at(&_nugget_); // nugget


 int zoom_level = containing_view_->zoomLevel();

 QPointF base_pixels = tileSource->ll2qgs(base, zoom_level); //tileSource


 QTransform trans;
 trans = trans.scale(scale, scale);

 QPolygonF qpf2 = trans.map(poly);

 QPolygonF qpf1;

 for(QPointF point : qpf2)
 {
  QPointF p1 = base_pixels - point;
  qpf1 << tileSource->qgs2ll(p1, zoom_level);
 }

 _geoPoly = qpf1;

 if(ref_marker_->outline_code() & 64)
 {
  QPen pen(QBrush(QColor(Qt::darkMagenta)), 15, Qt::DotLine);
  outline_styles.push_back({Qt::NoBrush, pen});
//  painter->setBrush(Qt::NoBrush);
//  painter->setPen(pen);
//  painter->drawPolygon(*pf);
 }

 if(ref_marker_->outline_code() & 30) // 2, 4, 8, 16
 {
  int outline_count = 0;
  if(ref_marker_->outline_code() & 2)
   ++outline_count;
  if(ref_marker_->outline_code() & 4)
   ++outline_count;
  if(ref_marker_->outline_code() & 8)
   ++outline_count;
  if(ref_marker_->outline_code() & 16)
   ++outline_count;

  int overall_width = 12 + outline_count * 3;

  int one_ring_width = overall_width / outline_count;
  int ring_width = overall_width;

  {
   QPen pen(QBrush(QColor(Qt::white)), ring_width + 4);
   outline_styles.push_back({Qt::NoBrush, pen});

//   painter->setBrush(Qt::NoBrush);
//   painter->setPen(pen);
//   painter->drawPolygon(*pf);
  }

  if(ref_marker_->outline_code() & 2)
  {
   QPen pen(QBrush(QColor(Qt::darkGreen)), ring_width);
   outline_styles.push_back({Qt::NoBrush, pen});

//   painter->setBrush(Qt::NoBrush);
//   painter->setPen(pen);
//   painter->drawPolygon(*pf);
   ring_width -= one_ring_width;
  }

  if(ref_marker_->outline_code() & 4)
  {
   QPen pen(QBrush(QColor(Qt::darkRed)), ring_width);
   outline_styles.push_back({Qt::NoBrush, pen});
//   painter->setBrush(Qt::NoBrush);
//   painter->setPen(pen);
//   painter->drawPolygon(*pf);
   ring_width -= one_ring_width;
  }

  if(ref_marker_->outline_code() & 8)
  {
   QPen pen(QBrush(QColor(Qt::darkMagenta)), ring_width);
   outline_styles.push_back({Qt::NoBrush, pen});
//   painter->setBrush(Qt::NoBrush);
//   painter->setPen(pen);
//   painter->drawPolygon(*pf);
   ring_width -= one_ring_width;
  }

  if(ref_marker_->outline_code() & 16)
  {
   QPen pen(QBrush(QColor(Qt::cyan)), ring_width);
   outline_styles.push_back({Qt::NoBrush, pen});
//   painter->setBrush(Qt::NoBrush);
//   painter->setPen(pen);
//   painter->drawPolygon(*pf);
   ring_width -= one_ring_width;
  }

  {
   QPen pen(QBrush(QColor(Qt::white)), 2);
   outline_styles.push_back({Qt::NoBrush, pen});
//   painter->setBrush(Qt::NoBrush);
//   painter->setPen(pen);
//   painter->drawPolygon(*pf);
  }


 }
 if(ref_marker_->outline_code() & 1)
 {
  nugget_styles.push_back({QBrush(QColor(Qt::red)), QPen(Qt::white, 3)});
//  painter->setBrush(QColor(Qt::red));
//  painter->setPen(QPen(Qt::white, 3));
//  painter->drawEllipse(pf->boundingRect().center(), 24, 24);
 }


}


//pure-virtual from MapGraphicsObject
void PolygonObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
 Q_UNUSED(option)
 Q_UNUSED(widget)

 tsl::ordered_map<const QString*, QVector<QPair<QBrush, QPen>>> styles;

//?#ifdef HIDE
 if(ref_marker_)
 {
  draw_scene_polygon_at_ll(ref_marker_->outline(), QPointF{latitude(), longitude()},
    Qt::red, styles,
    ref_marker_->scale());
 }
//?#endif //def HIDE

 painter->setRenderHint(QPainter::Antialiasing,true);

// if(ref_enu_polygon_.isEmpty())
// {
//?  QPolygonF enuPoly;

 if(!ref_geo_hot_spot_)
 {
  ref_geo_hot_spot_ = new Position({_geoPoly.boundingRect().center().x(), _geoPoly.boundingRect().top()},0);
 }

 current_enu_polygon_.clear();

 foreach(QPointF latLon, _geoPoly)
 {
  Position latLonPos(latLon,0.0);
  QPointF enu = Conversions::lla2enu(latLonPos,*ref_geo_hot_spot_).toPointF();
  current_enu_polygon_ << enu;
 }

 QPointF ll_enu;
 {
  Position latLonPos({longitude(), latitude()}, 0.0);
  ll_enu = Conversions::lla2enu(latLonPos,*ref_geo_hot_spot_).toPointF();
 }



//? ref_enu_hot_spot_ = Conversions::lla2enu(*ref_geo_hot_spot_,*ref_geo_hot_spot_).toPointF();
 ref_enu_hot_spot_ = {current_enu_polygon_.boundingRect().center().x(), current_enu_polygon_.boundingRect().top()};

// qDebug() << "ref_enu_hot_spot_ = " << ref_enu_hot_spot_;
// qDebug() << "ll_enu = " << ll_enu;

//?}

 QColor fill_color = ref_marker_
   ? ref_marker_->current_color() : _fillColor;

 painter->setBrush(fill_color);
// painter->drawPolygon(enuPoly);
 painter->drawPolygon(current_enu_polygon_);

//?
// painter->setBrush(Qt::NoBrush);
// painter->setPen(Qt::blue);
// painter->drawEllipse(ref_enu_hot_spot_, 5, 5);

// painter->setPen(Qt::yellow);
// painter->drawEllipse( ll_enu, 4, 4);

//?
// if(!held_enu_hot_spot_.isNull())
// {
//  painter->setPen(Qt::magenta);
//  painter->drawEllipse(held_enu_hot_spot_, 6, 6);
// }

// current_enu_polygon_ = enuPoly;

// painter->setBrush(Qt::NoBrush);
// painter->setPen(Qt::blue);

// painter->drawRect(current_enu_polygon_.boundingRect());

// QPointF enu;

// {
//  QPointF geoPos {longitude(), latitude()};
//  Position latLonCenterPos({_geoPoly.boundingRect().center().x(), _geoPoly.boundingRect().top()},0);
//  Position latLonPos(geoPos, 0.0);
//  enu = Conversions::lla2enu(latLonPos,latLonCenterPos).toPointF();

//  qDebug() << "enu ll (2) = " << enu;

// }


// {
//  QPointF enu1 =  QPointF(20.0184,-51.9679);
//  qDebug() << "enu1 (2) = " << enu1;

//  painter->setBrush(Qt::yellow);
//  painter->drawEllipse(enu1, 5, 5);

// }

// painter->setBrush(Qt::red);
// painter->drawEllipse(enu, 5, 5);


// qDebug() << "\n! cur = " << current_enu_polygon_;
// qDebug() << "! curb = " << current_enu_polygon_.boundingRect();


 QBrush restore_brush = painter->brush();
 QPen restore_pen = painter->pen();

 for(auto it = styles.begin(); it != styles.end(); ++it)
 {
  if(it.key() == _outline_)
  {
   for(const auto& pr : it.value())
   {
    painter->setBrush(pr.first);
    painter->setPen(pr.second);
//?    painter->drawPolygon(enuPoly);
    painter->drawPolygon(current_enu_polygon_);
   }
  }
  else if(it.key() == _nugget_)
  {
   for(const auto& pr : it.value())
   {
    painter->setBrush(pr.first);
    painter->setPen(pr.second);
    //?painter->drawEllipse(enuPoly.boundingRect().center(), 24, 24);
    painter->drawEllipse(current_enu_polygon_.boundingRect().center(), 24, 24);
   }
  }
 }


//?
 if(ref_marker_)
 //if(false)
 {
  // //  anything here?
 }
 else
 {
  //Populate edit and add-vertex handles if necessary.
  //Is there a better place to do this? Most likely, yes.
  if (_editCircles.isEmpty())
  {
   //Create objects to edit the polygon!
   for (int i = 0; i < _geoPoly.size(); i++)
   {
    //Edit circles - to change the shape
    CircleObject* circle = this->constructEditCircle();
    circle->setPos(_geoPoly.at(i));
    _editCircles.append(circle);

    QPointF current = _geoPoly.at(i);
    QPointF next = _geoPoly.at((i+1) % _geoPoly.size());
    QPointF avg((current.x() + next.x())/2.0,
                (current.y() + next.y())/2.0);

    //Add vertex circles - to add new vertices
    CircleObject* betweener = this->constructAddVertexCircle();
    betweener->setPos(avg);
    _addVertexCircles.append(betweener);
   }
  }
 }
}


//QRectF br;

//QPointF top_left = _geoPoly.at(2);
//QPointF bottom_right = {_geoPoly.at(0).x(), _geoPoly.at(3).y()};

//qDebug() << "top left = " << top_left;
//qDebug() << "bottom right = " << bottom_right;


////? top_left = {-74.11, 40.8788};
//// bottom_right = {-74.03, 40.68};
//// bottom_right = {-74.1093, 40.8782};

//br.setTopLeft(top_left);
//// br.setBottomRight(bottom_right);

//br.setWidth(120);
//br.setHeight(120);

////= _geoPoly.boundingRect();
//qDebug() << "br = " << br;
//painter->setBrush(Qt::blue);
//painter->setPen(Qt::red);

//painter->drawEllipse(enuPoly.at(0), 11, 11);


//painter->setBrush(Qt::white);
//painter->setPen(Qt::red);

//painter->drawEllipse(enuPoly.at(1), 11, 11);

//painter->setBrush(Qt::gray);
//painter->setPen(Qt::red);

//painter->drawEllipse(enuPoly.at(2), 11, 11);

//painter->setBrush(Qt::yellow);
//painter->setPen(Qt::red);

//painter->drawEllipse(enuPoly.at(3), 11, 11);

//// painter->drawRect(br);

void PolygonObject::setPos(const QPointF & nPos)
{
    /*
      If the new position moved the object from the center of the bounding box made by the geo poly
      then we need to move the geo poly
    */
    if (nPos != _geoPoly.boundingRect().center())
    {
        const QPointF diff = nPos - this->pos();
        //_geoPoly.translate(diff);

        //We should also move our edit handles
        foreach(MapGraphicsObject * circle, _editCircles)
            circle->setPos(circle->pos() + diff);

        //And our "add vertex" handles
        this->fixAddVertexCirclePos();
    }

    MapGraphicsObject::setPos(nPos);

    //If this isn't here, we get TEARING when we edit our polygons
    this->posChanged();
}

QPolygonF PolygonObject::geoPoly() const
{
    return _geoPoly;
}

void PolygonObject::setGeoPoly(const QPolygonF &newPoly)
{
    if (newPoly == _geoPoly)
        return;

    _geoPoly = newPoly;

    foreach(MapGraphicsObject * obj, _editCircles)
        this->destroyEditCircle(obj);
    foreach(MapGraphicsObject * obj, _addVertexCircles)
        this->destroyAddVertexCircle(obj);
    _editCircles.clear();
    _addVertexCircles.clear();

    this->setPos(newPoly.boundingRect().center());
    this->polygonChanged(newPoly);
}

void PolygonObject::setFillColor(const QColor &color)
{
    if (_fillColor == color)
        return;

    _fillColor = color;
    this->redrawRequested();
}

//protected
//virtual from MapGraphicsObject
void PolygonObject::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    /*
      This method is a bit of a hack --- ideally contains() or some other method
      we "reimplement" from PrivateQGraphicsObject should take care of this, but it
      hasn't been working. This prevents the polygon from being grabbed when the user
      click inside the bounding box but outside of the actual polygon
    */
    const QPointF geoPos = event->scenePos();

    //If the geo point is within our geo polygon, we might accept it
    if (_geoPoly.containsPoint(geoPos,Qt::OddEvenFill))
        MapGraphicsObject::mousePressEvent(event);
    //Otherwise we ignore it
    else
        event->ignore();
}

//protected
//virtual from MapGraphicsObject
void PolygonObject::keyReleaseEvent(QKeyEvent *event)
{
    if (event->matches(QKeySequence::Delete))
    {
        this->deleteLater();
        event->accept();
    }
    else
        event->ignore();
}

//private slot
void PolygonObject::handleEditCirclePosChanged()
{
    QObject * sender = QObject::sender();
    if (sender == 0)
        return;
    CircleObject * circle = qobject_cast<CircleObject *>(sender);
    if (circle == 0)
        return;

    int index = _editCircles.indexOf(circle);
    if (index == -1)
        return;

    QPointF newPos = circle->pos();
    _geoPoly.replace(index,newPos);
    this->setPos(_geoPoly.boundingRect().center());

    //We need to update the positions of our "add vertex" controllers
    this->fixAddVertexCirclePos();

    //Emit a signal so everyone knows that the polygon changed
    this->polygonChanged(this->geoPoly());
}

//private slot
void PolygonObject::handleAddVertexCircleSelected()
{
    QObject * sender = QObject::sender();
    if (sender == 0)
        return;
    CircleObject * circle = qobject_cast<CircleObject *>(sender);
    if (circle == 0)
        return;

    //If the circle isn't selected, something is wrong
    if (!circle->isSelected())
        return;

    //Now that we know the circle was selected, just deselect it. We don't need it selected actually
    circle->setSelected(false);

    //Get the position at which we should add a vertex
    QPointF geoPos = circle->pos();

    //The index at which we should insert the new vertex
    int index = _addVertexCircles.indexOf(circle);
    if (index == -1)
        return;
    index++;

    //Put the vertex in the polygon
    _geoPoly.insert(index,geoPos);

    //Create a new "Edit Circle" and put it in the right spot
    CircleObject * editCircle = this->constructEditCircle();
    editCircle->setPos(geoPos);
    _editCircles.insert(index,editCircle);

    editCircle->setSelected(true);


    //Create a new "Add vertex" circle and put it in the right spot
    CircleObject * addVertexCircle = this->constructAddVertexCircle();
    QPointF current = _geoPoly.at(index-1);
    QPointF next = _geoPoly.at(index);
    QPointF avg((current.x() + next.x())/2.0,
                (current.y() + next.y())/2.0);
    addVertexCircle->setPos(avg);
    _addVertexCircles.insert(index,addVertexCircle);

    this->fixAddVertexCirclePos();

    //Emit a signal so everyone knows that the polygon changed
    this->polygonChanged(this->geoPoly());
}

//private slot
void PolygonObject::handleEditCircleDestroyed()
{
    QObject * sender = QObject::sender();
    if (sender == 0)
    {
        qWarning() << "Can't process desyroyed edit circle. Sender is null";
        return;
    }
    CircleObject * circle = (CircleObject *) sender;

    int index = _editCircles.indexOf(circle);
    if (index == -1)
    {
        qWarning() << "Can't process destroyed edit circle. Not contained in edit circle list";
        return;
    }

    _geoPoly.remove(index);
    _editCircles.removeAt(index);
    _addVertexCircles.takeAt(index)->deleteLater();

    this->fixAddVertexCirclePos();
    this->redrawRequested();
    this->setPos(_geoPoly.boundingRect().center());
}

//private
void PolygonObject::fixAddVertexCirclePos()
{
    for (int i = 0; i < _geoPoly.size(); i++)
    {
        QPointF current = _geoPoly.at(i);
        QPointF next = _geoPoly.at((i+1) % _geoPoly.size());
        QPointF avg((current.x() + next.x())/2.0,
                    (current.y() + next.y())/2.0);

        //?
        if(!_addVertexCircles.isEmpty())
          _addVertexCircles.at(i)->setPos(avg);
    }
}

//private
CircleObject *PolygonObject::constructEditCircle()
{
//? CircleObject * toRet = new CircleObject(containing_view_, 8);
    CircleObject * toRet = new CircleObject(containing_view_, 18);
    connect(toRet,
            SIGNAL(posChanged()),
            this,
            SLOT(handleEditCirclePosChanged()));
    connect(toRet,
            SIGNAL(destroyed()),
            this,
            SLOT(handleEditCircleDestroyed()));

    this->newObjectGenerated(toRet);
    return toRet;
}

//private
void PolygonObject::destroyEditCircle(MapGraphicsObject *obj)
{
    disconnect(obj,
               SIGNAL(posChanged()),
               this,
               SLOT(handleEditCirclePosChanged()));
    disconnect(obj,
               SIGNAL(destroyed()),
               this,
               SLOT(handleEditCircleDestroyed()));
    obj->deleteLater();
}

//private
CircleObject *PolygonObject::constructAddVertexCircle()
{
    CircleObject * toRet = new CircleObject(containing_view_, 3,
                                            true,
                                            QColor(100,100,100,255));
    toRet->setFlag(MapGraphicsObject::ObjectIsMovable,false);
    connect(toRet,
            SIGNAL(selectedChanged()),
            this,
            SLOT(handleAddVertexCircleSelected()));

    this->newObjectGenerated(toRet);
    toRet->setToolTip("Single-click (don't drag!) to add vertex.");
    return toRet;
}

//private
void PolygonObject::destroyAddVertexCircle(MapGraphicsObject *obj)
{
    disconnect(obj,
               SIGNAL(selectedChanged()),
               this,
               SLOT(handleAddVertexCircleSelected()));
    obj->deleteLater();
}

#ifdef HIDE

 QPolygonF qpf = poly;
 QPointF base = latlon.transposed();

 QPolygonF qpf1;
 QSharedPointer<MapTileSource> tileSource = containing_view_->tileSource();  //_infoSource->tileSource();
 if (tileSource.isNull())
 {
  qWarning() << this << "can't do bounding box conversion, null tile source.";
 }
 else
 {
  int zoom_level = containing_view_->zoomLevel();
  QPointF base_pixels = tileSource->ll2qgs(base, zoom_level); //tileSource
  QTransform trans;
  trans = trans.scale(scale, scale);

  QPolygonF qpf2 = trans.map(poly);

//  QTransform trans1;
//  trans1 = trans1.translate(longitude(), latitude());

  for(QPointF point : qpf2)
  {
   qDebug() << "point = " << point;

   QPointF p1 = base_pixels - point;
   qDebug() << "p1 = " << p1;

   qpf1 << tileSource->qgs2ll(p1, zoom_level);
  }
 }
 _geoPoly = qpf1;

#endif



 //???

     //#ifdef HIDE
        //   qpf = trans.map(qpf);
        //   trans = {};
        //   trans = trans.fromTranslate(longitude(), latitude());
        ////   QTransform trans1;
        ////   trans1 = trans1.fromTranslate(longitude(), latitude());

     //   qDebug() << "qpf 3= " << qpf;
     //   qDebug() << "qpf1 = " << *qpf1;

     //   _geoPoly = qpf;
     //?   QPolygonF enuPoly;

     //   QPolygonF qpf1;
     //    QPolygonF* qpf1 = new QPolygonF;
     //    (*qpf1) << QPointF(40.8782, -74.1096).transposed();
     //    (*qpf1) << QPointF(40.8782, -74.1196).transposed();
     //    (*qpf1) << QPointF(40.8882, -74.1196).transposed();
     //    (*qpf1) << QPointF(40.8882, -74.1096).transposed();

     //   Position latLonCenterPos(qpf1->boundingRect().center(),0);
     //   foreach(QPointF latLon, *qpf1)
     //   {

     //       Position latLonPos(latLon,0.0);
     //       QPointF enu = Conversions::lla2enu(latLonPos,latLonCenterPos).toPointF();

     //       qDebug() << "1 enu latLon = " << latLon << "; enu = " << enu;

     //       enuPoly << enu;
     //   }

     //   painter->setBrush(_fillColor);
     //   painter->drawPolygon(enuPoly);

     //   return;

     //   PolygonObject* poly  = new PolygonObject(view_, *qpf, transit_color);

     //   poly->setLatitude(loc.coordinate().latitude());
     //   poly->setLongitude(loc.coordinate().longitude());
     //   scene_->addObject(poly);

     //   #endif //def HIDE

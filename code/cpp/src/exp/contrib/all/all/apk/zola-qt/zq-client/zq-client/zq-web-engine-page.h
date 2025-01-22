
//           Copyright Nathaniel Christen 2020.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#ifndef ZQ_WEB_ENGINE_PAGE__H
#define ZQ_WEB_ENGINE_PAGE__H

#include <QWebEngineView>
#include <QMainWindow>
#include <QApplication>
#include <QTabWidget>
#include <QWebChannel>
#include <QWebEnginePage>
#include <QContextMenuEvent>
#include <QMenu>
#include <QWebEngineContextMenuData>

class ZQ_Web_Engine_View;

//class NavigationRequestInterceptor;
class NavigationRequestInterceptor : public QWebEnginePage
{
 QWebEnginePage* target;
public:
 NavigationRequestInterceptor(QWebEnginePage* parent);
 bool acceptNavigationRequest(const QUrl &url, NavigationType type,
                              bool isMainFrame) Q_DECL_OVERRIDE;
};


class ZQ_Web_Engine_Page : public QWebEnginePage
{
 Q_OBJECT

 QString last_youtube_link_;

 ZQ_Web_Engine_View* view_;

public:
 ZQ_Web_Engine_Page(ZQ_Web_Engine_View* view);
 ZQ_Web_Engine_Page(ZQ_Web_Engine_Page* parent);

 bool acceptNavigationRequest(const QUrl &url, NavigationType type,
                              bool isMainFrame) override;

 NavigationRequestInterceptor*
 createWindow(QWebEnginePage::WebWindowType type) override;

 void emit_navRequest(const QUrl &url);

 void reset_last_youtube_link(const QUrl &url);

 QString last_youtube_link_as_non_embed();

Q_SIGNALS:

 void navRequest(const QUrl& url, const QPoint pos);

};

#endif
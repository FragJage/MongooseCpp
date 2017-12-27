// Copyright (c) 2015 Cesanta Software Limited
// All rights reserved

#include <iostream>
#include "mongoose/mongoose.h"

using namespace std;

static const char *s_http_port = "8001";
static struct mg_serve_http_opts s_http_server_opts;

static void ev_handler(struct mg_connection *nc, int ev, void *p)
{
    cout << "EVT : " << ev << endl;
  if (ev == MG_EV_HTTP_REQUEST) {
    cout << "RQ..." << endl;
    mg_serve_http(nc, (struct http_message *) p, s_http_server_opts);
  }
}

int main(void) {
  struct mg_mgr mgr;
  struct mg_connection *nc;

  mg_mgr_init(&mgr, NULL);
  printf("Starting web server on port %s\n", s_http_port);
  nc = mg_bind(&mgr, s_http_port, ev_handler);
  if (nc == NULL) {
    printf("Failed to create listener\n");
    return 1;
  }

  // Set up HTTP server parameters
  mg_set_protocol_http_websocket(nc);
  s_http_server_opts.document_root = ".";  // Serve current directory
  s_http_server_opts.enable_directory_listing = "yes";

  for (;;) {
    mg_mgr_poll(&mgr, 1000);
  }
  mg_mgr_free(&mgr);

  return 0;
}

/*
server.setOption("document_root", "html");
server.setOption("listening_ports", "8080");
server.setOption("num_threads", "5");

server.start();

cout << "Test server started, press enter to quit..." << endl;
cin.ignore();

server.stop();
*/
/*
    m_PagesController.SetRootDir(documentRoot);
    m_PagesController.AddDefaultPage("index.html");
    m_WebMgr.AddRoute("/web/*", &m_PagesController);

    m_ModulesController.SetCacheManager(&m_CacheMgr);
    m_ModulesController.SetxPLSender(&m_xPLMgr);
    m_WebMgr.AddRoute("/api/v1/modules/[id]", &m_ModulesController);

    m_AdvanceConfigController.SetCacheManager(&m_CacheMgr);
    m_AdvanceConfigController.SetxPLSender(&m_xPLMgr);
    m_WebMgr.AddRoute("/api/v1/advanceconfig/[id]", &m_AdvanceConfigController);

    m_DevicesController.SetCacheManager(&m_CacheMgr);
    m_DevicesController.SetxPLSender(&m_xPLMgr);
    m_WebMgr.AddRoute("/api/v1/devices/[id]", &m_DevicesController);

    m_DataloggersController.SetCacheManager(&m_CacheMgr);
    m_DataloggersController.SetxPLSender(&m_xPLMgr);
    m_WebMgr.AddRoute("/api/v1/datalogger/{search}/[device]", &m_DataloggersController);

    m_MessagesController.SetxPLSender(&m_xPLMgr);
    m_WebMgr.AddRoute("/api/v1/messages/", &m_MessagesController);
*/

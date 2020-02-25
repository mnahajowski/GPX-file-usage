#include <stdio.h>

#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#define __USE_XOPEN

#include <time.h>

#include <math.h>

#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include <glib.h>

#include "gpx_file.h"

xmlXPathContextPtr xmlXPathNewContextNode(xmlDocPtr doc, xmlNodePtr node) {
    xmlXPathContextPtr val = xmlXPathNewContext(doc);
    xmlXPathRegisterNs(val, (xmlChar *) "gpx", (xmlChar *) "http://www.topografix.com/GPX/1/0");
    val->node = node;
    return val;
}

gpx_data *parse_gpx(char *file_name) {
    gpx_data *result = malloc(sizeof(gpx_data));

    xmlInitParser();
    xmlDocPtr doc = xmlParseFile(file_name);

    xmlXPathContextPtr context = xmlXPathNewContext(doc);
    xmlXPathRegisterNs(context, (xmlChar *) "gpx", (xmlChar *) "http://www.topografix.com/GPX/1/0");

    xmlXPathObjectPtr tracks = xmlXPathEvalExpression((xmlChar *) "//gpx:trk/gpx:trkseg", context);

    assert(tracks != NULL);
    assert(!xmlXPathNodeSetIsEmpty(tracks->nodesetval));

    xmlNodeSetPtr trackNodes = tracks->nodesetval;

    gpx_point curPt = {0};
    gpx_point prevPt = {0};

    xmlChar *end = 0;

    for (int i = 0; i < trackNodes->nodeNr; ++i) {
        if (trackNodes->nodeTab[i]->type != XML_ELEMENT_NODE) continue;

        xmlXPathContextPtr ptContext = xmlXPathNewContextNode(doc, trackNodes->nodeTab[i]);
        xmlXPathObjectPtr points = xmlXPathEvalExpression((xmlChar *) "gpx:trkpt", ptContext);

        assert(points != 0);
        assert(!xmlXPathNodeSetIsEmpty(points->nodesetval));

        prevPt.valid = false;

        result->points_length = points->nodesetval->nodeNr;
        result->points = (gpx_point *) malloc(result->points_length * sizeof(gpx_point));

        for (int j = 0; j < points->nodesetval->nodeNr; ++j) {
            curPt.valid = true;

            xmlNodePtr ptNode = points->nodesetval->nodeTab[j];

            if (xmlHasProp(ptNode, (xmlChar *) "lat") && xmlHasProp(ptNode, (xmlChar *) "lon")) {
                xmlChar *lat = xmlGetProp(ptNode, (xmlChar *) "lat");
                curPt.lat = strtod((char *) lat, (char **) &end);
                if (end == lat) {
                    curPt.valid = false;
                }

                xmlChar *lon = xmlGetProp(ptNode, (xmlChar *) "lon");
                curPt.lon = strtod((char *) lon, (char **) &end);
                if (end == lon) {
                    curPt.valid = false;
                }

                if (lon != 0) xmlFree(lon);
                if (lat != 0) xmlFree(lat);

            } else {
                curPt.valid = false;
                continue;
            }

            xmlXPathContextPtr eleContext = xmlXPathNewContextNode(doc, ptNode);
            xmlXPathObjectPtr elevation = xmlXPathEvalExpression((xmlChar *) "gpx:ele/text()", eleContext);
            if (elevation != 0 && xmlXPathNodeSetIsEmpty(elevation->nodesetval)) {
                curPt.valid = false;

            } else if (elevation != 0) {
                xmlChar *eVal = elevation->nodesetval->nodeTab[0]->content;
                if (end == eVal) {
                    curPt.valid = false;
                }
            }
            if (elevation) xmlXPathFreeObject(elevation);
            xmlXPathFreeContext(eleContext);

            xmlXPathContextPtr timeContext = xmlXPathNewContextNode(doc, ptNode);
            xmlXPathObjectPtr time = xmlXPathEvalExpression((xmlChar *) "gpx:time/text()", timeContext);
            if (time != 0 && xmlXPathNodeSetIsEmpty(time->nodesetval)) {
                curPt.valid = false;

            } else if (time != 0) {
                xmlChar *tVal = time->nodesetval->nodeTab[0]->content;
                if (strptime((char *) tVal, "%Y-%m-%dT%H:%M:%SZ", &(curPt.time)) == NULL)
                    curPt.valid = false;
            }
            if (time) xmlXPathFreeObject(time);
            xmlXPathFreeContext(timeContext);

            prevPt = curPt;
            result->points[j] = curPt;
        }
        xmlXPathFreeObject(points);
        xmlXPathFreeContext(ptContext);
    }

    xmlXPathFreeObject(tracks);
    xmlXPathFreeContext(context);

    xmlFreeDoc(doc);
    xmlCleanupParser();

    assert(result->points != NULL);

    return result;
}
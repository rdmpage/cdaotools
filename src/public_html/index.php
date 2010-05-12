<?php require_once 'header.php'  ?>
<div id="content" class="content">
<div class="header"><p>
CDAO-store is a repository providing a rich set of API's for querying and visualizing phyloinformatics data.  The store is a triple-store which encodes data as RDF triples constructed according to the CDAO concept vocabulary.  CDAO-store provides three classes of services: a service for importing data in CDAO format, a PhyloWS interface supporting an advanced set of queries for other external applications, and a web-interface for interacting with data in the store.
</p></div><br/><br/>



<div id="rssbox" class="rssbox">

  <!--
  // Created with a Google AJAX Search and Feed Wizard
  // http://code.google.com/apis/ajaxsearch/wizards.html
  -->

  <!--
  // The Following div element will end up holding the actual feed control.
  // You can place this anywhere on your page.
  -->
  <div class="feed-control" id="feed-control" >
    <span style="color:#676767;font-size:11px;margin:10px;padding:4px;">Loading...</span>
  </div>

  <!-- Google Ajax Api
  -->
  <script src="http://www.google.com/jsapi?key=notsupplied-wizard"
    type="text/javascript"></script>

  <!-- Dynamic Feed Control and Stylesheet -->
  <script src="http://www.google.com/uds/solutions/dynamicfeed/gfdynamicfeedcontrol.js"
    type="text/javascript"></script>
  <script type="text/javascript">
    function LoadDynamicFeedControl() {
      var feeds = [
	{title: 'cdaotools',
	 url: 'http://twitter.com/statuses/user_timeline/137491207.rss'
	}];
      var options = {
        stacked : true,
        horizontal : false,
        title : "cdao store",
        displayTime : 20000,
	fadeOutTime : 0
      }

      new GFdynamicFeedControl(feeds, 'feed-control', options);
    }
    // Load the feeds API and set the onload callback.
    google.load('feeds', '1');
    google.setOnLoadCallback(LoadDynamicFeedControl);
  </script>
<!-- ++End Dynamic Feed Control Wizard Generated Code++ -->

 </div>
</div>
<?php require_once 'footer.php' ?>

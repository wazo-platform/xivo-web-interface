<?php

#
# XiVO Web-Interface
# Copyright (C) 2006-2011  Avencall
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#


header('Access-Control-Allow-Origin: *');

$form = &$this -> get_module('form');
?>

<style type="text/css">
	/* Conteneur principal */

	#ConteneurPrincipal {

		width: 750px;
		height: 400px;
	}
</style>

<script type="text/javascript">
	$(function() {
		// On transforme notre conteneur principal en un conteneur d'onglets
		$("#ConteneurPrincipal").tabs();
		// On rend notre conteneur principal redimensionnable
		//$("#ConteneurPrincipal").resizable();
	}); 
</script>

<div id="ConteneurPrincipal" class="ui-widget-content">
	<ul>
		<li>
			<a href="#Configuration">Configuration</a>
		</li>
		<li>
			<a href="#Campagnes">Campagnes</a>
		</li>
	</ul>

	<div id="Configuration">
		<script type="text/javascript">
			console.log("getJSON");
			
		    $.ajaxSetup({
		        error: function(jqXHR, exception) {
		            if (jqXHR.status === 0) {
		                alert('Not connect.\n Verify Network.');
		            } else if (jqXHR.status == 404) {
		                alert('Requested page not found. [404]');
		            } else if (jqXHR.status == 500) {
		                alert('Internal Server Error [500].');
		            } else if (exception === 'parsererror') {
		                alert('Requested JSON parse failed.');
		            } else if (exception === 'timeout') {
		                alert('Time out error.');
		            } else if (exception === 'abort') {
		                alert('Ajax request aborted.');
		            } else {
		                alert('Uncaught Error.\n' + jqXHR.responseText);
		            }
		        }
		    });
			
			$.ajax({
				url : "http://192.168.51.100:5050/rest/record/",
				dataType : "application/json"
			}).done(function() {
				console.log("success");

				// $('<ul/>').appendTo('#Configuration');
				// $.each(data, function(key, val) {
				// $('<li id="' + key + '">' + val + '</li>').appendTo('#Configuration');
				// });
			}).fail(function(jqXHR, textStatus, errorThrown) {
				console.log("error " + textStatus);
				console.dir(jqXHR);

			}).complete(function() {
				console.log("completed");
			})

			console.log("Over");

		</script>
		<p>
			Text configuration
		</p>
	</div>

	<div id="Campagnes">
		<p>
			Text campagnes
		</p>
	</div>
</div>

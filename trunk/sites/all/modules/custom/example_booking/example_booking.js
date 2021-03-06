
jQuery.base64=(function($){var _PADCHAR="=",_ALPHA="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/",_VERSION="1.0";function _getbyte64(s,i){var idx=_ALPHA.indexOf(s.charAt(i));if(idx===-1){throw"Cannot decode base64"}return idx}function _decode(s){var pads=0,i,b10,imax=s.length,x=[];s=String(s);if(imax===0){return s}if(imax%4!==0){throw"Cannot decode base64"}if(s.charAt(imax-1)===_PADCHAR){pads=1;if(s.charAt(imax-2)===_PADCHAR){pads=2}imax-=4}for(i=0;i<imax;i+=4){b10=(_getbyte64(s,i)<<18)|(_getbyte64(s,i+1)<<12)|(_getbyte64(s,i+2)<<6)|_getbyte64(s,i+3);x.push(String.fromCharCode(b10>>16,(b10>>8)&255,b10&255))}switch(pads){case 1:b10=(_getbyte64(s,i)<<18)|(_getbyte64(s,i+1)<<12)|(_getbyte64(s,i+2)<<6);x.push(String.fromCharCode(b10>>16,(b10>>8)&255));break;case 2:b10=(_getbyte64(s,i)<<18)|(_getbyte64(s,i+1)<<12);x.push(String.fromCharCode(b10>>16));break}return x.join("")}function _getbyte(s,i){var x=s.charCodeAt(i);if(x>255){throw"INVALID_CHARACTER_ERR: DOM Exception 5"}return x}function _encode(s){if(arguments.length!==1){throw"SyntaxError: exactly one argument required"}s=String(s);var i,b10,x=[],imax=s.length-s.length%3;if(s.length===0){return s}for(i=0;i<imax;i+=3){b10=(_getbyte(s,i)<<16)|(_getbyte(s,i+1)<<8)|_getbyte(s,i+2);x.push(_ALPHA.charAt(b10>>18));x.push(_ALPHA.charAt((b10>>12)&63));x.push(_ALPHA.charAt((b10>>6)&63));x.push(_ALPHA.charAt(b10&63))}switch(s.length-imax){case 1:b10=_getbyte(s,i)<<16;x.push(_ALPHA.charAt(b10>>18)+_ALPHA.charAt((b10>>12)&63)+_PADCHAR+_PADCHAR);break;case 2:b10=(_getbyte(s,i)<<16)|(_getbyte(s,i+1)<<8);x.push(_ALPHA.charAt(b10>>18)+_ALPHA.charAt((b10>>12)&63)+_ALPHA.charAt((b10>>6)&63)+_PADCHAR);break}return x.join("")}return{decode:_decode,encode:_encode,VERSION:_VERSION}}(jQuery));

(function ($) {

  var FilterTypeAlreadyProcessed = false;
  
  function OnFilterTypeChange () {
  
    var form = $('fieldset#edit-booking');
    
    if (!form[0])
      return;
    
    var sport       = form.find('select#edit-sport');
    var type        = form.find('div.form-item-type');
    var className   = form.find('div.form-item-class');
    var court       = form.find('div.form-item-court');
    var instructor  = form.find('div.form-item-instructor');
    var duration    = form.find('div.form-item-duration');
    
    form.find ("select option[value='None']").attr('disabled', true);

    if (sport[0] && !FilterTypeAlreadyProcessed) {
    
      sport.change (OnFilterTypeChange);
      type.find ('select').change (OnFilterTypeChange);
      className.find ('select').change (OnFilterTypeChange);
      court.find ('select').change (OnFilterTypeChange);
      instructor.find ('select').change (OnFilterTypeChange);
      duration.find ('select').change (OnFilterTypeChange);
    }

    type.hide();
    type.find ('select').attr ('disabled', true);
    className.hide();
    className.find ('select').attr ('disabled', true);
    court.hide();
    court.find ('select').attr ('disabled', true);
    instructor.hide();
    instructor.find ('select').attr ('disabled', true);
    duration.hide();
    duration.find ('select').attr ('disabled', true);
    
    if (sport.val () != 'All')
    {
      type.show();
      type.find('select').removeAttr('disabled');
      
      if (type.find('select').val () != 'All')
      {
        if (type.find('select option:selected').val () == 'classes')
        {
          className.show();
          className.find('select').removeAttr('disabled');
          
          if (className.find('select').val () != 'All')
          {
            if (className.find('select option:selected').val () != 'All')
            {
              instructor.show();
              instructor.find('select').removeAttr('disabled');
            }
          }
        }
        else
        if (type.find('select option:selected').val () == 'courts')
        {
          court.show();
          court.find('select').removeAttr('disabled');
          duration.show();
          duration.find('select').removeAttr('disabled');
        }
        
        
        
      }
    }
  
    
    FilterTypeAlreadyProcessed = true;
  }
  
  function example_booking_onload() {
    $('.container-inline-date').find("input").eq(0).change(function(){$('.container-inline-date').find("input").eq(2).val(this.value)});
    $('.container-inline-date').find("input").eq(2).change(function(){$('.container-inline-date').find("input").eq(0).val(this.value)});
    
    OnFilterTypeChange ();
    
    if (!Drupal.settings.booking_timeslots_user_may_book)
      return;
    
    var GetGreenBoxInfo = function (td) {
      
      return JSON.parse (decodeURIComponent($(td)[0].id));
    };
    
    var GetNearHour = function (td, y) {
      
      var result = {};
      var info   = GetGreenBoxInfo (td);
      var tds    = [];
      
      $(td).parent ().parent ().find ('td.calendar-agenda-hour').each (function () {
        var obj;
        
        var hm = $(this).find ('.calendar-hour').html();
        
        if (hm < info.start_time || hm >= info.end_time)
          return;
        
        tds.push (obj = {
          y:   $(this).offset ().top,
          yl:  $(this).offset ().top + $(this).outerHeight (),
          hm:  hm
        });
        obj.m = parseInt (obj.hm.substr(0, 2), 10) * 60 + parseInt (obj.hm.substr(3, 5), 10);
      });
      
      var height = info.height ? info.height : parseInt ($('select#edit-duration option:selected').val ());
      
      for (var k = 0; k < tds.length; k++)
      {
        if (y >= tds[k].y && y <= tds[k].yl)
        {
          var halfway = {
            y:  tds[k].m - height / 2 + Drupal.settings.example_booking_calendar_granularity,
            yl: tds[k].m
          };
          
          if (halfway.yl >= (tds[tds.length - 1].m))
          {
            halfway.yObj  = tds[tds.length - height / Drupal.settings.example_booking_calendar_granularity];
            halfway.y     = tds[tds.length - height / Drupal.settings.example_booking_calendar_granularity].m;
            halfway.ylObj = tds[tds.length - 1];
            halfway.yl    = tds[tds.length - 1].m;
          }
          else
          {
            if (halfway.y <= tds[0].m)
            {
              halfway.yObj  = tds[0];
              halfway.y     = tds[0].m;
              halfway.ylObj = tds[height / Drupal.settings.example_booking_calendar_granularity - 1];
              halfway.yl    = tds[height / Drupal.settings.example_booking_calendar_granularity - 1].m;
            }
          }
        
          // Searching half block up

          for (var i = k; i >= 0; i--)
          {
            if (halfway.y >= tds[i].m && (tds.length >= i || halfway.y < tds[i + 1].m))
            {
              halfway.yObj = tds[i];
              halfway.y    = halfway.yObj.m;
              halfway.ylObj = tds[i + height / Drupal.settings.example_booking_calendar_granularity - 1];
              halfway.yl   = halfway.y + height;
              break;
            }
          }
          
          result.startDate     = info.date.substr (0, 10) + " " + halfway.yObj.hm;
          result.startHour     = halfway.yObj.hm;
          result.startPosition = halfway.yObj.y;
          result.endHour       = halfway.yObj.m + height;
          result.endHour       = (("0" + parseInt(result.endHour / 60, 10)).substr (-2)) + ":" + (("0" + parseInt(result.endHour % 60, 10)).substr (-2));
          result.height        = halfway.ylObj.yl - halfway.yObj.y;
                    
          break;
        }
      }
      
      
      var price_key = Drupal.settings.example_booking_user_is_member ? 'members' : 'non_members';
      
      if (
          info.data.price.type == 'regular'     && (!info.data.price.regular     || !info.data.price.regular[price_key] || info.data.price.regular[price_key].length == 0) ||
          info.data.price.type == 'non_regular' && (!info.data.price.non_regular[height] || info.data.price.non_regular[height][price_key].length == 0)
          )
        result.price = 'N/A';
      else
        result.price = '$' + (info.data.price.type == 'regular' ? info.data.price.regular[price_key] : info.data.price.non_regular[height][price_key]);
      
      return result;
    }
    
    $('td.calendar-day-green-box').click (function (e) {
	   window.location.href = jQuery.base64.decode( $(this).attr('link') );
    });
    
    $('td.calendar-item-green-box, td.calendar-item-available').mouseleave (function (e) {
      if (window.GreenBoxItem)
      {
        window.GreenBoxItem.parentNode.removeChild (window.GreenBoxItem);
        window.GreenBoxItem = false;
      }
    });
    
    $('td.calendar-item-available').mousemove (function (e) {
      
      var data = JSON.parse (decodeURIComponent($(this)[0].id)); 
    
      GreenBoxShowArrow.apply(this, [{
        height: 0,
        price: data.price,
        startDate: data.date,
        startHour: data.start_time,
        startPosition: jQuery(this).offset().top + jQuery(this).height()/2,
        endHour: data.end_time
      }, false])
      
    });
    
    $('td.calendar-item-green-box').mousemove (function (e) {
      
      var info = GetGreenBoxInfo (this);
      
      // Searching for the hour near the mouse cursor
      
      var x = e.pageX;
      var y = e.pageY;
      
      var result = GetNearHour (this, y);

      if (!result.height)
        return;
      
      
      var topTd = this;
      
      GreenBoxShowArrow.apply (this, [result]);
      
    });
    
    function GreenBoxShowArrow (result, showPlaceholder) {
      
      if (window.GreenBoxItem)
      {
        if (window.GreenBoxItem._lastDate == result.startDate)
          return;
        
        window.GreenBoxItem.parentNode.removeChild (window.GreenBoxItem);
        window.GreenBoxItem = false;
      }

      window.GreenBoxItem = document.createElement ('table');
      
      var tr = document.createElement ('tr');
      var td = document.createElement ('td');
      
      
      td.innerHTML = "book now!";
      td.style.verticalAlign = "middle";
      td.style.textAlign = "center";
      td.style.width = '100%';
      td.setAttribute('class', 'book-now')
      
      if (showPlaceholder != false)
      {
        window.GreenBoxItem.appendChild (tr);
        tr.appendChild (td)
      }

      
      window.GreenBoxItem.style.position = "relative";
      
      
      window.GreenBoxItem.style.top = result.startPosition - $(this).offset ().top + "px";
      window.GreenBoxItem.style.height = result.height - 1 + "px";
      
      window.GreenBoxItem.setAttribute ('class', 'calendar-item-unavailable calendar-item-greenbox-virtualslot');
      window.GreenBoxItem._lastDate = result.startDate;
      
      var height = result.height;

      var rightInfoArrow = document.createElement ('div');
      rightInfoArrow.setAttribute ('class', 'green-box-arrow-box');
      rightInfoArrow.style.left   = $(this).offset ().left + $(this).outerWidth () + $(window).scrollLeft() - 15 + "px";
      rightInfoArrow.style.top    = result.startPosition - $(window).scrollTop() + height / 2 - 30 / 2 + "px";
//      rightInfoArrow.style.width  = "20px";
//      rightInfoArrow.style.height = "22px";
      rightInfoArrow.innerHTML = "<div class='top'></div><div class='bottom'></div>";
      window.GreenBoxItem.appendChild (rightInfoArrow);
      
      var rightInfo = document.createElement ('div');
      rightInfo.setAttribute ('class', 'green-box-info-box');
      rightInfo.style.left   = $(this).offset ().left + $(this).outerWidth () + $(window).scrollLeft() + 13 + "px";
      rightInfo.style.top    = result.startPosition - $(window).scrollTop() + height / 2 - 30 / 2 + "px";
      rightInfo.style.width  = "100px";
      rightInfo.style.height = "22px";
      rightInfo.innerHTML = result.startHour + " - " + result.endHour;
      window.GreenBoxItem.appendChild (rightInfo);

      var rightPayInfo = document.createElement ('div');
      rightPayInfo.setAttribute ('class', 'green-box-info-pay-box');
      rightPayInfo.style.left   = $(this).offset ().left + $(this).outerWidth () + $(window).scrollLeft () + 112 + "px";
      rightPayInfo.style.top    = result.startPosition - $(window).scrollTop() + height / 2 - 40 / 2 + "px";
      rightPayInfo.style.width  = "26px";
      rightPayInfo.style.height = "25px";
      rightPayInfo.innerHTML = result.price;
      
      window.GreenBoxItem.appendChild (rightPayInfo);
      
      window.GreenBoxItem.onclick = function () {
        
        var info = GetGreenBoxInfo (arguments.callee._greenBox); 
        var slot = arguments.callee._slot;
        
        var height = info.height ? info.height : parseInt ($('select#edit-duration option:selected').val ());
        
        var args = jQuery.base64.encode (JSON.stringify([info.primary_id, info.secondary_id, info.tertiary_id, info.cid, slot.startDate, height]));
        
        window.location.href = Drupal.settings.basePath + "booking/add/" + args;
      }
      
      window.GreenBoxItem.onclick._greenBox = this;
      window.GreenBoxItem.onclick._slot     = result;
      
      if (showPlaceholder == false)
      {
//        window.GreenBoxItem.style.visibility = 'hidden';
  //      window.GreenBoxItem.style.height = '0px';
    //    window.GreenBoxItem.style.display = 'inline';
      }
      this.appendChild (window.GreenBoxItem);
    }
  }
  
  var lastDateSent = '';
  
  $(document).ready(function() {
    example_booking_onload();
  });
  
  $(document).ajaxSend(function(event, jqXHR, ajaxOptions){
    if(!ajaxOptions.url.match('/opening_hours/i')) {
      example_booking_onload();
      var dateFound = false;
      console.log(event);
      console.log(jqXHR);
      
      if (!ajaxOptions.data)
        ajaxOptions.data = '';
      
      var paramArr = ajaxOptions.data.split('&');
      for(var i in paramArr)
      {
        var param = paramArr[i].split('=');
	if(param[0] == 'date') {
	  dateFound = true;
	  lastDateSent = param[1];
	}
      }
      console.log(dateFound);
      console.log(lastDateSent);
      if(!dateFound && lastDateSent != '') {
	ajaxOptions.data = ajaxOptions.data + '&date=' + lastDateSent;
	console.log("attaching data");
      }
    }
  });
  
  $(document).ajaxSuccess(function(event, XMLHttpRequest, ajaxOptions) {
    if(!ajaxOptions.url.match('/opening_hours/i')) {
      example_booking_onload();
    }
  });  
})(jQuery);
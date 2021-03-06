Booking Example Site
====================

This site provides demo for testing [Booking Time Slots module](https://drupal.org/project/booking_timeslots).

Requirements
------------

 - Unix-like command-line access
 - configured LAMP environment
 - drush
 
Installation
------------

1. Check database credentials in trunk/sites/default/settings.php (you might need to copy and rename default.settings.php into settings.php)
2. Install environment by the following command from webroot:

        cd ./scripts && ./build_env.sh

3. Access your site by pointing your HTTP server to site's webroot trunk directory, e.g. http://booking.example/

Demo
----
1. Log-in as admin (admin/asdf).
2. Open any of the venues and go to the "Opening Hours" tab.
3. Click on some day of week to add venue opening hours.
4. Enter start time e.g. 08:00 and end time e.g. 20:00. It will restrict facilities' and classes' opening hours to be lay between specified hours.
5. Open any of the facilities and go to "Edit" tab.
6. Specify previously edited venue as the "Venue Facility belongs to". You may also specifiy categories(sports) of the facility. You may later filter facilities by that category.
7. Go to the "Opening Hours" tab of the facility.
8. Click on the day when previously edited venue is open and specify facility opening hours (must lay between venue opening hours), pricing and other things. Please use non regular hours for testing.
9. You may finally open the venue and go to "Schedule" tab to display calendar and be able to book slots.
10. If you create class, you may relate it with the facility. This will allow you to create bookable slots of custom length (user must book the whole slot instead of selecting custom length).
11. If you create instructor, you may relate it with the class to enable filtering classes by instructor (you won't be able to create slots per instructor so 4th level content type acts like classes filter).

Those steps should allow you to book timeslots.

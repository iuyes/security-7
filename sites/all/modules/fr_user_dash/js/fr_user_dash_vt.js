(function(){
// The following line will prevent a JavaScript error if this file is included and vertical tabs are disabled.
Drupal.verticalTabs = Drupal.verticalTabs || {};

// Note the name here matches the name of the fieldset ID.
Drupal.verticalTabs.myfieldset = function() {
  if ($('#edit-fr_user_dash-option1').size()) {
    if ($('#edit-fr_user_dash-option1').attr('checked')) {
      return Drupal.t('Option 1 enabled');
    }
    else {
      return Drupal.t('Option 1 disabled');
    }
  }
  else {
    return '';
  }
}
})(jQuery);
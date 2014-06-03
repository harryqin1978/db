<?php

function contact_us_admin_information($form, &$form_state) {

  /*
  $form['description'] = array(
    '#type' => 'item',
    '#title' => t('Description above the form'),
  );
  */

  $form['address'] = array(
    '#type' => 'text_format',
    '#title' => t('Address'),
    '#default_value' => variable_get('contact_us_information_address', ''),
  );

  $form['phone'] = array(
    '#type' => 'textfield',
    '#title' => t('Phone number'),
    '#default_value' => variable_get('contact_us_information_phone', ''),
  );

  $form['fax'] = array(
    '#type' => 'textfield',
    '#title' => t('Fax number'),
    '#default_value' => variable_get('contact_us_information_fax', ''),
  );

  $form['email'] = array(
    '#type' => 'textfield',
    '#title' => t('Email'),
    '#default_value' => variable_get('contact_us_information_email', ''),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );

  return $form;

}



function contact_us_admin_information_validate($form, &$form_state) {

  if ( $form_state['values']['phone'] && !preg_match('/^[0-9|-]{6,20}$/',$form_state['values']['phone']) ) {
    form_set_error('phone', t('Phone number format is not correct.'));
  }

  if ( $form_state['values']['fax'] && !preg_match('/^[0-9|-]{6,20}$/',$form_state['values']['fax']) ) {
    form_set_error('fax', t('Fax number format is not correct.'));
  }

  if ( $form_state['values']['email'] && !valid_email_address($form_state['values']['email']) ) {
    form_set_error('email', t('Email format is not correct.'));
  }

}



function contact_us_admin_information_submit($form, &$form_state) {

  variable_set('contact_us_information_address', $form_state['values']['address']['value']);
  variable_set('contact_us_information_phone', $form_state['values']['phone']);
  variable_set('contact_us_information_fax', $form_state['values']['fax']);
  variable_set('contact_us_information_email', $form_state['values']['email']);

  drupal_set_message(t('Update complete.'));

}



function contact_us_admin_location($form, &$form_state) {

  $form['maptype'] = array(
    '#type' => 'select',
    '#title' => t('Type of map'),
    '#options' => array(
      '' => t('None'),
      'google_map' => t('Google map'),
      'upload_map' => t('Upload map'),
    ),
    '#default_value' => variable_get('contact_us_location_maptype', ''),
  );

  $form['address'] = array(
    '#type' => 'textfield',
    '#title' => t('Location address'),
    '#default_value' => variable_get('contact_us_location_address', ''),
    '#states' => array(
      'visible' => array(
        ':input[name="maptype"]' => array('value' => 'google_map'),
      ),
    ),
  );

  $form['mapupload'] = array(
    '#type' => 'file',
    '#title' => t('Upload location map'),
    '#size' => 48,
    '#description' => t('Upload a file, allowed extensions: jpg, jpeg, png, gif'),
    '#states' => array(
      'visible' => array(
        ':input[name="maptype"]' => array('value' => 'upload_map'),
      ),
    ),
  );
  
  $mapupload = variable_get('contact_us_location_mapupload', '');
  
  if ($mapupload) {
    $form['mapupload_preview'] = array(
      '#type' => 'item',
      '#markup' => theme('image_style', array('style_name' => 'thumbnail', 'path' => $mapupload, 'getsize' => FALSE)),
      '#states' => array(
        'visible' => array(
          ':input[name="maptype"]' => array('value' => 'upload_map'),
        ),
      ),
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );

  return $form;

}



function contact_us_admin_location_validate($form, &$form_state) {

  $mapupload = file_save_upload('mapupload', array(
    'file_validate_is_image' => array(), // Validates file is really an image.
    'file_validate_extensions' => array('png gif jpg jpeg'), // Validate extensions.
  ));
  
  // If the file passed validation:
  if ($mapupload) {
    // Move the file, into the Drupal file system
    if ($mapupload = file_move($mapupload, 'public://')) {
      // Save the file for use in the submit handler.
      $form_state['storage']['mapupload'] = $mapupload;
    }
    else {
      form_set_error('mapupload', t('Failed to write the uploaded file the site\'s file folder.'));
    }
  }
  else {
    // form_set_error('mapupload', t('No file was uploaded.'));
  }
  
  if ( $form_state['values']['maptype']=='upload_map' && !$mapupload && !variable_get('contact_us_location_mapupload', '') ) {
    form_set_error('mapupload', t('No file was uploaded.'));
  }

  if ( $form_state['values']['maptype']=='google_map' && !$form_state['values']['address'] ) {
    form_set_error('address', t('No location address.'));
  }

}



function contact_us_admin_location_submit($form, &$form_state) {

  variable_set('contact_us_location_maptype', $form_state['values']['maptype']);
  variable_set('contact_us_location_address', $form_state['values']['address']);

  if (isset($form_state['storage']['mapupload'])) {
    
    $mapupload = $form_state['storage']['mapupload'];
    // We are done with the file, remove it from storage.
    unset($form_state['storage']['mapupload']);
    // Make the storage of the file permanent
    $mapupload->status = FILE_STATUS_PERMANENT;
    // Save file status.
    file_save($mapupload);
    // Set a response to the user.
    
    variable_set('contact_us_location_mapupload', $mapupload->uri);
  
  }

  drupal_set_message(t('Update complete.'));

}

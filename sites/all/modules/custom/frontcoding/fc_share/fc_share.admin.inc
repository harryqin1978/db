<?php 

/*
 * builed the share admin form 
 * 
 * */

function fc_share_admin_information(){
	
	$form['share']['sina_ico'] = array(
		'#type' => 'managed_file',
		'#title' => t('Sina ICO'),
		'#upload_location'    => 'public://',
		"#upload_validators"  => array("file_validate_extensions" => array("png gif jpg jpeg ico")),
		'#default_value' => variable_get('sina_ico', ''),
	);
	$form['share']['sina_url'] = array(
		'#type' => 'textfield',
		'#title' => t('Sina URL'),
		'#default_value' => variable_get('sina_url', ''),
	);
	$form['share']['sina_title'] = array(
		'#type' => 'checkbox',
		'#title' => t('Open Sina Share'),
		'#default_value' => variable_get('sina_title', ''),
	);
	$form['share']['tencent_ico'] = array(
		'#type' => 'managed_file',
		'#title' => t('Tencent ICO'),
		'#upload_location'    => 'public://',
		"#upload_validators"  => array("file_validate_extensions" => array("png gif jpg jpeg ico")),			
		'#default_value' => variable_get('tencent_ico', ''),
	);
	$form['share']['tencent_url'] = array(
		'#type' => 'textfield',
		'#title' => t('Tencent URL'),
		'#default_value' => variable_get('tencent_url', ''),
	);
	$form['share']['tencent_title'] = array(
		'#type' => 'checkbox',
		'#title' => t('Open Tencent Share'),
		'#default_value' => variable_get('tencent_title', ''),
	);
	$form['share']['kaixin_ico'] = array(
		'#type' => 'managed_file',
		'#title' => t('Kaixin ICO'),
		'#upload_location'    => 'public://',
		"#upload_validators"  => array("file_validate_extensions" => array("png gif jpg jpeg ico")),	
		'#default_value' => variable_get('kaixin_ico', ''),		
	);
	$form['share']['kaixin_url'] = array(
		'#type' => 'textfield',
		'#title' => t('Kaixin URL'),
		'#default_value' => variable_get('kaixin_url', ''),	
	);
	$form['share']['kaixin_title'] = array(
		'#type' => 'checkbox',
		'#title' => t('Open Kaixin Share'),
		'#default_value' => variable_get('kaixin_title', ''),	
	);
	$form['share']['renren_ico'] = array(
		'#type' => 'managed_file',
		'#title' => t('Renren ICO'),
		'#upload_location'    => 'public://',
		"#upload_validators"  => array("file_validate_extensions" => array("png gif jpg jpeg ico")),		
		'#default_value' => variable_get('renren_ico', ''),	
	);
	$form['share']['renren_url'] = array(
		'#type' => 'textfield',
		'#title' => t('Renren URL'),
		'#default_value' => variable_get('renren_url', ''),	
	);
	$form['share']['renren_title'] = array(
		'#type' => 'checkbox',
		'#title' => t('Open Renren Share'),
		'#default_value' => variable_get('renren_title', ''),	
	);
	$form['share']['linkedin_ico'] = array(
		'#type' => 'managed_file',
		'#title' => t('Linkedin ICO'),
		'#upload_location'    => 'public://',
		"#upload_validators"  => array("file_validate_extensions" => array("png gif jpg jpeg ico")),		
		'#default_value' => variable_get('linkedin_ico', ''),	
	);
	$form['share']['linkedin_url'] = array(
		'#type' => 'textfield',
		'#title' => t('Linkedin URL'),
		'#default_value' => variable_get('linkedin_url', ''),	
	);
	$form['share']['linkedin_title'] = array(
		'#type' => 'checkbox',
		'#title' => t('Open Linkedin Share'),
		'#default_value' => variable_get('linkedin_title', ''),	
	);		
	$form['share']['twitter_ico'] = array(
		'#type' => 'managed_file',
		'#title' => t('Twitter ICO'),
		'#upload_location'    => 'public://',
		"#upload_validators"  => array("file_validate_extensions" => array("png gif jpg jpeg ico")),		
		'#default_value' => variable_get('twitter_ico', ''),	
	);
	$form['share']['twitter_url'] = array(
		'#type' => 'textfield',
		'#title' => t('Twitter URL'),
		'#default_value' => variable_get('twitter_url', ''),	
	);
	$form['share']['twitter_title'] = array(
		'#type' => 'checkbox',
		'#title' => t('Open Twitter Share'),
		'#default_value' => variable_get('twitter_title', ''),	
	);	
	$form['share']['facebook_ico'] = array(
		'#type' => 'managed_file',
		'#title' => t('Facebook ICO'),
		'#upload_location'    => 'public://',
		"#upload_validators"  => array("file_validate_extensions" => array("png gif jpg jpeg ico")),		
		'#default_value' => variable_get('facebook_ico', ''),	
	);
	$form['share']['facebook_url'] = array(
		'#type' => 'textfield',
		'#title' => t('Facebook URL'),
		'#default_value' => variable_get('facebook_url', ''),	
	);
	$form['share']['facebook_title'] = array(
		'#type' => 'checkbox',
		'#title' => t('Open Facebook Share'),
		'#default_value' => variable_get('facebook_title', ''),	
	);	
  $form['share']['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );
	
	
	return $form;
}
/*
 * 
 * form validate
 * 
 * 

function fc_share_admin_information_validate($form, &$form_state){
		
}
*/

/*
 * 
 * set the values
 * $variable
 * 
 * */

function fc_share_admin_information_submit($form, &$form_state) {
	
	//print_r($form_state['values']['renren_url']);
  //variable_set('contact_us_information_address', $form_state['values']['address']['value']);
  
	
	//Sina Info
	variable_set('sina_ico',$form_state['values']['sina_ico']);
	variable_set('sina_url',$form_state['values']['sina_url']);
	variable_set('sina_title',$form_state['values']['sina_title']);
	//Tencent Info
	variable_set('tencent_ico',$form_state['values']['tencent_ico']);
	variable_set('tencent_url',$form_state['values']['tencent_url']);
	variable_set('tencent_title',$form_state['values']['tencent_title']);	
	//Kaixin Info
	variable_set('kaixin_ico',$form_state['values']['kaixin_ico']);
	variable_set('kaixin_url',$form_state['values']['kaixin_url']);
	variable_set('kaixin_title',$form_state['values']['kaixin_title']);	
	//Renren Info
	variable_set('renren_ico',$form_state['values']['renren_ico']);
	variable_set('renren_url',$form_state['values']['renren_url']);
	variable_set('renren_title',$form_state['values']['renren_title']);	
	//Linkedin Info
	variable_set('linkedin_ico',$form_state['values']['linkedin_ico']);
	variable_set('linkedin_url',$form_state['values']['linkedin_url']);
	variable_set('linkedin_title',$form_state['values']['linkedin_title']);	
	//Twitter Info
	variable_set('twitter_ico',$form_state['values']['twitter_ico']);
	variable_set('twitter_url',$form_state['values']['twitter_url']);
	variable_set('twitter_title',$form_state['values']['twitter_title']);	
	//Facebook Info
	variable_set('facebook_ico',$form_state['values']['facebook_ico']);
	variable_set('facebook_url',$form_state['values']['facebook_url']);
	variable_set('facebook_title',$form_state['values']['facebook_title']);	
	
  drupal_set_message(t('update successful!'));

}




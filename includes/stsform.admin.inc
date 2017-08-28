<?php


function stsform_admin_settings_form($form, &$form_state) {
    $form = array();
    $form['stsform_Default_ForwardingSettings'] = array(
		'#type' => 'fieldset',
     	'#title' => t('Default Form Forwarding Address'),
		'#description' => t('Please enter in the URL the form should be sent too.'),
	);
	$form['stsform_Default_ForwardingSettings']['stsform_Default_status'] = array(
	 	'#type' => 'checkbox',
		'#default_value' => variable_get('stsform_defaultStatus'),
	 	'#title' => t('Turn on default forwarding address'),
	);
	$form['stsform_Default_ForwardingSettings']['stsform_Default_check'] = array(
	 	'#type' => 'container',
	 	'#states' => array(
	 		'visible' => array(
	 			':input[name="stsform_Default_status"]' => array('checked' => TRUE),
			),	
	 	),
	);
 
	if(variable_get('stsform_defaultStatus') == 1) {
		$form['stsform_Default_ForwardingSettings']['stsform_Default_check']['stsform_Default_URL'] = array(
	 	'#type' => 'textfield',
    	'#title' => t('Enter URL Name'),
		'#description' => t("Please enter the default forwarding URL. This URL will be used only if forwarding is turned on and if no custom URL is specified for a given form."),
		'#default_value' => variable_get('stsform_defaultURL'),
		);
	} else {
		$form['stsform_Default_ForwardingSettings']['stsform_Default_check']['stsform_Default_URL'] = array(
	 	'#type' => 'textfield',
    	'#title' => t('Enter URL Name'),
		'#description' => t("Please enter the default forwarding URL. This URL will be used only if forwarding is turned on and if no custom URL is specified for a given form."),
		'#default_value' => "",
		);
	}
	
	$form['#validate'][] = 'stsform_admin_settings_validate';

	$form['#submit'] = array(
		'stsform_admin_settings_submit',
	);
    return system_settings_form($form);
    
} 

function stsform_admin_settings_validate($form, &$form_state) {

	//If no default URL is specified and forwarding is on
	if ($form_state['values']['stsform_Default_status'] == 1) {
		if (empty($form_state['values']['stsform_Default_URL'])) {
			form_set_error('stsform_URL', 'Please enter a url.');
		}
	} 
}


/**
 * Submit handler for the stsform_admin_settings() form.
 */
function stsform_admin_settings_submit($form, &$form_state) {
	variable_set('stsform_defaultURL', $form_state['values']['stsform_Default_URL']);
	variable_set('stsform_defaultStatus', $form_state['values']['stsform_Default_status']);
}
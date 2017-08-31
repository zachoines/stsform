<?php
function stsform_configure_form($form, &$form_state, $node) {
	

	$form = array();

	$nid = $node->nid;
	
	//Sets up the forwarding form
	$form['stsform_ForwardingSettings'] = array(
		'#type' => 'fieldset',
     	'#title' => t('Forwarding Settings'),
		'#description' => t('Please enter in the URL the form should be sent too.'),
	);
	$form['stsform_ForwardingSettings']['stsform_status'] = array(
	 	'#type' => 'checkbox',
	 	'#title' => t('Turn on Forwarding'),
	);
	$form['stsform_ForwardingSettings']['stsform_check'] = array(
	 	'#type' => 'container',
	 	'#states' => array(
	 		'visible' => array(
	 			':input[name="stsform_status"]' => array('checked' => TRUE),
			),	
	 	),
	$form['stsform_ForwardingSettings']['stsform_check']['stsform_URL'] = array(
	 	'#type' => 'textfield',
    	'#title' => t('Enter URL Name'),
	));
	$form['#submit'] = array(
		'stsform_form_submit',
	);

	//This will be sent along with the POST 
	$form['#node'] = $node;

	$form['nid'] = array(
		'#type' => 'value',
		'#value' => $node->nid,
 	);
	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Save Configuration'),
  	);
	
	return $form;
}

//check for entered url
function  stsform_form_validate($form, &$form_state) {
	$url = $form_state['values']['stsform_URL'];
	$forwarding = $form_state['values']['stsform_status'];
	$stsform_defaultStatus = variable_get('stsform_defaultStatus');
	$url = trim($url); 

	//If no default URL is specified and forwarding is on
	if (($forwarding == 1) && ($stsform_defaultStatus == 0)) {
		if ($url == "") {
			form_set_error('stsform_URL', 'Please enter a url.');
		}
	} 
}

function stsform_form_submit($form, &$form_state) {
	$node = $form['#node'];	 
	$record = stsform_get_record($node->nid);
	$forwarding = $form_state['values']['stsform_status'];
	$stsform_defaultStatus = variable_get('stsform_defaultStatus');
	$url = $form_state['values']['stsform_URL'];
	$url = trim($url); 

	//If no record, set to default values
	if ($record === FALSE) {
		
		//Build the default record if forwarding is on, but no url is specified.
		if (($forwarding == 1) && ($stsform_defaultStatus == 1) && empty($url)) {
			$record = new StdClass();
			$record->nid = $node->nid;
			$record->url =  trim(variable_get('stsform_defaultURL'));
			$record->forwarding = $form_state['values']['stsform_status'];
		
		//if fowarding is on and there is a url specified
		} else if (($forwarding == 1) && !empty($url)) {
			$record = new StdClass();
			$record->nid = $node->nid;
			$record->url =  trim($form_state['values']['stsform_URL']); 		
			$record->forwarding = $form_state['values']['stsform_status'];
		
		} 

		//Save the record
		stsform_set_record($record);
		drupal_set_message(t('The form settings have been updated.'));
	
	} else {
		if (($url == "") && ($forwarding == 1) && ($stsform_defaultStatus == 1)) {
			$record->nid = $node->nid;
			$record->url =  trim(variable_get('stsform_defaultURL'));
			$record->forwarding = $form_state['values']['stsform_status'];
		}
		
		else if (($forwarding == 0 && $stsform_defaultStatus == 1)) {
			$record->nid = $node->nid;
			$record->url = trim(variable_get('stsform_defaultURL')); 		
			$record->forwarding = $form_state['values']['stsform_status']; 
		} 
		
		else {
			$record->nid = $node->nid;
			$record->url = trim($form_state['values']['stsform_URL']); 		
			$record->forwarding = $form_state['values']['stsform_status']; 
		
		}
		
		stsform_set_record($record);
		drupal_set_message(t('The form settings have been updated.'));
	}
	
}



